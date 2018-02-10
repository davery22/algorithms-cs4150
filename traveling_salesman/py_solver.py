import sys
import re

lines = [line.strip() for line in sys.stdin.readlines()]
n = int(lines[0])
lines = [re.split('\s+', l) for l in lines[1:]]
edges = [[int(y) for y in x[:n]] for x in lines]

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

print best

