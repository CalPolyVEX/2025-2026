#!/usr/bin/python3.10

from pathlib import Path

import tomlkit

p = Path("cfg/green.toml")

# Reading a TOML file
with open(p, "r") as f:
    data = tomlkit.parse(f.read())

# Accessing data
#print(data["key"])  # Access TOML keys like a dictionary

#print(data)

def translate_c(data: dict):
    s = ""
    for key, val in data["motors"].items():
        s += f"vex::motor {key} = vex::motor(PORT{val["port"]}, {val["rev"]});\n"

    s += "\n"

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

    print(s)
        

# Writing to a TOML file
data["new_key"] = "new_value"

translate_c(data)

with open(p, "w") as f:
    f.write(tomlkit.dumps(data))