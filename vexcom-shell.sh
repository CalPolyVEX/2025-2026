#!/bin/bash

# Find the latest VEX Code extension directory dynamically
export VEXCOM_PATH=$(find $HOME/.vscode/extensions -maxdepth 1 -type d -name "vexrobotics.vexcode-*" | sort -V | tail -n 1)/resources/tools/vexcom

# Dynamically set VEXCOM_PATH based on OS and architecture
case "$(uname -s)" in
  Darwin)
    # macOS
    export VEXCOM_PATH="$HOME/.vscode/extensions/vexrobotics.vexcode-0.7.2025041600/resources/tools/vexcom/osx"
    ;;
  Linux)
    # Linux, check architecture
    case "$(uname -m)" in
      x86_64)
        export VEXCOM_PATH="$HOME/.vscode/extensions/vexrobotics.vexcode-0.7.2025041600/resources/tools/vexcom/linux-x64"
        ;;
      arm*)
        export VEXCOM_PATH="$HOME/.vscode/extensions/vexrobotics.vexcode-0.7.2025041600/resources/tools/vexcom/linux-arm32"
        ;;
      aarch64)
        export VEXCOM_PATH="$HOME/.vscode/extensions/vexrobotics.vexcode-0.7.2025041600/resources/tools/vexcom/linux-arm64"
        ;;
    esac
    ;;
  *)
    # Fallback or warning for unsupported systems (e.g., Windows)
    echo "Warning: Unsupported OS for vexcom setup"
    ;;
esac

# Add VEXCOM_PATH to PATH if it exists
[ -d "$VEXCOM_PATH" ] && export PATH="$VEXCOM_PATH:$PATH"

#export PATH="~/Library/Application\ Support/Code/User/globalStorage/vexrobotics.vexcode/sdk/cpp/V5/V5_20240802_15_00_00:$PATH"
export VEX_SDK_PATH="$HOME/Library/Application\ Support/Code/User/globalStorage/vexrobotics.vexcode/sdk/cpp/V5/V5_20240802_15_00_00"

# Ensure the terminal remains interactive
exec bash