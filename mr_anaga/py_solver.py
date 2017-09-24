import sys

resolve = set()
reject = set()

data = [''.join(sorted(line[:-1])) for line in sys.stdin.readlines()[1:]]
count = 0

for line in data:
    if line in resolve:
        resolve.remove(line)
        reject.add(line)
        count -= 1
    elif line not in reject:
        resolve.add(line)
        count += 1

print count
