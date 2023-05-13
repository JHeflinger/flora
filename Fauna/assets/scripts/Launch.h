#pragma once
#include "Flora/Scene/ScriptableEntity.h"
#include "Flora/Utils/PhysicsUtils.h"

namespace Flora {
	class Launch : public ScriptableEntity {
	public:
		void OnCreate() {
			PhysicsUtils::ApplyImpulse(GetComponent<RigidBody2DComponent>().RuntimeBody, { 100, 75 });
		}

		void OnDestroy() {
		
		}

		void OnUpdate(Timestep ts) { 

		}
	};
}