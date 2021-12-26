#!/usr/bin/env python3
# lol
# just use this to order the huts, I can't be bothered to change the code
# to store everything then sort then write to the file in C++, and this
# is sort of safer anyways.
# to use: sort.py your_file
# pipe into a file with sort.py your_file > output
# also deduplicates, because why not

import sys, re
args = sys.argv[1:]
if '--clean' in args:
    clean = True
else:
    clean = False
args = [x for x in args if not x.startswith('-')]

whre = re.compile(r'CENTER for \d huts: -?(\d+), *-?(\d+)\s*$')
def whsort(l):
    mo = re.match(whre, l)
    if mo is None:
        return 0 # idk, who cares
    x = int(mo.group(1))
    y = int(mo.group(2))
    return x**2 + y**2

def cleaned(l):
    if not clean:
        return l
    return l.split(':')[1].lstrip()

with open(sys.argv[1], 'r') as file:
    print(''.join([cleaned(x) for x in sorted(set(file.readlines()), key=whsort)]))
