
from pathlib import Path

from settings_vex import change_name

import tomlkit
from get_vexcom import get_color
from translate_cfg import get_wiremap_string

from chassis_generate import get_JAR_Template_chassis_string

color = "green"

color = get_color() or "green"

p = Path(f"cfg/{color}.toml")
# Reading a TOML file
with open(p, "r") as f:
    data = tomlkit.parse(f.read())

name = f"FALL-2025-{data["color"].upper()}"

change_name(name)

print(f"\n-----CONFIGURING {color.upper()} ROBOT-----\n")




if __name__ == "__main__":

    

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




    chassis_data = data["chassis"]

    c = get_JAR_Template_chassis_string(
        left_group_string=chassis_data["drivetrain"]["left"],
        right_group_string=chassis_data["drivetrain"]["right"],
        inertial=chassis_data["inertial"]["port"],
        forward=chassis_data["encoders"]["forward"]["pin"],
        forward_dia=chassis_data["encoders"]["forward"]["diameter"],
        forward_ctr_dist=chassis_data["encoders"]["forward"]["center_dist"],
        sideways=chassis_data["encoders"]["sideways"]["pin"],
        sideways_dia=chassis_data["encoders"]["sideways"]["diameter"],
        sideways_ctr_dist=chassis_data["encoders"]["sideways"]["center_dist"],
        drive_gear_ratio = float(chassis_data["gear_input"]) / float(chassis_data["gear_output"]),
        drive_setup=chassis_data["drive_setup"],
        drive_dia=chassis_data["wheel_diameter"]

    )
    jar = """/*---------------------------------------------------------------------------*/
/*                             JAR-Template Config                           */
/*                                                                           */
/*  Where all the magic happens. Follow the instructions below to input      */
/*  all the physical constants and values for your robot. You should         */
/*  already have configured your motors.                                     */
/*---------------------------------------------------------------------------*/


"""
    chassis_cpp_fp = "src/chassis.cpp"
    with open(chassis_cpp_fp, "w") as f:
        f.write(f'#include "vex.h"\n#include "JAR-Template/drive.h"\n#include "devices.h"\n{jar}\n{c}')


    chassis_h_fp = "include/chassis.h"
    with open(chassis_h_fp, "w") as f:
        f.write('#include "vex.h"\n#include "JAR-Template/drive.h"\n#include "devices.h"\nextern Drive chassis;')