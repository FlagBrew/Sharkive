import os
import re

regex_v = r'\s*([\d.]+)'

for root, _, files in os.walk("./switch"):
    for file in files:
        with open(os.path.join(root, file), "r") as f:
            lines = f.readlines()
        version = ""
        with open(os.path.join(root, file), "w") as f:
            for line in lines:
                line = line.strip()
                if line.startswith('{'):
                    if not version:
                        try:
                            x = re.search(regex_v, line)
                            i = 0
                            while version.find('.') is -1:
                                version = x.group(i)
                                i += 1
                            if version is '.':
                                version = None
                        except Exception:
                            version = None
                if version and not version.startswith(' '):
                    version = ' ' + version
                elif line.startswith('[') and version and version not in line:
                    line = line[:line.rfind(']')] + version + ']'
                f.write(line + '\n')