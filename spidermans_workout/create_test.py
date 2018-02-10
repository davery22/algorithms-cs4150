import random

N = 101
M = 40

out = str(N) + '\n'
for i in range(N):
    out += str(M) + '\n'
    out += ' '.join(str(random.randint(1,30)) for j in range(M)) + '\n'

with open('output.txt', 'w') as outfile:
    outfile.write(out)
