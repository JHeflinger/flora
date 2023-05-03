#pragma once
#include "Flora/Scene/ScriptableEntity.h"

namespace Flora {
	class TestScript : public ScriptableEntity {
	public:
		void OnCreate() {
		
		}

		void OnDestroy() {
		
		}

		void OnUpdate(Timestep ts) {
			Scene* myScene = GetScene();
			Entity mainCamera = myScene->GetEntitiesByTag("Main Camera")[0];
			auto& rotation = mainCamera.GetComponent<TransformComponent>().Rotation;
			float speed = 0.1f;
			rotation.x += speed;
		}
	};
}