#!/usr/bin/python3
import requests
import os
import argparse
import lxml.etree
import json

parser = argparse.ArgumentParser(description="Sharkive cheat docs creation tool")
parser.add_argument("type", help="3ds, switch")


def createDetails(type, name, cheats):
    details = ""
    if "3ds" in type:
        details = "\n<details>\n"
        details += "<summary>{}</summary>\n\n".format(name)
        for cheat in cheats:
            details += "- [{} ![{}]]({})\n".format(name, cheat["region"], cheat["path"])
        details += "\n</details>\n"
    elif "switch" in type:
        for cheat in cheats:
            details += "- [{} ![{}]]({})\n".format(name, cheat["region"], cheat["path"])
    return details


def saveMD(db, type):
    with open("./docs/{}.md".format(type), "w") as f:
        header = '## **<p align="center">'
        body = ""
        for key in db.keys():
            header += " " + '<a href="{}">{}</a>'.format(key, key)
            body += "\n## [{}]\n\n".format(key)
            for key2 in db[key].keys():
                body += createDetails(type, key2, db[key][key2])
        header += "</p>**\n"
        flags = """
<!-- Flags -->
[CHN]: http://nswdb.com/images/CHN.jpg "CHN"
[EUR]: http://nswdb.com/images/EUR.jpg "EUR"
[FRA]: http://nswdb.com/images/FRA.jpg "FRA"
[GER]: http://nswdb.com/images/GER.jpg "GER"
[WLD]: http://nswdb.com/images/WLD.jpg "GLO"
[ITA]: http://nswdb.com/images/ITA.jpg "ITA"
[JPN]: http://nswdb.com/images/JPN.jpg "JPN"
[KOR]: http://nswdb.com/images/KOR.jpg "KOR"
[NLD]: http://nswdb.com/images/NLD.jpg "NLD"
[SPA]: http://nswdb.com/images/SPA.jpg "SPA"
[TWN]: http://nswdb.com/images/TWN.jpg "TWN"
[UKV]: http://nswdb.com/images/UKV.jpg "UKV"
[USA]: http://nswdb.com/images/USA.jpg "USA"
        """
        f.write(header)
        f.write(body)
        f.write(flags)


def main(args):
    if "3ds" in args.type:
        url = "http://3dsdb.com/xml.php"
    elif "switch" in args.type:
        url = "http://nswdb.com/xml.php"
    else:
        exit(0)
    response = requests.get(url)

    xmlroot = lxml.etree.fromstring(response.content)

    cheatFiles = []
    db = {}
    if "3ds" in args.type:
        for root, _, files in os.walk("./db"):
            for file in files:
                cheatFiles.append(os.path.splitext(file)[0])
    elif "switch" in args.type:
        for root, _, files in os.walk("./switch"):
            titleid = root[root.rfind("/") + 1 :]
            if "switch" in titleid:
                continue
            cheatFiles.append(titleid)
    for elem in xmlroot.findall("release"):
        titleid = elem.find("titleid").text
        if titleid in cheatFiles:
            name = elem.find("name").text
            initialLetter = "#" if name[0].isdigit() else name[0].upper()
            if initialLetter not in db.keys():
                db[initialLetter] = {}
            cheat = {
                "region": elem.find("region").text,
                "path": "../blob/master/db/{}.txt".format(titleid)
                if "3ds" in args.type
                else "../tree/master/switch/{}".format(titleid),
            }
            if name not in db[initialLetter].keys():
                db[initialLetter][name] = []
            db[initialLetter][name].append(cheat)

    db = dict(sorted(db.items()))
    saveMD(db, args.type)


if __name__ == "__main__":
    main(parser.parse_args())
