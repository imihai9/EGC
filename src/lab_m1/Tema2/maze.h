#pragma once

#include <vector>
#include "core/gpu/mesh.h"

using namespace std;

namespace tema2 {
	class Maze
	{
	public:
		Maze();
		vector<vector<int>> getMatrix();

	private:
		// 0 = empty space
		// 1 = wall
		// 2 = enemy

		vector<vector<int>> matrix;
	};
}