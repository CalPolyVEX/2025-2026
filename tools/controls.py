import re
from pathlib import Path

def change_driver(driver: str) -> None:
    """
    Dynamically patch the DRIVER_NAME macro in include/controls.h
    so that the identifier becomes driver.upper().
    The macro's value (everything after the identifier) is preserved.
    """
    path = Path("include/controls.h")
    if not path.is_file():
        raise FileNotFoundError(f"{path} not found")

    # Fixed regex: properly closed groups and safe whitespace handling
    define_pat = re.compile(
        r"""
        ^                       # Start of line
        (\s*)                   # Capture leading whitespace (group 1)
        \#                      # Literal '#'
        \s*                     # Optional whitespace
        define                  # 'define'
        \s+                     # Required whitespace
        (\w+)                   # Macro name to replace (group 2)
        (.*)                    # Rest of the line: the value (group 3)
        $                       # End of line
        """,
        re.VERBOSE
    )

    new_driver = driver.upper()
    content = path.read_text(encoding="utf-8")
    lines = content.splitlines(keepends=True)

    modified = False
    for i, line in enumerate(lines):
        m = define_pat.match(line)
        if m:
            leading_ws, old_id, value = m.groups()
            if old_id != new_driver:
                # Reconstruct line: leading_ws + '#define ' + NEW_NAME + value
                new_line = f"{leading_ws}#define {new_driver}{value}\n"
                lines[i] = new_line
                modified = True
            break  # Only replace the first match

    if modified:
        path.write_text("".join(lines), encoding="utf-8")


# ----------------------------------------------------------------------
# Example usage
# ----------------------------------------------------------------------
if __name__ == "__main__":
    change_driver("my_new_driver")