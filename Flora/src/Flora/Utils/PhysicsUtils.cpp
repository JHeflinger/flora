#include "flpch.h"
#include "PhysicsUtils.h"

namespace Flora {
	void PhysicsUtils::WarpBody(b2Body* body, glm::vec2 vector, float rotation) {

	}

	void PhysicsUtils::ApplyForce(b2Body* body, glm::vec2 vector) {
		body->ApplyForce(b2Vec2(vector.x, vector.y), body->GetWorldCenter(), true);
	}

	void PhysicsUtils::ApplyImpulse(b2Body* body, glm::vec2 vector) {
		body->ApplyLinearImpulse(b2Vec2(vector.x, vector.y), body->GetWorldCenter(), true);
	}

	void PhysicsUtils::SetLinearVelocity(b2Body* body, glm::vec2 vector) {

	}

	void PhysicsUtils::SetAngularVelocity(b2Body* body, float dps) {

	}
}