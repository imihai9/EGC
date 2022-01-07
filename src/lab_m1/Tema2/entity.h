#pragma once

#include <vector>
#include "core/gpu/mesh.h"

// Base abstract class for entities

namespace tema2 {
	class Entity {
	public:
		// Primitive structure
		// Holds primitive name + model matrix (used for rendering in tema2.cpp)
		struct Primitive {
			char* name;
			glm::mat4 modelMatrix;
		};

		virtual void Create() = 0;
		virtual std::vector<Primitive> const& getPrimitives() = 0;
		glm::mat4 modelMatrix; // the local model matrix
	};
}