import os
import platform
import glob
from pathlib import Path

# Find the latest VEX Code extension directory dynamically
home_dir = str(Path.cwd())
vexcode_pattern = os.path.join(home_dir, ".vscode/extensions/vexrobotics.vexcode-*")
vexcode_dirs = sorted(glob.glob(vexcode_pattern), key=lambda x: [int(i) for i in os.path.basename(x).split('-')[1].split('.')], reverse=True)
vexcom_path = os.path.join(vexcode_dirs[0], "resources/tools/vexcom") if vexcode_dirs else ""

# Dynamically set VEXCOM_PATH based on OS and architecture
system = platform.system()
if system == "Darwin":
    # macOS
    vexcom_path = os.path.join(home_dir, "tools/vexcom/osx")
elif system == "Linux":
    # Linux, check architecture
    arch = platform.machine()
    if arch == "x86_64":
        vexcom_path = os.path.join(home_dir, "tools/vexcom/linux-x64")
    elif arch.startswith("arm"):
        vexcom_path = os.path.join(home_dir, "tools/vexcom/linux-arm32")
    elif arch == "aarch64":
        vexcom_path = os.path.join(home_dir, "tools/vexcom/linux-arm64")
else:
    # Fallback for unsupported systems
    print("Warning: Unsupported OS for vexcom setup")
    vexcom_path = ""

def get_vexcom_path():
    return vexcom_path


def get_color():
    vexcom_path = get_vexcom_path()
    # Run the vexcom command with --json flag
    try:
        vexcom_executable = os.path.join(vexcom_path, "vexcom")
        if not os.path.isfile(vexcom_executable):
            print(f"Error: vexcom executable not found at {vexcom_executable}")
            exit(1)
    
        # Execute the command and capture output
        result = subprocess.run([vexcom_executable, "--json"], capture_output=True, text=True, check=True)
    
        # Parse the JSON output
        try:
            json_output = json.loads(result.stdout)
            #print(json.dumps(json_output, indent=2))  # Pretty-print the JSON
        except json.JSONDecodeError as e:
            print(f"Error: Failed to parse JSON output: {e}")
            print(f"Raw output: {result.stdout}")
    
    except subprocess.CalledProcessError as e:
        print(f"Error: Failed to run vexcom: {e}")
        print(f"Error output: {e.stderr}")
        exit(-1)
    except Exception as e:
        print(f"Unexpected error: {e}")

    return json_output["v5"]["brain"]["name"]


import subprocess
import json


if __name__ == "__main__":
    vexcom_path = get_vexcom_path()
    # Run the vexcom command with --json flag
    try:
        vexcom_executable = os.path.join(vexcom_path, "vexcom")
        if not os.path.isfile(vexcom_executable):
            print(f"Error: vexcom executable not found at {vexcom_executable}")
            exit(1)

        # Execute the command and capture output
        result = subprocess.run([vexcom_executable, "--json"], capture_output=True, text=True, check=True)

        # Parse the JSON output
        try:
            json_output = json.loads(result.stdout)
            print(json.dumps(json_output, indent=2))  # Pretty-print the JSON
        except json.JSONDecodeError as e:
            print(f"Error: Failed to parse JSON output: {e}")
            print(f"Raw output: {result.stdout}")

    except subprocess.CalledProcessError as e:
        print(f"Error: Failed to run vexcom: {e}")
        print(f"Error output: {e.stderr}")
    except Exception as e:
        print(f"Unexpected error: {e}")