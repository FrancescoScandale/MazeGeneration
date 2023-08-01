# University project - Maze Generation

Starting from a grid (representing cells of a maze, each with its 4 walls), remove a random wall from a cell (chosen at random as well). <br>
Stop when there is a path between the top left cell and the bottom right cell.
Use the Disjoint Set theory and the interface they provide.

## Compile and execute
To compile the file, type in the terminal: <br>
gcc maze.c -o maze <br><br>

To then execute it, with n and m being the sizes of the maze to be generated, type: <br>
./maze n m
