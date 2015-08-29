# Jigsaw Solver
An automatic jigsaw puzzle solver.

## Features and limitations
This tool can solve jigsaw problem which is required to construct the original
image from a set of non-overlapping, unordered, known-orientation, square puzzle
parts. To do so, we use genetic algorithm. At first, a set of solutions is
generated. These solution then combine together in order to produce new
generation which is usually better than the current one. We keep doing that
after a given time and return the best solutions as the result.

According to the experimental results, this tool can solve puzzle up to 1000
pieces within reasonable time. The output image is almost the same or near the
same as original image. Illustrator and lower resolution (compare to number of
piece) image give bad results.

## Installation

Compile the jigsaw solver by running the following command
```bash
$ make all
```

in the main directory. This will generate ```jigsawsolver``` file in the ```bin```
subdirectory. After that, you can use this tool:
```bash
$ bin/jigsawsolver samples/prob01.ppm -o result01.ppm
```

Run
```bash
$ bin/jigsawsolver
```

to see details about available options and syntax.

## Future Plans
...
