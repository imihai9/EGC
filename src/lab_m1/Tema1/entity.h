#include <vector>
#include "core/gpu/mesh.h"

// Base abstract class for entities (player, enemy, obstacle)

class Entity {
	public:
		virtual void Create() = 0;
		virtual std::vector<Mesh*> const& getMeshes() = 0;
		glm::mat3 modelMatrix; // the local model matrix
};