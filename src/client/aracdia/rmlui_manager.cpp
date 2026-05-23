// Luanti / Aracdia
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "config.h"

#if ENABLE_ARACDIA_RMLUI

#include "client/aracdia/rmlui_manager.h"
#include "client/aracdia/rmlui_system_interface.h"
#include "filesys.h"
#include "log.h"
#include "porting.h"
#include "settings.h"

#include <RmlUi/Core/Context.h>
#include <RmlUi/Core/Core.h>
#include <RmlUi/Core/ElementDocument.h>
#include <RmlUi/Core/Input.h>
#include <RmlUi_Renderer_GL3.h>

#include <IVideoDriver.h>
#include <memory>

namespace {

AracdiaRmlSystemInterface g_system_interface;
std::unique_ptr<RenderInterface_GL3> g_renderer;
Rml::Context *g_context = nullptr;
Rml::ElementDocument *g_document = nullptr;

const char *const kSpikeDocument = R"RML(
<rml>
<head>
<style>
body {
	font-family: sans-serif;
	width: 100%;
	height: 100%;
}
#panel {
	position: absolute;
	left: 50%;
	top: 50%;
	transform: translate(-50%, -50%);
	width: 420dp;
	padding: 24dp;
	background: rgba(14, 14, 20, 0.92);
	border: 1dp rgba(99, 102, 241, 0.45);
	border-radius: 12dp;
	color: #E0E7FF;
	text-align: center;
}
h1 {
	font-size: 22dp;
	margin-bottom: 8dp;
}
p {
	color: #A8A8B8;
	font-size: 13dp;
	margin-bottom: 6dp;
}
small {
	color: #6366F1;
	font-size: 11dp;
}
</style>
</head>
<body>
	<div id="panel">
		<h1>Aracdia UI</h1>
		<p>RmlUi natif — spike C.0</p>
		<small>F8 pour afficher / masquer</small>
	</div>
</body>
</rml>
)RML";

bool load_default_font()
{
	std::string font_path = g_settings->get("font_path");
	if (font_path.empty())
		font_path = "fonts/Arimo-Regular.ttf";

	std::string absolute = porting::path_share + DIR_DELIM + font_path;
	if (!fs::PathExists(absolute)) {
		warningstream << "Aracdia RmlUi: font not found at " << absolute << std::endl;
		return false;
	}

	if (!Rml::LoadFontFace(absolute)) {
		warningstream << "Aracdia RmlUi: failed to load font " << absolute << std::endl;
		return false;
	}
	return true;
}

} // namespace

AracdiaRmlUiManager &AracdiaRmlUiManager::get()
{
	static AracdiaRmlUiManager instance;
	return instance;
}

void AracdiaRmlUiManager::init(video::IVideoDriver *driver)
{
	if (m_initialized || !driver)
		return;

	m_width = driver->getScreenSize().Width;
	m_height = driver->getScreenSize().Height;

	Rml::String gl_message;
	if (!RmlGL3::Initialize(&gl_message)) {
		errorstream << "Aracdia RmlUi: OpenGL loader failed: " << gl_message << std::endl;
		return;
	}
	infostream << "Aracdia RmlUi: " << gl_message << std::endl;

	g_renderer = std::make_unique<RenderInterface_GL3>();
	if (!*g_renderer) {
		errorstream << "Aracdia RmlUi: GL3 renderer construction failed" << std::endl;
		g_renderer.reset();
		RmlGL3::Shutdown();
		return;
	}

	Rml::SetSystemInterface(&g_system_interface);
	Rml::SetRenderInterface(g_renderer.get());

	if (!Rml::Initialise()) {
		errorstream << "Aracdia RmlUi: Rml::Initialise failed" << std::endl;
		g_renderer.reset();
		RmlGL3::Shutdown();
		return;
	}

	load_default_font();

	g_context = Rml::CreateContext("aracdia_main",
			Rml::Vector2i(m_width, m_height));
	if (!g_context) {
		errorstream << "Aracdia RmlUi: failed to create context" << std::endl;
		Rml::Shutdown();
		g_renderer.reset();
		RmlGL3::Shutdown();
		return;
	}

	g_document = g_context->LoadDocumentFromMemory(kSpikeDocument, "aracdia_spike.rml");
	if (!g_document) {
		errorstream << "Aracdia RmlUi: failed to load spike document" << std::endl;
		Rml::RemoveContext("aracdia_main");
		g_context = nullptr;
		Rml::Shutdown();
		g_renderer.reset();
		RmlGL3::Shutdown();
		return;
	}

	g_document->Show();
	m_initialized = true;
	infostream << "Aracdia RmlUi: overlay ready (" << m_width << "x" << m_height << ")"
	           << std::endl;
}

void AracdiaRmlUiManager::shutdown()
{
	if (!m_initialized)
		return;

	if (g_document) {
		g_document->Close();
		g_document = nullptr;
	}
	if (g_context) {
		Rml::RemoveContext("aracdia_main");
		g_context = nullptr;
	}

	Rml::Shutdown();
	g_renderer.reset();
	RmlGL3::Shutdown();

	m_initialized = false;
}

void AracdiaRmlUiManager::onResize(int width, int height)
{
	m_width = width;
	m_height = height;
	if (g_context)
		g_context->SetDimensions(Rml::Vector2i(width, height));
	if (g_renderer)
		g_renderer->SetViewport(width, height);
}

void AracdiaRmlUiManager::update(float dtime)
{
	(void)dtime;
	if (!m_initialized || !m_visible || !g_context)
		return;

	g_context->Update();
}

void AracdiaRmlUiManager::render()
{
	if (!m_initialized || !m_visible || !g_context || !g_renderer)
		return;

	g_renderer->SetViewport(m_width, m_height);
	g_renderer->BeginFrame();
	g_context->Render();
	g_renderer->EndFrame();
}

#endif // ENABLE_ARACDIA_RMLUI
