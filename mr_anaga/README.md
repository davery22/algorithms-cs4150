# Solution to the 'Mr. Anaga' problem

## Input
The first line contains integers _n_ and _k_ separated by a space, where 1≤_n_≤10000 and 1≤_k_≤1000. The _n_ words, one to a line, follow. Each word contains exactly _k_ lower case letters. (The words are not necessarily in any dictionary you’ve ever seen.)

## Output
Produce a single line of output that contains the number of words on the list that are not anagrams of any other words on the list. This number, of course, should be between 0 and _n_ inclusive.

### Testing
Usage: `python create_test.py <n> <k> [output.txt]`
Creates a problem input file with _n_ words each consisting of _k_ randomly chosen lowercase letters. Randomly installs reshuffled duplicates of words to facilitate anagram presence in the list.
Can optionally specify a name for the created file.
