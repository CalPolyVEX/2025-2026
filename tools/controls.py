import yaml


def get_map(d: str):
    with open(f"cfg/{d}.yaml") as f:
        m = yaml.load(f, yaml.Loader)
    return m

def get_controls(driver: str):
    s = """#include "vex.h"\n#include "controls.h"\n\nvoid bind_all(controller c){\n"""
    d = get_map(driver)
    for v in d:
        button = v
        s += f"    c.Button{button}.pressed({d[v]["onpress"]});\n"
        s += f"    c.Button{button}.released({d[v]["onrelease"]});\n"

    s += "}"
    return s


def write_controls(driver: str):
    string = get_controls(driver)
    with open("src/controls.cpp", "w") as f:
        _ = f.write(string)
        f.close()
    with open("include/controls.h", "w") as f:
        s = ""
        m = get_map(driver)
        for k in m:
            s += f"extern void {m[k]["onpress"]}();\n"
            s += f"extern void {m[k]["onrelease"]}();\n"
        s += "\nvoid bind_all(controller c);\n"
        _ = f.write(s)
    return string

if __name__ == "__main__":
    s = write_controls("joseph")
    print(s)