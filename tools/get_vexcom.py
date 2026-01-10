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
if not matches:
    raise RuntimeError("No VEXcode extension folder found in ~/.vscode/extensions")

latest = max(matches, key=os.path.getmtime)
vexcom_folder_path = os.path.join(latest, "resources", "tools", "vexcom")



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
    vexcom_path = os.path.join(vexcom_folder_path, "win32", "vexcom.exe")
else:
    # Fallback for unsupported systems
    print("Warning: Unsupported OS for vexcom setup")
    vexcom_path = ""


def get_vexcom_path():
    return vexcom_path

def _windows_probe_ports(vexcom_executable: str):
    # Probe COM1..COM10 (adjust if needed)
    for n in range(5, 10):
        port = f"COM{n}"
        result = subprocess.run(
            [vexcom_executable, "--json", port],
            capture_output=True,
            text=True,
            check=False,
            timeout=3
        )
        out = (result.stdout or "").strip()
        if out.startswith("{"):
            try:
                return json.loads(out), port
            except json.JSONDecodeError:
                pass
    return None, None

def get_color():
    vexcom_executable = get_vexcom_path()
    if not os.path.isfile(vexcom_executable):
        print(f"Error: vexcom executable not found at {vexcom_executable}")
        return None

    if platform.system() == "Windows":
        json_output, port = _windows_probe_ports(vexcom_executable)
        if json_output:
            print(f"Detected brain on {port}")
            return json_output.get("v5", {}).get("brain", {}).get("name")

        print("\n!!! vexcom couldn't find a brain on any COM port (COM5-10) !!!\n")
        return None

    # non-Windows: keep your original behavior
    result = subprocess.run([vexcom_executable, "--json"], capture_output=True, text=True, check=False, timeout=5)
    stdout = (result.stdout or "").strip()
    if stdout.startswith("{"):
        json_output = json.loads(stdout)
        return json_output.get("v5", {}).get("brain", {}).get("name")
    return None



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