import sys
import random
import uuid

n = sys.argv[1]
f = sys.argv[2]
r = sys.argv[3]
out = sys.argv[4] if len(sys.argv) > 4 else "output.txt"

# Generate student names
students = set()
for _ in range(int(n)):
    student = uuid.uuid4().hex[:5]
    while (student in students):
        student = uuid.uuid4().hex[:5]
    students.add(student)

# Generate friend pairs
friends = {student:set() for student in students}
for _ in range(int(f)):
    pair = random.sample(students, 2)
    while pair[1] in friends[pair[0]] or pair[0] in friends[pair[1]]:
        pair = random.sample(students, 2)
    friends[pair[0]].add(pair[1])

# Generate the rumor starters
rumors = random.sample(students, int(r))

data = n + '\n'
data += '\n'.join(student for student in students)
data += '\n' + f + '\n'
data += '\n'.join(studA + ' ' + studB for studA in friends for studB in friends[studA])
data += '\n' + r + '\n'
data += '\n'.join(student for student in rumors)

with open(out, 'w') as out:
    out.write(data)

# NOT ACYCLIC!
