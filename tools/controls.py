import yaml

def get_controls(color):
    fp = f"cfg/{color}.toml"
    with open(fp) as f:
        c = yaml.safe_load(f.read())
    driver = c["driver"]
    dfp = f"cfg/{driver.lower()}.toml"
    with open(dfp) as f:
        data = yaml.safe_load(f.read())
    
