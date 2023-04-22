#pragma once
#include "Flora/Scene/ScriptableEntity.h"
#include <glm/glm.hpp>

namespace Flora {
	class AutoRotater : public ScriptableEntity {
	public:
		void OnCreate() {}

		void OnDestroy() {}

		void OnUpdate(Timestep ts) {
			auto& rotation = GetComponent<TransformComponent>().Rotation;
			float speed = 5.0f;
			rotation.x += speed * ts * 0.5f;
			rotation.y += speed * ts;
			rotation.z += speed * ts * 1.5f;
			if (rotation.x > glm::radians(360.0f)) rotation.x -= glm::radians(360.0f);
			if (rotation.y > glm::radians(360.0f)) rotation.y -= glm::radians(360.0f);
			if (rotation.z > glm::radians(360.0f)) rotation.z -= glm::radians(360.0f);
		}
	};
}