#pragma once
#include "Flora/Scene/ScriptableEntity.h"
#include "Flora/Utils/PhysicsUtils.h"

namespace Flora {
	class Launch : public ScriptableEntity {
	public:
		void OnCreate() {
			PhysicsUtils::ApplyImpulse(GetComponent<RigidBody2DComponent>().RuntimeBody, { 1, 0.75f }, {1, 1});
			//PhysicsUtils::ApplyAngularImpulse(GetComponent<RigidBody2DComponent>().RuntimeBody, -20);
			//PhysicsUtils::WarpBody(GetComponent<RigidBody2DComponent>().RuntimeBody, { 1, 0 });
		}

		void OnDestroy() {
		
		}

		void OnUpdate(Timestep ts) {
			//PhysicsUtils::ApplyTorque(GetComponent<RigidBody2DComponent>().RuntimeBody, -80);
			//PhysicsUtils::ApplyForce(GetComponent<RigidBody2DComponent>().RuntimeBody, { 100, 75 });
		}
	};
}