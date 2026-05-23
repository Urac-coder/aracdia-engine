// Luanti / Aracdia
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include "config.h"

#if ENABLE_ARACDIA_RMLUI

struct RunStats;

namespace video {
class IVideoDriver;
}

/// Client-side RmlUi overlay (Phase C.0 spike).
class AracdiaRmlUiManager {
public:
	static AracdiaRmlUiManager &get();

	void init(video::IVideoDriver *driver);
	void shutdown();

	void setVisible(bool visible) { m_visible = visible; }
	bool isVisible() const { return m_visible; }
	void toggleVisible() { m_visible = !m_visible; }

	void update(float dtime);
	void render();

	void onResize(int width, int height);

private:
	AracdiaRmlUiManager() = default;

	bool m_initialized = false;
	bool m_visible = true;
	int m_width = 0;
	int m_height = 0;
};

#endif // ENABLE_ARACDIA_RMLUI
