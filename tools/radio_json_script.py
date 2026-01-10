import json
import subprocess
from pathlib import Path

VEXCOM = r"C:/Users/VexAccount/.vscode/extensions/vexrobotics.vexcode-0.7.2025041600/resources/tools/vexcom/win32/vexcom.exe"

def get_vexcom_json(port: str) -> dict:
    # Run: vexcom.exe --json COM5
    proc = subprocess.run(
        [VEXCOM, "--json", port],
        capture_output=True,
        text=True,
        check=True,
    )
    return json.loads(proc.stdout)

data = get_vexcom_json("COM5")

connection = data["device"]["connection"]                 # "controller"
ctrl_ver   = data["v5"]["controller"]["version"]          # "0x0100004B"
radio_ver  = data["v5"]["controller"]["radio"]            # "0x01000030"
radio_bits = data["v5"]["controller"]["radio_bits"]       # "0x02"

print("connection:", connection)
print("controller fw:", ctrl_ver)
print("radio fw:", radio_ver)
print("radio bits:", radio_bits)