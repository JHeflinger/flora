#pragma once
#include "Flora/Scene/ScriptableEntity.h"
//#include "box2d/b2_body.h"

namespace Flora {
	class Launch : public ScriptableEntity {
	public:
		void OnCreate() {
		
		}

		void OnDestroy() {
		
		}

		void OnUpdate(Timestep ts) {
			RigidBody2DComponent physBody = GetComponent<RigidBody2DComponent>();
			//b2Body* realbody = physBody.RuntimeBody;
		}
	};
}