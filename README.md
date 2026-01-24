# 2025-2026: Push Back

This repository contains code from CPSLO for the 2025-2026 VexU season.

## Some of the setup below may be outdated temporarily

## VEX SDK Setup

To use: install the Vex extension for VSCode (also searchable in the VSCode extensions in the sidebar): [VEX VSCode Extension](https://www.vexrobotics.com/vexcode/vscode-extension?srsltid=AfmBOopWRqiDJHq3BagY0oy3-fJPv0uJFSfLEpMd36oV9kMFW66pCS7N)

## Robot Targeting Tools Setup

Also install: [uv](https://docs.astral.sh/uv)
and run:

```sh
uv venv
uv sync
```

Verify that the above worked by running:

```sh
uv run python tools/target.py
```

If the above command outputs:

```sh
-----CONFIGURING {GREEN/GOLD} ROBOT-----
```

You're good! Targeting tools are now set up and will auto-detect the robot configuration.

## Embedded Setup

Ensure that you have nix installed: [[https://nixos.org/download/|nix]]

To enter the nix development environment for the RP2350x board, run:

```sh
cd pico
nix-shell
```

And wait a moment as nix installs dependencies into its cache.

Once you enter the nix shell, you have access to picotool and our rust toolchain, but to make things as easy as possible we have two helper commands:

```sh
build <binary>
```

Builds the project associated with src/\<binary\>.rs (src/main.rs by default).

```sh
upload <binary>
```

Uploads the project associated with src/\<binary\>.rs after building (src/main.rs by default).
