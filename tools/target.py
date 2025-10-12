#!/usr/bin/python3.10

from pathlib import Path

from settings_vex import change_name

import tomlkit

import argparse

p = Path("cfg/green.toml")

# Reading a TOML file
with open(p, "r") as f:
    data = tomlkit.parse(f.read())


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

change_name(f"CPSLO-{data["color"].upper()}")


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Generate VEX C++ code from TOML config.")
    parser.add_argument("output_file", type=str, help="Path to output file")
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
    