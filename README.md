# External Sort - K-Way Merge Sort Algorithm

## Aim

External Sorting is a class of algorithms used to deal with massive amounts of data that do not fit in memory. This question aims at implementing one such type: K-Way merge sort algorithm to sort a very large array. This algorithm is a perfect example of the use of divide and conquer where with limited resources large problems are tackled by breaking the problem space into small computable subspaces, and then operations are done on them.

## Input Format

- `Input.txt` contains space-separated integers.
- The program should take two command-line arguments.
  - First command-line argument is the input file path.
  - Second command-line argument is the output file path.
- For example, if you have a directory named `data` in your current working directory inside which you have kept the `input.txt` file and you wish to create `output.txt` inside the `data` directory itself, then run the program using the following command:
- ./a.out ./data/input.txt ./data/output.txt
- Input files can be arbitrarily large.

## Output Format

- Output file should contain integers in non-descending sorted order in a new line.

## Generation of Unsorted File

To generate the unsorted file, a python script is uploaded. It contains all the instructions required to run it.
