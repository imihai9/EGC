#include "lab_m1/Tema2/maze.h"
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <core/gpu/mesh.h>

using namespace tema2;
using namespace std;

vector<vector<int>> Maze::getMatrix() {
	return this->matrix;
}

Maze::Maze() {
	
	this->matrix = {
	  {1,1,1,1,1,1,1,1,1,1,1,1,1},
	  {1,2,1,2,1,0,1,0,0,0,0,0,1},
	  {1,2,1,2,0,0,1,0,1,1,1,0,1},
	  {1,0,0,0,1,1,1,0,0,0,0,0,1},
	  {1,0,1,0,0,0,0,0,1,1,1,0,1},
	  {1,0,1,0,1,1,1,0,1,0,0,0,1},
	  {1,0,1,0,1,0,0,0,1,1,1,0,1},
	  {1,0,1,0,1,1,1,0,1,0,1,0,1},
	  {1,0,0,0,0,0,0,0,0,0,1,0,1},
	  {1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	
	/*
	this->matrix = {
		{0, 1, 1, 1, 0},
		{0, 1, 2, 1, 0},
		{0, 1, 1, 1, 0}
	}; */
}
