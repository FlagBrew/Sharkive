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


def remove_duplicates(cheat):
    result = {}
    for key, value in cheat.items():
        if value not in result.values():
            result[key] = value
    return result   


print("Downloading Switch Releases XML...")
URL = "http://nswdb.com/xml.php"
RESPONSE = requests.get(URL)
if RESPONSE.status_code is not 200:
    exit()

ROOT = lxml.etree.fromstring(RESPONSE.content)
RESULTS = ROOT.findall("release")
TITLEIDS = set([r.find("titleid").text for r in RESULTS])
TITLEIDS_TO_REMOVE = []
TITLEIDS_TO_ADD = []

for titleid in TITLEIDS:
    if "+" in titleid:
        ids = titleid.split("+")
        TITLEIDS_TO_REMOVE.append(titleid)
        TITLEIDS_TO_ADD.extend(ids)

for titleid in TITLEIDS_TO_REMOVE:
    TITLEIDS.remove(titleid)

TITLEIDS.update(TITLEIDS_TO_ADD)

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

OKCOUNTER = 0
EXCEPTIONS = 0

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
            OKCOUNTER += 1
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
                # print(cheatcontent)
                fullcheat = cheatcontent.copy()
                fullcheat.update(cheat)
                fullcheat = remove_duplicates(fullcheat)
                dump_cheats(buildid_path, fullcheat)
            except Exception as e:
                print(e)
                EXCEPTIONS += 1
    else:
        print(" - Cheats not found on the API with titleid {}".format(titleid))
    time.sleep(1.0)

print("Found {}/{} cheats with {} exceptions".format(OKCOUNTER, COUNT, EXCEPTIONS))