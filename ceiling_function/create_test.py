import random
import sys

n = int(sys.argv[1])
k = int(sys.argv[2])
outfile = sys.argv[3] if len(sys.argv) > 3 else 'output.txt'

lis = [' '.join(str(random.randint(1,100)) for _ in range(k)) for _ in range(n)]
data = '\n'.join(['{0} {1}'.format(n,k)] + lis)

with open(outfile, 'w') as out:
    out.write(data)
