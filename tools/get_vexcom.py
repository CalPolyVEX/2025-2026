import os
import platform
import glob
from pathlib import Path
import subprocess
import json

# Find the latest VEX Code extension directory dynamically
home_dir = str(Path.cwd())
home_dir = os.path.expanduser("~")
pattern = os.path.join(home_dir, ".vscode", "extensions", "vexrobotics.vexcode-*")
matches = glob.glob(pattern)
vexcom_folder_path = os.path.join(matches[0], "resources/tools/vexcom")



# Dynamically set VEXCOM_PATH based on OS and architecture

system = platform.system()
if system == "Darwin":
    # macOS
    vexcom_path = os.path.join(home_dir, f"{vexcom_folder_path}/osx/vexcom")
elif system == "Linux":
    # Linux, check architecture
    arch = platform.machine()
    if arch == "x86_64":
        vexcom_path = os.path.join(home_dir, f"{vexcom_folder_path}/linux-x64/vexcom")
    elif arch.startswith("arm"):
        vexcom_path = os.path.join(home_dir, f"{vexcom_folder_path}/linux-arm32/vexcom")
    elif arch == "aarch64":
        vexcom_path = os.path.join(home_dir, f"{vexcom_folder_path}/linux-arm64/vexcom")
elif system == "Windows":
    vexcom_path = os.path.join(home_dir, f"{vexcom_folder_path}/win32/vexcom.exe")
else:
    # Fallback for unsupported systems
    print("Warning: Unsupported OS for vexcom setup")
    vexcom_path = ""


def get_vexcom_path():
    return vexcom_path


def get_color():
    vexcom_executable = get_vexcom_path()
    # Run the vexcom command with --json flag
    try:
        
        
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
    
    except subprocess.CalledProcessError:
        print("\n!!! VEX Brain not detected. Building anyway. !!!\n")
        
        
    except Exception as e:
        print(f"Unexpected error: {e}")

    try:
        color = json_output["v5"]["brain"]["name"]
    except Exception:
        #print(f"invalid color, error msg: {e}")
        color = None
    return color





if __name__ == "__main__":
    vexcom_path = get_vexcom_path()
    # Run the vexcom command with --json flag
    try:
        vexcom_executable = vexcom_path
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