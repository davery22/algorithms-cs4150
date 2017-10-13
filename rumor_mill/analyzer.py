import sys

with open('a.txt', 'r') as one:
    one = one.readlines()

with open('b.txt', 'r') as two:
    two = two.readlines()

one = ''.join(one)
two = ''.join(two)

one = one.split(' ')
two = two.split(' ')

for i,_ in enumerate(one):
    if one[i] != two[i]:
        print '\x1b[032m{0}\x1b[033m{1}\x1b[0m'.format(one[i], two[i]),
    else:
        print one[i],
