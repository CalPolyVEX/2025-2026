
from pathlib import Path

from settings_vex import change_name

import tomlkit
import argparse
import os
from get_vexcom import get_vexcom_path

import subprocess
import json

color = "green"

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
except Exception as e:
    print(f"Unexpected error: {e}")

color = json_output["v5"]["brain"]["name"]

p = Path(f"cfg/{color}.toml")
# Reading a TOML file
with open(p, "r") as f:
    data = tomlkit.parse(f.read())

name = f"FALL-{data["color"].upper()}"


print(f"\n-----CONFIGURING {color.upper()} ROBOT-----\n")

import subprocess
import json




def get_threewire_pin(port:int, pin:str):
    return f"threeWirePort{port}.{str(pin).upper()}"

def get_threewire_dev(port:int):
    return f"\nvex::triport threeWirePort{port} = vex::triport(PORT{port});\n"


def translate(data: dict):
    s = ""

    temp_list_ports = []

    for key, val in data["pneumatics"].items():
        port = val["port"]
        if (port not in temp_list_ports):
            s += get_threewire_dev(port)
            temp_list_ports.append(port)
        s += f"vex::pneumatics {key} = vex::pneumatics({get_threewire_pin(port, val["pin"])});\n"

    for key, val in data["encoders"].items():
        port = val["port"]
        if (port not in temp_list_ports):
            s += get_threewire_dev(port)
            temp_list_ports.append(port)
        s += f"vex::encoder {key} = vex::encoder({get_threewire_pin(port, val["pin"])});\n"

    s += '\n'

    for key, val in data["motorGroups"].items():
        
        try :
            motor = data["motorGroups"][key]
            s += f"vex::motor {key} = vex::motor(PORT{motor["port"]}, {str(motor["rev"]).lower()});\n"
            #print(motor)

            continue
            pass
        except Exception:
            pass
        
        
        for motor, motor_data in data["motorGroups"][key].items():
            s += f"vex::motor {motor} = vex::motor(PORT{motor_data["port"]}, {str(motor_data["rev"]).lower()});\n"
            #print(motor, motor_data)
            #s += f"vex::motor {motor} = vex::motor(PORT{motor_data["port"]}, {motor_data["rev"]});\n"

        

        s += f"vex::motor_group {key} = vex::motor_group({', '.join(val)});\n"
    s += '\n'

    s += device_template("optical")
    s += '\n'
    s += device_template("distance")
    s += '\n'


    #print(s)
    return s
        
    
def device_template(thing):
    s = ""
    for k, v in data[f"{thing}s"].items():
        s += f"vex::{thing} {k} = vex::{thing}(PORT{v["port"]});\n"
    return s

# # Writing to a TOML file
# data["new_key"] = "new_value"

s = "#include \"vex.h\"\n"
s += "\n"

s += translate(data)

change_name(name)


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Generate VEX C++ code from TOML config.")
    parser.add_argument("output_file", type=str, help="Path to output file", default="src/devices.cpp")
    args = parser.parse_args()

    with open(args.output_file, "w") as out_f:
        out_f.write(s)
    
    with open(args.output_file, "r") as out_f:
        lines = out_f.readlines()

    result = ""
    for line in lines:
        if result == "":
            result = "#include \"vex.h\"\n\n"
            continue
        words = line.strip().split()
        if len(words) >= 2:
            result += f"extern {words[0]} {words[1]};\n"

    header_file = args.output_file.replace(".cpp", ".h").replace("src", "include")
    with open(header_file, "w") as header_f:
        header_f.write(result)
    

    with open("include/colors.h", "w") as color_h:
        lines = f"""#define PRIMARY_COLOR {hex(data["hex_primary"])}
#define SECONDARY_COLOR {hex(data["hex_secondary"])}
#define TERTIARY_COLOR {hex(data["hex_tertiary"])}
#define BLUE_COLOR {hex(data["hex_blue"])}
#define RED_COLOR {hex(data["hex_red"])}
"""
        color_h.write(lines)