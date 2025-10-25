import tomlkit

def get_controls(color):
    fp = f"cfg/{color}.toml"
    with open(fp) as f:
        c = tomlkit.parse(f.read())
    driver = c["driver"]
    dfp = f"cfg/{driver.lower()}.toml"
    with open(dfp) as f:
        data = tomlkit.parse(f.read())
    
