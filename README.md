# EXP Boost (Kyubu port)

> This is a **port to Kyubu** of the original mod [`ChrisMiuchiz/Cube-World-EXP-Buff`](https://github.com/ChrisMiuchiz/Cube-World-EXP-Buff).
> Original author: **ChrisMiuchiz**. This port is not an official release by the author.

**What it does:** Multiplies the XP you get for kills. The multiplier is a setting.

**Note:** This is not an exact copy of the original's formula: the original changed the level curve, this mod multiplies the reward.

**Original:** <https://github.com/ChrisMiuchiz/Cube-World-EXP-Buff>

**Side:** client, server (`sides` in `mod.toml`).

## Installation

Requirements: **Cube World Alpha** (the 2013-07-20 `Cube.exe`, SHA-256 `84a7a132...30bf`, unpatched) and **Kyubu** (the mod loader, <https://github.com/kyubumods/kyubu>; this mod needs `kyubu = "^1.1"`, see `mod.toml`).

1. Install Kyubu with its own installer (`install\install.cmd`). The loader is not part of this mod.
2. Put **`exp_boost.zip`** (do not unpack it) into the **`Mods\`** folder next to `Cube.exe`. If `exp_boost.zip` is already there, replace it.
3. Start the game. `kyubu\logs\latest.log` should have a line saying `exp_boost` loaded.
4. To turn the mod off or remove it, take `exp_boost.zip` out of `Mods\` (or switch it off in the mod manager: **Mods** on the title screen).

The old Cube World Mod Launcher is **not needed** for this port and is not compatible with it: do not put the original mod's DLL next to it.

## Settings

File `kyubu\config\exp_boost.toml` (Kyubu creates it on first run) or the mod manager. Settings marked *restart* take effect on the next start of the game.

| Setting | Default | Range | What it does |
|---|---|---|---|
| `exp_multiplier` | `3.0` | 0.0 .. 1000.0 | Multiplier for the XP gained from kills (1.0 = vanilla, 2.0 = +100%, 5.0 = five times as much). |


Mod id: `exp_boost`. Source: `Mods\exp_boost\src\main.cpp`.
