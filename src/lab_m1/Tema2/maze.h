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
		vector<vector<int>> matrix;
	};
}