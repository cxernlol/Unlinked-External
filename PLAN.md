# Unlinked External Feature Expansion Plan
This plan outlines the addition of several high-impact features to the Unlinked External project to enhance its functionality and user experience.

# User Review Required
Please review the proposed features and let me know which ones you want to prioritize or if you want to modify the scope of any specific feature.

# Open Questions (Resolved)
- **Priority**: Execution will begin with Phase 1 and Phase 2.

# Proposed Changes

## UI Layout & Tab Integration (`src/ui/tabs/`)
To maintain compatibility with the existing ImGui framework, all new features will be routed to their respective UI tabs:
- **`tab_aimbot.hpp`**: Triggerbot (F1), Aimbot Humanization (F4), Recoil Control (F8), Auto-Wall (F9).
- **`tab_visual.hpp`**: Memory Chams (F3), Skeleton ESP (F5), Custom Hit Sounds (F11).
- **`tab_exploits.hpp`**: Advanced Rage/Speedhack/Fly (F2), Backtrack (F7), Fake Lag (F10), Anti-Aim (F12), AirStuck (F14), Event Flooder (F18), Chat Spammer (F19), Item Magnet (F20), No-Clip (F21).
- **`tab_settings.hpp`**: FOV Changer (F6), Freecam (F13), World Modulation (F15), Silent Walk (F16), Name Stealer (F17), Customizable Keybinds (F22).

## Feature 1: Triggerbot
Goal: Automatically fire when an enemy is in the crosshair.
Implementation: Add a new triggerbot.hpp module. Read the local player's look vector and check for intersection with enemy hitboxes (using existing offset/bone logic). Add configurable pre-fire delay and burst duration settings to the UI.

## Feature 2: Advanced Rage Features
Goal: Expand the rage tab with more aggressive exploits.
Implementation:
Update world.hpp / gameplay.hpp.
Click-to-Teleport: Implement a raycast from the camera to the 3D world on Ctrl+Click and overwrite the local HumanoidRootPart CFrame.
Speedhack & Fly: Add memory writes to WalkSpeed and manipulate velocity/gravity vectors.
Hitbox Expander: Overwrite the size properties of enemy parts to make targeting easier.

## Feature 3: Memory-based Chams (Material/Color Override)
Goal: Force enemies to be visible through walls by manipulating their rendering materials and colors directly in memory.
Implementation: Iterate through enemy character parts and overwrite their `Material` property (e.g., forcing it to ForceField or Neon) and `Color3` properties. This requires caching original values to restore them when toggled off.

## Feature 4: Aimbot Humanization (Smoothing & Bezier Curves)
Goal: Make aimbot movements appear entirely natural to spectators and recording software, masking the robotic snap.
Implementation: Calculate a curved path with slight, randomized overshoots instead of a linear trajectory. Send these calculated micro-movements to the game externally.

## Feature 5: Skeleton ESP
Goal: Render a highly accurate, zero-latency 2D skeletal overlay over enemy models to perfectly track posture, movement, and orientation through walls.
Implementation: Continuously use RPM on the 3D bone matrix arrays (Head, Neck, Spine, Pelvis, Shoulders, Elbows, Hands, Knees, Feet) for all valid enemy entities. Apply a World-to-Screen (W2S) transformation using the cached `ViewMatrix`. Draw smooth, antialiased connecting lines via ImGui's draw list (`AddLine`), implementing dynamic scaling and depth sorting so skeletons in the distance don't clutter the foreground.

## Feature 6: FOV Changer (Field of View)
Goal: Allow the user to widen their field of view beyond the game's normal constraints.
Implementation: Locate the camera structure in memory and use WPM to overwrite the FOV float value.

## Feature 7: Backtrack (Time Manipulation)
Goal: Exploit server-side lag compensation by caching historical tick data, allowing the local client to register hits on enemies where they were up to 400ms in the past.
Implementation: Establish a highly optimized circular buffer (e.g., `std::deque` or custom ring buffer) to continuously cache the `CFrame` and bone matrix of every enemy entity per tick. Intercept the local player's user command (CUserCmd) generation. When the aimbot/triggerbot engages, identify the optimal historical record based on current ping and server tickrate, then forcefully overwrite the `tick_count` in the outgoing packet to force the server to evaluate the shot against the past position.

## Feature 8: Standalone Recoil Control System (RCS)
Goal: Programmatically negate all weapon recoil and visual kickback to achieve pixel-perfect spray control without requiring an active aimbot target.
Implementation: Continuously RPM the local player's `punchAngle` or equivalent recoil property vector from the active weapon/camera entity. Calculate the delta between the current frame's recoil and the previous frame's recoil. Apply the inverted delta directly to the `ViewAngles` (via WPM) or synthesize counter-movements via `mouse_event` / `SendInput`, applying an adjustable smoothing factor (cubic bezier interpolation) to maintain a legitimate, human-like appearance to server-side anti-cheat heuristics.

## Feature 9: Auto-Wall (Penetration Check)
Goal: Ensure the aimbot only snaps to enemies (or the triggerbot only fires) if the bullet can actually penetrate the intervening walls.
Implementation: Implement a custom raycaster in the external, mirroring the game's raycast logic. Read map geometry bounds or material properties to determine if the line of sight is clear or if the material is thin enough to shoot through.

## Feature 10: Fake Lag (Packet Choking)
Goal: Manipulate network state to make the local player appear to teleport or jitter on the enemy's screen, making them incredibly hard to hit.
Implementation: Intercept or freeze the network threads/socket calls periodically (e.g., suspend the thread sending position updates for a few ticks, then resume) to choke outgoing packets while continuing to receive incoming ones.

## Feature 11: Custom Hit/Kill Sounds & UI Impact
Goal: Provide satisfying auditory and visual feedback when landing shots, regardless of game support.
Implementation: Monitor enemy health values in the cached player list. When a drop in health is detected and the local player recently fired, play a custom `.wav` file through the external UI and draw a hitmarker (e.g., an 'X') at the crosshair or 3D world hit location.

## Feature 12: Anti-Aim (Spinbot / Jitter)
Goal: Manipulate the player's server-sided rotation to make the head hitbox impossible to hit, while keeping the local client's camera completely normal.
Implementation: Continuously overwrite the server-sent yaw/pitch values of the local player's character with randomized or rapidly spinning values, de-syncing the visible character model from the actual hitbox.

## Feature 13: Freecam / Detached Camera
Goal: Allow the user to detach their camera from their character and explore the map freely to gather intel without exposing themselves.
Implementation: Overwrite the camera's `CameraSubject` and `CameraType` properties in memory to scriptable, then intercept mouse/keyboard inputs to translate the camera's CFrame independently of the HumanoidRootPart.

## Feature 14: AirStuck (Velocity Freezing)
Goal: Allow the player to freeze themselves mid-air to throw off enemy prediction, dodge shots, or access out-of-bounds areas.
Implementation: Bind a hotkey that, when held, continually overwrites the player's X, Y, and Z velocity vectors to exactly 0, and potentially anchors the HumanoidRootPart in memory to suspend them instantly.

## Feature 15: World Modulation (Fullbright & Night Mode)
Goal: Remove shadows and manipulate lighting to make enemies stand out perfectly against the environment, regardless of the map's native lighting.
Implementation: Read the `Lighting` service properties from memory (e.g., `Ambient`, `OutdoorAmbient`, `Brightness`, `ClockTime`) and overwrite them to custom values. Force `GlobalShadows` to false.

## Feature 16: Silent Walk / Footstep Muter
Goal: Mask the player's approach by preventing the server from broadcasting footstep sounds, perfect for flanking.
Implementation: Hook or freeze the specific memory addresses responsible for updating the player's movement state/animation speed that triggers the sound, or locate the sound objects within the character model and force their volume to 0 or remove them entirely.

## Feature 17: Name Stealer / Spoofing
Goal: Cause confusion in the lobby and avoid targeted reports by stealing the identity of another player.
Implementation: Read the player list from memory, select a random or targeted username, and overwrite the local player's `DisplayName` or `Name` properties in the relevant UI/character structures.

## Feature 18: Event Flooder (Lag Switch / Crasher)
Goal: Disrupt the game for other players by severely lagging the server or crashing the instance entirely.
Implementation: Identify a vulnerable remote event or network channel in memory. Rapidly invoke or send malformed packets to this channel to overwhelm the server's processing queue.

## Feature 19: Chat Spammer / Filter Bypass
Goal: Dominate the in-game chat with custom messages or bypass standard chat filters.
Implementation: Directly write string payloads into the chat UI's memory buffer or hook the function responsible for sending chat messages to the server, bypassing client-side cooldowns or sanitization.

## Feature 20: Item Magnet / Auto-Loot
Goal: Instantly acquire all valuable items or weapons without having to physically travel to them.
Implementation: Iterate through the workspace to find dropped items or loot containers. Use WPM to overwrite their positional CFrame data, teleporting them directly to the local player's feet.

## Feature 21: No-Clip (Collision Bypass)
Goal: Completely bypass client-side collision detection, allowing the player to seamlessly glide through solid map geometry, walls, and sealed doors.
Implementation: Continuously RPM the local player's character entity to find the `HumanoidRootPart` and individual limb pointers. Use WPM in a high-frequency loop to forcefully toggle the `CanCollide` boolean flags to `false` (typically a 1-byte write at the respective property offset). Alternatively, for more stable execution, locate and overwrite the memory address of the client's global collision mask or hook the physics engine's sweep function to permanently return a pass for the local player entity.

## Feature 22: Customizable Keybind System
Goal: Allow users to dynamically bind any feature (aimbot, triggerbot, panic key, etc.) to any keyboard or mouse button, saving preferences persistently.
Implementation: Implement an ImGui `Hotkey()` widget globally across all tabs next to toggleable features. Update `store.hpp` to serialize virtual key codes (`VK_*`) into the config format so user preferences are restored upon reinjection.

## Feature 23: ImGui Rework
Goal: Overhaul the external UI to achieve a highly minimalist, ultra-clean aesthetic that stays out of the user's way.
Implementation: Strip out unnecessary styling bloat. Focus entirely on typography, precise spacing, and muted, sophisticated color palettes. Remove heavy borders and glowing effects in favor of flat, pixel-perfect geometry and subdued contrast, ensuring the menu looks premium but entirely utilitarian. Re-organize `src/ui/tabs/` into a streamlined layout with zero visual clutter.

# Verification Plan (Phased Execution)
Validation will occur in distinct phases to isolate dependencies and minimize debugging overhead. Execution will begin with Phase 1 and Phase 2.

## Phase 1: Basic Memory Writing
*Goal: Ensure non-destructive WPM operations stick.*
- **Feature 3 & 6 (Chams & FOV)**: Overwrite `Material`, `Color3`, and FOV floats. Ensure original values cache correctly.
- **Feature 15 (World Modulation)**: Modify client-sided `Lighting` service properties.

## Phase 2: Input & Aim Mechanics
*Goal: Validate visual aim logic before applying stealth.*
- **Feature 1 & 9 (Triggerbot & Auto-Wall)**: Validate custom raycaster against materials, then ensure triggerbot respects walls.
- **Feature 4 & 8 (Humanization & RCS)**: Log bezier calculations; graph micro-movements for natural appearance before sending inputs.

## Phase 3: Advanced & Volatile Exploits (Rage)
*Goal: Isolate high-risk features that manipulate physics or network state.*
- **Feature 2 & 21 (Speedhack/Fly & No-Clip)**: Verify manipulating `CanCollide` and velocity doesn't cause infinite void falls.
- **Feature 7 & 10 (Backtrack & Fake Lag)**: Verify network thread freezing chokes packets without disconnects. Ensure circular buffer holds exactly ~200ms.
- **Feature 18 (Event Flooder)**: Private server testing only to avoid immediate lobby crashes.

## Automated & Manual Tests
- Add unit tests in `tests/` for new math functions (e.g., W2S projection, Base64 serialization).
- Run `build.bat --test` to ensure no existing tests break.
- Compile with `build.bat` and manually test in a live game environment following the phased priority above.