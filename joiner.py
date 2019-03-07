#!/usr/bin/python3
import argparse
import os
import json

parser = argparse.ArgumentParser(description = 'Sharkive cheat codes joiner')
parser.add_argument('type', help = '3ds, switch')

def main(args):
    if '3ds' in args.type:
        cheats = os.listdir('./3ds')
    elif 'switch' in args.type:
        cheats = os.listdir('./switch')
    else:
        exit(0)

    db = {}
    for cheat in cheats:
        with open(os.path.join(args.type, cheat), 'r') as file:
            titleid = cheat[:cheat.rfind('.')]
            lines = [line.strip() for line in file]
            lines = list(filter(None, lines))
            
            db[titleid] = {}
            selectedCheat = lines[0]
            for line in lines:
                if line.startswith('[') and line.endswith(']'):
                    selectedCheat = line[1:-1]
                    db[titleid][selectedCheat] = []
                else:
                    db[titleid][selectedCheat].append(line)
    with open(os.path.join('build', args.type + '.json'), 'w') as f:
        f.write(json.dumps(db))

if __name__ == '__main__':
    main(parser.parse_args())