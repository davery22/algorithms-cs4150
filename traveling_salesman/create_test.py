import sys
import random

n = int(sys.argv[1])

edges = [[random.randint(1,501) for i in range(n)] for j in range(n)]
out = str(n) + '\n' + '\n'.join(' '.join(str(x) for x in y) for y in edges) + '\n'

with open('output.in', 'w') as outfile:
    outfile.write(out)
'''
best = float('inf')
def solve(u, edges, visited, cur):
    global best
    if all(visited):
        cur += edges[u][0]
        if cur < best:
            best = cur
        return

    for v in [e for e in range(len(edges)) if not visited[e]]:
        v2 = visited[:]
        v2[v] = True
        solve(v, edges, v2, cur + edges[u][v])

visited = [False for x in range(n)]
visited[0] = True
solve(0, edges, visited, 0)

with open('output.ans', 'w') as outfile:
    outfile.write(str(best))
'''
