import json
import time
import os
from codecs import encode, decode
import lxml.etree
import requests


def parse_cheat(lines):
    mycheat = {}
    lines = list(filter(None, lines))
    selectedCheat = lines[0]
    for line in lines:
        line = line.strip()
        if line.startswith("{") and line.endswith("}"):
            pass
        elif line.startswith("[") and line.endswith("]"):
            selectedCheat = line[1:-1]
            mycheat[selectedCheat] = []
        else:
            mycheat[selectedCheat].append(line)
    return mycheat


def parse_cheat_file(file):
    with open(file, "r") as cheat_file:
        return parse_cheat([line.strip() for line in cheat_file])


def dump_cheats(buildid_path, cheats):
    with open(buildid_path, "w") as f:
        for key, value in cheats.items():
            if value:
                f.write("[{}]\n".format(key))
                for line in value:
                    f.write("{}\n".format(line))
                f.write("\n")


print("Downloading Switch Releases XML...")
URL = "http://nswdb.com/xml.php"
RESPONSE = requests.get(URL)
if RESPONSE.status_code is not 200:
    exit()

ROOT = lxml.etree.fromstring(RESPONSE.content)
RESULTS = ROOT.findall("release")
TITLEIDS = set([r.find("titleid").text for r in RESULTS])

print("Counting {} cheats in the list".format(len(TITLEIDS)))
with open("config.json") as f:
    config = json.loads(f.read())
TOKEN = config["token"]

URL = "https://www.switchcheatsdb.com/api/v1/cheats/count"
RESPONSE = requests.get(URL)
if RESPONSE.status_code is not 200:
    exit()
COUNT = json.loads(RESPONSE.content)["count"]
print("Counting {} cheats from the API".format(COUNT))

print("Starting to request cheats")
HEADERS = {"X-API-TOKEN": TOKEN}

for titleid in TITLEIDS:
    titleid_path = "./switch/{}".format(titleid)
    url = "https://www.switchcheatsdb.com/api/v1/cheats/" + titleid
    response = requests.get(url, headers=HEADERS)
    if response.status_code is 200:
        print(" + Cheats found on the API with titleid {}".format(titleid))
        apicheats = json.loads(response.content)["cheats"]
        if not os.path.isdir(titleid_path):
            os.mkdir(titleid_path)
        for apicheat in apicheats:
            try:
                buildid = apicheat["buildid"].lower()
                buildid_path = "{}/{}.txt".format(titleid_path, buildid)
                content = decode(
                    encode(apicheat["content"], "latin-1", "backslashreplace"),
                    "unicode-escape",
                )
                cheat = {}
                if os.path.isfile(buildid_path):
                    cheat = parse_cheat_file(buildid_path)
                cheatcontent = parse_cheat(content.splitlines())
                print(cheatcontent)
                fullcheat = cheatcontent.copy()
                fullcheat.update(cheat)
                dump_cheats(buildid_path, fullcheat)
            except Exception as e:
                print(e)
    else:
        print(" - Cheats not found on the API with titleid {}".format(titleid))
    time.sleep(1.5)
