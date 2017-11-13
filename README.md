# Kattis Algorithms
My solutions to a number of the U of U's algorithms puzzles on Kattis.

Since we are in the unique situation where all solutions are truly one-offs, only one file is allowed in the submission, and correctness and execution speed are the ONLY evaluation metrics, this code will look different from most anything I've ever written. I like to think I bagged up my lifelong potential for reinventing wheels, and got it all out in an academic setting. As an added challenge (and performance boost), all my final solutions are written in C.

Most of these solutions are currently on the leaderboard for top 10 all-time performances.

## Note:
Each solution can be built by running `gcc <solver-file>` from the problem directory. Solutions take input from STDIN and direct output to STDOUT, so a sample run might look like `./a.out < test.in`. 

## Testing:
Tests are organized into `tests/` subdirectories. A .in file is a test input, while a .ans file is the corresponding test output (using the same base file name). There is a simple helper script `run_tests.sh` in the root directory, which can be called from a problem directory to run all tests for that problem (may require some configuration - see `run_tests.sh` for more documentation). 
