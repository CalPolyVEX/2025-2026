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

def get_threewire_pin(port:int, pin:str):
    return f"threeWirePort{port}.{str(pin).upper()}"

#print(data)

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
            print(motor)

            continue
            pass
        except Exception:
            pass
        
        
        for motor, motor_data in data["motorGroups"][key].items():
            s += f"vex::motor {motor} = vex::motor(PORT{motor_data["port"]}, {str(motor_data["rev"]).lower()});\n"
            #print(motor, motor_data)
            #s += f"vex::motor {motor} = vex::motor(PORT{motor_data["port"]}, {motor_data["rev"]});\n"

        
        s += f"vex::motorgroup {key} = vex::motorgroup({', '.join(val)});\n"
        s += '\n'

    print(s)
    return s
        
    
# Writing to a TOML file
data["new_key"] = "new_value"

s = translate(data)

change_name(f"CPSLO - {data["color"].upper()}")


if __name__ == "__main__":
    pass