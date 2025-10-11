#!/usr/bin/python3.10

from pathlib import Path

from settings_vex import change_name

import tomlkit

p = Path("cfg/green.toml")

# Reading a TOML file
with open(p, "r") as f:
    data = tomlkit.parse(f.read())

# Accessing data
#print(data["key"])  # Access TOML keys like a dictionary

#print(data)

def translate(data: dict):
    s = ""
    for key, val in data["motors"].items():
        s += f"vex::motor {key} = vex::motor(PORT{val["port"]}, {val["rev"]});\n"

    #s += "\n"

    temp_list_ports = []

    for key, val in data["pneumatics"].items():
        port = val["port"]
        if (port not in temp_list_ports):
            s += f"\nvex::triport threeWirePort{port} = vex::triport(PORT{port});\n"
            temp_list_ports.append(port)
        s += f"vex::pneumatics {key} = vex::pneumatics(threeWirePort{port}.{val["pin"]});\n"

    for key, val in data["encoders"].items():
        port = val["port"]
        if (port not in temp_list_ports):
            s += f"\nvex::triport threeWirePort{port} = vex::triport(PORT{port});\n"
            temp_list_ports.append(port)
        s += f"vex::encoder {key} = vex::encoder(threeWirePort{port}.{val["pin"]});\n"

    s += '\n'
    for key, val in data["motorGroups"].items():
        s += f"vex::motorgroup {key} = vex::motorgroup({', '.join(val)});\n"

    print(s)
    return s
        
    
# Writing to a TOML file
data["new_key"] = "new_value"

s = translate(data)

change_name(f"CPSLO - {data["color"].upper()}")


if __name__ == "__main__":
    pass