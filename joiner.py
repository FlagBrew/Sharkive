#!/usr/bin/python3
import argparse
import os
import json
import bz2

parser = argparse.ArgumentParser(description = 'Sharkive cheat codes joiner')
parser.add_argument('type', help = '3ds, switch')

def main(args):
    db = {}

    if '3ds' in args.type:
        for cheat in os.listdir('./3ds'):
            with open(os.path.join('./3ds', cheat), 'r', encoding="UTF-8") as file:
                titleid = cheat[:cheat.rfind('.')]
                lines = [line.strip() for line in file]
                lines = list(filter(None, lines))

                db[titleid] = {}
                selectedCheat = lines[0]
                for line in lines:
                    if line.startswith('{') and line.endswith('}'):
                        pass
                    elif line.startswith('[') and line.endswith(']'):
                        selectedCheat = line[1:-1]
                        db[titleid][selectedCheat] = []
                    else:
                        db[titleid][selectedCheat].append(line)
    elif 'switch' in args.type:
        for root, _, files in os.walk('./switch'):
            root = root.replace('\\', '/')
            titleid = root[root.rfind('/')+1:]

            if "switch" in titleid:
                continue

            db[titleid] = {}
            for file in files:
                buildid = file[:file.find('.')]
                db[titleid][buildid] = {}
                with open(os.path.join(root, file), 'r') as f:
                    lines = [line.strip() for line in f]
                    lines = list(filter(None, lines))
                    selectedCheat = lines[0]
                    for line in lines:
                        if line.startswith('{') and line.endswith('}'):
                            pass
                        elif line.startswith('[') and line.endswith(']'):
                            selectedCheat = line[1:-1]
                            db[titleid][buildid][selectedCheat] = []
                        else:
                            db[titleid][buildid][selectedCheat].append(line)
    else:
        exit(0)
    compressed = bz2.compress(str.encode(json.dumps(db)))
    with open(os.path.join('build', args.type + '.json'), 'w') as f:
        f.write(json.dumps(db))
    with open(os.path.join('build', args.type + '.json.bz2'), 'wb') as f:
        f.write(compressed)

if __name__ == '__main__':
    main(parser.parse_args())
