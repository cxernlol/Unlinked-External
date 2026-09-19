# Unlinked External

[![CI](https://github.com/cxernlol/Unlinked-External/actions/workflows/ci.yml/badge.svg)](https://github.com/cxernlol/Unlinked-External/actions/workflows/ci.yml)
[![Tests](https://img.shields.io/badge/tests-222%20passed-brightgreen.svg)](tests/)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)

## What it is

Unlinked is a transparent, click-through overlay. The menu sits in the center of the screen. Clicks on the panel stay with Unlinked. Clicks outside go through to the game.

The overlay is a fully standalone executable featuring a natively resizable UI. Insert shows or hides the menu (the bind is remappable). Escape never quits Unlinked Ã¢â‚¬â€ it only closes a listen, a dropdown, the explorer, or a draft.

Configs and the offset cache live in `%AppData%\Unlinked`.

---

## Offsets

Unlinked does not ship a frozen offset list. It reads your running Roblox build from the process path (`version-Ã¢â‚¬Â¦`) and compares it to the dump on [`offsets.imtheo.lol`](https://offsets.imtheo.lol).

On startup it:

1. Asks `offsets.imtheo.lol` for the current **LIVE** Roblox version
2. Downloads `offsets.json` when that version changed
3. Applies the table and writes it to `%AppData%\Unlinked`
4. Falls back to the last good cache if you are offline

The dump host only publishes the LIVE channel. Roblox can put you on another channel. If your client hash and the LIVE hash differ, Unlinked opens a popup and ESP / aim stay empty until you switch.

### Wrong channel

Follow [Switching to LIVE](https://offsets.imtheo.lol/docs/live-channel):

1. Download [Fishstrap](https://www.fishstrap.app/Fishstrap.exe)
2. Install it, then open **Fishstrap** from Windows search
3. Click **Configure Settings**
4. Open the **Deployment** tab
5. Set **Channel** to `production` and press Enter
6. Set **Automatic channel change action** to **Never change**
7. Press **Save and Launch**

Launch Roblox through Fishstrap after that. Settings still shows the dump hash and a **Refresh** control. Do not edit offsets by hand.

---

## Features

### Aimbot (Targeting Utilities)

| Control | What it does |
| --- | --- |
| Enabled | Hold-to-aim with a remappable key (default right mouse) |
| Triggerbot | Automatically fire when a target enters the crosshair |
| Humanization | Bezier curve pathing and smooth micro-movements for natural tracking |
| Auto-Wall | Penetration checks to ensure line-of-sight before firing |
| Recoil Control | Smoothly counteracts weapon kick (Standalone RCS available) |
| Prediction | Dynamic ping-based lead for moving targets |
| Target | Head, neck, chest, stomach, body, legs |
| Extras | Team check, Visible only, Sticky aim, FOV customization |

### Silent Aim (Advanced Targeting)

Same targeting rules, on its own key (default left mouse).

| Control | What it does |
| --- | --- |
| Chance | `0` never activates. `100` always activates. Values in between roll once per press |
| Backtrack | Time manipulation to rewind target positions based on tick history |
| Hardware Input | Route inputs via hardware/driver level for maximum stealth |
| Target | The bone you pick is the bone it uses Ã¢â‚¬â€ Head stays on the head |
| Draw FOV | Circle on the overlay |

### Visual (Enhancements)

| Overlay | Visuals | Extras |
| --- | --- | --- |
| Box / Name / Health | Skeleton / Distance | 2D Radar Overlay |
| Out-of-FOV Arrows | Directional indicators for off-screen targets | Spectator Warning System |
| Memory Chams | Direct material and color overrides in memory | Custom Hit/Kill Sounds |

### Exploits (Movement & Utility)

| Category | Features |
| --- | --- |
| Movement | Speedhack, Fly, No-Clip (Collision Bypass), AirStuck (Velocity Freezing) |
| World | World Modulation (Fullbright/Night Mode), Freecam (Detached Camera) |
| Disruption | Fake Lag (Packet Choking), Anti-Aim (Jitter/Spin), Silent Walk |
| Utility | Click-to-Teleport, Hitbox Expander, Item Magnet, Name Stealer |
| Experimental | Event Flooder, Chat Spammer |

### Explorer

A live DataModel tree next to the menu.

- Search by name or class
- Copy path, goto, destroy, mark / unmark
- Name, class, and parent for the selected instance

### Settings & Security

| Group | Options |
| --- | --- |
| Security | Anti-Reverse Engineering, OBS Bypass (Stream-Proof Overlay) |
| Configs | Base64 Import/Export string sharing, Customizable Global Keybinds |
| Misc | FPS cap, VSync, menu key, FOV Changer |
| Game | Anti-AFK, uncapped FPS, explorer, offset version + refresh |
| Theme | Colors, shader, particles |
| Overlay | Watermark, FPS, menu opacity, ESP range |

Configs save and load from the Configs tab. Each file is a plain text preset under `%AppData%\Unlinked\configs`.

---

## Controls

| Key | Action |
| --- | --- |
| Insert | Show or hide the menu (default, remappable) |
| Escape | Close listen / dropdown / explorer / draft only |
| Silent key | Hold to apply silent aim (default Mouse 1) |
| Aim key | Hold to apply aimbot (default Mouse 2) |

*Note: Typing in text fields (like the Explorer search or config names) automatically suspends cheat hotkeys to prevent accidental activation.*

---

## Use

1. Grab the [compiled release](https://github.com/cxernlol/Unlinked-External/releases/latest)
2. Run `Unlinked.exe`

The executable is 100% standalone. No `assets/` folder is required. Offsets still sync on first launch.

## Build

The source zip has everything needed to compile. Double-click `build.bat` or run from terminal.

Windows 10 or 11, x64. Visual Studio 2022 or newer with **Desktop development with C++** (CMake is in that workload). No other repos, no extra packages.

```cmd
build.bat             # Build Release executable (default)
build.bat --debug     # Build Debug executable
build.bat --test      # Build and run CTest unit test suite
build.bat --debug --test
```

Output: `build\windows-release\Unlinked.exe` (or `build\windows-debug\Unlinked.exe`). The UI framework lives in `third_party/custom-framework`. Fonts live in `third_party/fonts`.

## Testing

Unlinked includes a zero-dependency C++20 unit test suite verifying core math, config serialization/parsing, and dynamic offset handling.

Run tests using `build.bat`:
```cmd
build.bat --test
```

Or directly via CTest using CMake presets:
```cmd
ctest --preset windows-release --output-on-failure
ctest --preset windows-debug --output-on-failure
```

---

## Layout

```
build.bat           one-click compile and test runner
assets/             source assets (fonts, icons, logo) compiled directly into the executable
src/                overlay, aim, ESP, configs
tests/              zero-dependency unit test suite and runner
third_party/custom-framework   bundled UI library
third_party/fonts              Inter + JetBrains Mono + Outfit + Montserrat
media/              README preview, menu clip, in-game clip
```

---

## Requirements

- Windows 10 or 11, 64-bit
- Roblox (`RobloxPlayerBeta.exe`)
- Network on first launch so offsets can sync (after that, the cache is enough)
- Visual Studio only if you run `build.bat` Ã¢â‚¬â€ not required to run the release exe

**Unlinked**.
