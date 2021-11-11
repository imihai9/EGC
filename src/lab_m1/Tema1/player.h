#include "components/simple_scene.h"
#include "lab_m1/Tema1/entity.h"
#include "space.h"

class Player : public Entity
{
	public:
		Player(LogicSpace logicSpace);
		void Create();
		std::vector<Mesh*> const& getMeshes();
	private:
		int meshCount; // number of meshes used to represent the player model
		std::vector<glm::vec3> circleCenters;
		std::vector<Mesh*> meshes;
};
