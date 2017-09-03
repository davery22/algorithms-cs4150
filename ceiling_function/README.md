#Solution to the 'Ceiling Function' problem

##Input
The first line of the input contains two integers _n_ (1≤_n_≤50), which is the number of ceiling prototypes to analyze, and _k_ (1≤_k_≤20), which is the number of layers in each of the prototypes.

The next _n_ lines describe the ceiling prototypes. Each of these lines contains _k_ distinct integers (between 1 and 10^6, inclusive), which are the collapse-resistance values of the layers in a ceiling prototype, ordered from top to bottom.

##Output
Display the number of different tree shapes.

###Testing
Usage: `python create_test.py <n> <k> [output.txt]`
Creates a problem input file with _n_ lines each consisting of _k_ random integers. 
Can optionally specify a name for the created file.
