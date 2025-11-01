# 2025-2026: Push Back

This repository contains code from CPSLO for the 2025-2026 VexU season.

To use: install the Vex extension for VSCode (also searchable in the VSCode extensions in the sidebar): [VEX VSCode Extension](https://www.vexrobotics.com/vexcode/vscode-extension?srsltid=AfmBOopWRqiDJHq3BagY0oy3-fJPv0uJFSfLEpMd36oV9kMFW66pCS7N)

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

You're good!
