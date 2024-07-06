#include "MazeGenerator.h"

int main(){
	Maze* maze = generate(10,10);
	print_maze(maze);
	free_maze(maze);
	return 0;
}