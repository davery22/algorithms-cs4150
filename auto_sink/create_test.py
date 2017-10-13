import sys
import random
import uuid

class Node(object):
    def __init__(val):
        self.value = val
        self.children = []

class Graph(object):
    def __init__():
        self.root = None
        self.count = 0

    def traverse(node, pos, target):
        if pos == target:
            return node, pos

        for c in enumerate(node.children):
            _, pos = 

    def add_node(city):
        if not self.root:
            self.root = Node(city)
        else:
            
        self.count += 1

n = sys.argv[1]
h = sys.argv[2]
t = sys.argv[3]
out = sys.argv[4] if len(sys.argv) > 4 else "output.txt"

# Generate city names (unique) and tolls
cities = {}
for _ in range(int(n)):
    city = uuid.uuid4().hex[:15]
    while (city in cities):
        city = uuid.uuid4().hex[:15]
    cities[city] = random.randint(0, 10000)

# Generate highways between cities
highways = {city:set() for city in cities}
for _ in range(int(h)):
    pair = random.sample(cities, 2)
    while pair[1] in highways[pair[0]] or pair[0] in highways[pair[1]]:
        pair = random.sample(cities, 2)
    highways[pair[0]].add(pair[1])

# Generate trips
trips = {city:set() for city in cities}
for _ in range(int(t)):
    pair = random.sample(cities, 2)
    while pair[1] in trips[pair[0]] and pair[0] in trips[pair[1]]:
        pair = random.sample(cities, 2)
    if pair[1] in trips[pair[0]]:
        trips[pair[0]].add(pair[1])
    else:
        trips[pair[1]].add(pair[0])

print trips

data = n + '\n'
data += '\n'.join(city + ' ' + str(toll) for city,toll in cities.iteritems())
data += '\n' + h + '\n'
data += '\n'.join(cityA + ' ' + cityB for cityA in highways for cityB in highways[cityA])
data += '\n' + t + '\n'
data += '\n'.join(cityA + ' ' + cityB for cityA in trips for cityB in trips[cityA])
print data

with open(out, 'w') as out:
    out.write(data)

# NOT ACYCLIC!
