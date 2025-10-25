import tomlkit
def get_threewire_pin(port:int, pin:str): #threewire pin template
    return f"threeWirePort{port}.{str(pin).upper()}"

def get_threewire_dev(port:int): # threewire port template
    return f"\nvex::triport threeWirePort{port} = vex::triport(PORT{port});\n"


def translate(data: dict): # translates toml to the VEX API
    s = ""

    temp_list_ports = []
    try:
        for key, val in data["pneumatics"].items(): #parse pneumatics as pins
            port = val["port"]
            if (port not in temp_list_ports):
                s += get_threewire_dev(port)
                temp_list_ports.append(port)
            s += f"vex::pneumatics {key} = vex::pneumatics({get_threewire_pin(port, val["pin"])});\n"
    except tomlkit.exceptions.NonExistentKey:
        pass
    try:
        for key, val in data["encoders"].items(): # parse encoders as pins
            port = val["port"]
            if (port not in temp_list_ports):
                s += get_threewire_dev(port)
                temp_list_ports.append(port)
            s += f"vex::encoder {key} = vex::encoder({get_threewire_pin(port, val["pin"])});\n"
    except tomlkit.exceptions.NonExistentKey:
        pass
    s += '\n'

    for key, val in data["motorGroups"].items():
        
        try : #try to parse group as a single motor
            motor = data["motorGroups"][key]
            s += f"vex::motor {key} = vex::motor(PORT{motor["port"]}, {str(motor["rev"]).lower()});\n"
            #print(motor)

            continue
            pass
        except Exception: # no big deal if it fails, try parsing the group next
            #print(e)
            pass
        
        
        for motor, motor_data in data["motorGroups"][key].items(): #parse motor groups
            s += f"vex::motor {motor} = vex::motor(PORT{motor_data["port"]}, {str(motor_data["rev"]).lower()});\n"
            #print(motor, motor_data)
            #s += f"vex::motor {motor} = vex::motor(PORT{motor_data["port"]}, {motor_data["rev"]});\n"

        

        s += f"vex::motor_group {key} = vex::motor_group({', '.join(val)});\n"
    s += '\n'

    s += device_template(data, "optical")
    s += '\n'
    s += device_template(data, "distance")
    s += '\n'
    s += device_template(data, "inertial")
    s += '\n'

    #print(s)
    return s
        
    
def device_template(data, thing):  # generates a vex::namespace object
    try:
        s = ""
        for k, v in data[f"{thing}s"].items():
            s += f"vex::{thing} {k} = vex::{thing}(PORT{v["port"]});\n"
        return s
    except tomlkit.exceptions.NonExistentKey:
        return ""

# # Writing to a TOML file
# data["new_key"] = "new_value"

def get_wiremap_string(data): # creates the whole device file
    s = "#include \"vex.h\"\n"
    s += "\n"

    s += translate(data)
    return s