#pragma once
#include "Flora/Scene/ScriptableEntity.h"

namespace Flora {
	class CameraBinder : public ScriptableEntity {
	public:
		void OnCreate() {
		
		}

		void OnDestroy() {
		
		}

		void OnUpdate(Timestep ts) {
			//glm::vec3 translation = GetScene()->GetEntitiesByTag("Player")[0].GetComponent<TransformComponent>().Translation;
			//GetComponent<TransformComponent>().Translation = translation;
		}
	};
}