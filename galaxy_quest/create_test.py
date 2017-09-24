import sys
import random

d = sys.argv[1]
k = sys.argv[2]
out = sys.argv[3] if len(sys.argv) > 3 else "output.txt"

#data = [[str(random.randint(0, 1e9)), str(random.randint(0, 1e9))] for _ in range(int(k))]
data = [['5', '5'] for _ in range(int(k))]
data = '\n'.join(' '.join(lis) for lis in [[d,k]] + data)

with open(out, 'w') as out:
    out.write(data)
