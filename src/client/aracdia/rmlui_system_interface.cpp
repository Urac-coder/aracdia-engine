// Luanti / Aracdia
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "client/aracdia/rmlui_system_interface.h"
#include "log.h"
#include "porting.h"

double AracdiaRmlSystemInterface::GetElapsedTime()
{
	return static_cast<double>(porting::getTimeUs()) / 1000000.0;
}

bool AracdiaRmlSystemInterface::LogMessage(Rml::Log::Type type,
		const Rml::String &message)
{
	switch (type) {
	case Rml::Log::LT_ERROR:
		errorstream << "[RmlUi] " << message << std::endl;
		break;
	case Rml::Log::LT_ASSERT:
	case Rml::Log::LT_WARNING:
		warningstream << "[RmlUi] " << message << std::endl;
		break;
	default:
		infostream << "[RmlUi] " << message << std::endl;
		break;
	}
	return true;
}
