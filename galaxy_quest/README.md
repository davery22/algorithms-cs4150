# Solution to the 'Galaxy Quest' problem

## Description
NASA recently confirmed the discovery of parallel universes (PUs) occupying alternate dimensions. These universes are quite different from our own universe, in the following ways:

Each PU is a two-dimensional square that stretches 10^9 light years from left to right and from top to bottom.
Each PU has a galactic diameter of _d_ light years, where _d_ is an integer.
Each star is exactly _x_ light years from its universe’s left edge and _y_ light years from its universe’s bottom edge, where _x_ and _y_ are non-negative integers.
Stars are clustered into galaxies. Each galaxy consists of one or more stars. Each star is at most _d_ light years from every other star in its galaxy. Any two stars from different galaxies are more than _d_ light years apart.
It is crucial to understand the implications of this. The locations of the stars determine the galaxies. The galaxy to which a star _s_ belongs consists of _s_ plus all other stars that are within _d_ light years of _s_. Suppose you draw a circle of diameter _d_ around all of the stars of a galaxy. Any star not belonging to the galaxy will be outside the circle and will be more than _d_ light years from every star in the galaxy.
For each PU, NASA has obtained all of its stellar coordinates and has measured its value of _d_.

Given the description of a PU, NASA would like to be able to determine whether that PU has a galaxy that contains more than half of the stars in the PU. NASA has turned to you.

## Input
The input describes a single PU. All numbers in the input are integers.

The first line of the input contains the PU’s galactic diameter _d_ (1≤_d_≤10^6) and star count _k_ (1≤_k_≤10^6).

There are exactly _k_ more lines. Each line contains the _x_ (0≤_x_≤10^9) and _y_ (0≤_y_≤10^9) coordinates of a star in the PU. No two of these lines are identical, as a black hole would result!

The star positions and _d_ are guaranteed to obey the clustering constraint discussed above.

## Output
If the PU described by the input has a galaxy containing more than half of the stars, display the number of stars in that galaxy. Otherwise, display NO.

### Testing
Usage: `python create_test.py <d> <k> [output.txt]`
Creates a problem input file with galactic diameter _d_, and _k_ stars.
Can optionally specify a name for the created file.

### Notes
`c_solver.c` utilizes an algorithm with recurrence relation T(n) = 2T(n/2) + O(n), for an upper-bound asymptotic complexity of O(nlog(n)) [Master Thm.].
`2c_solver.c` utilizes an algorithm with recurrence relation T(n) = T(n/2) + O(n), for an upper-bound asymptotic complexity of O(n) [Master Thm.].
