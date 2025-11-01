import json
import os

def change_name(name: str): # puts the name in the project JSON cfg file
    p = os.path.join(os.path.dirname(__file__), "..", ".vscode", "vex_project_settings.json")
    with open(p, 'r') as f:
        data = json.load(f)
    data["project"]["name"] = name
    with open(p, 'w') as f:
        json.dump(data, f, indent=4)

        
if __name__ == "__main__":
    change_name("CPSLO - GREEN")