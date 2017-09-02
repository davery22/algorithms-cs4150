import sys
import random
import string

n = int(sys.argv[1])
k = int(sys.argv[2])
outfile = sys.argv[3] if len(sys.argv) > 3 else 'output.txt'

lis = [''.join(random.choice(string.ascii_lowercase) for _ in range(k)) for _ in range(n)]

# Install duplicates
for i in random.sample(range(n), random.randint(0,n-1)):
    randel = lis[random.randint(0,n-1)]
    shuffled = ''.join(random.sample(randel, k))
    lis[i] = shuffled
data = '\n'.join(['{0} {1}'.format(n,k)] + lis)

with open(outfile, 'w') as out:
    out.write(data)

