# Aracdia engine changes (LGPL)

Modifications to the Luanti fork for the Aracdia commercial game.

## Native UI — RmlUi (Phase C.0)

- **CMake**: `ENABLE_ARACDIA_RMLUI` (default ON) fetches [RmlUi 6.0](https://github.com/mikke89/RmlUi) via FetchContent.
- **Client overlay**: `src/client/aracdia/rmlui_manager.cpp` — GL3 renderer, spike document.
- **Render hook**: `src/client/render/plain.cpp` — draws after Irrlicht GUI.
- **Input**: F8 toggles overlay visibility.
- **Toggle off**: `cmake -DENABLE_ARACDIA_RMLUI=OFF ..`

## Pause menu (v0.2.0)

- `TOSERVER_PAUSE_MENU` / `TOCLIENT_SHOW_NATIVE_PAUSE_MENU` (protocol 53)
- `core.register_on_pause_menu` Lua hook

See monorepo `docs/ARACDIA_NATIVE_UI.md` for the full roadmap.
