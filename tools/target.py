
from pathlib import Path

from settings_vex import change_name

import tomlkit
import argparse
import os
from get_vexcom import get_vexcom_path, get_color

import subprocess
import json

color = "green"

#color = get_color()

p = Path(f"cfg/{color}.toml")
# Reading a TOML file
with open(p, "r") as f:
    data = tomlkit.parse(f.read())

name = f"FALL-2025-{data["color"].upper()}"

change_name(name)

print(f"\n-----CONFIGURING {color.upper()} ROBOT-----\n")


from translate_cfg import get_wiremap_string



if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Generate VEX C++ code from TOML config.")
    parser.add_argument("vex_sdk_filepath", type=str, help="Path to VEX SDK")
    args = parser.parse_args()

    device_file = "src/devices.cpp"

    s = get_wiremap_string(data)
 
    with open(device_file, "w") as out_f:
        out_f.write(s)
    
    with open(device_file, "r") as out_f:
        lines = out_f.readlines()

    result = ""
    for line in lines:
        if result == "":
            result = "#include \"vex.h\"\n\n"
            continue
        words = line.strip().split()
        if len(words) >= 2:
            result += f"extern {words[0]} {words[1]};\n"

    header_file = device_file.replace(".cpp", ".h").replace("src", "include")
    with open(header_file, "w") as header_f:
        header_f.write(result)
    
    color_header = "include/colors.h"
    with open(color_header, "w") as h:
        lines = f"""#define PRIMARY_COLOR {hex(data["hex_primary"])}
#define SECONDARY_COLOR {hex(data["hex_secondary"])}
#define TERTIARY_COLOR {hex(data["hex_tertiary"])}
#define BLUE_COLOR {hex(data["hex_blue"])}
#define RED_COLOR {hex(data["hex_red"])}
"""
        h.write(lines)