// Luanti / Aracdia
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <RmlUi/Core/SystemInterface.h>

/// Minimal RmlUi system interface wired to Luanti logging and timers.
class AracdiaRmlSystemInterface : public Rml::SystemInterface {
public:
	double GetElapsedTime() override;
	bool LogMessage(Rml::Log::Type type, const Rml::String &message) override;
};
