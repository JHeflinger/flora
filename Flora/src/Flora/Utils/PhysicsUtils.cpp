#include "flpch.h"
#include "PhysicsUtils.h"

namespace Flora {
	void PhysicsUtils::WarpBody(b2Body* body, glm::vec2 vector) {
		body->SetTransform(b2Vec2(vector.x, vector.y), body->GetAngle());
	}

	void PhysicsUtils::WarpBody(b2Body* body, glm::vec2 vector, float rotation) {
		body->SetTransform(b2Vec2(vector.x, vector.y), rotation);
	}

	void PhysicsUtils::ApplyForce(b2Body* body, glm::vec2 vector, glm::vec2 offset) {
		body->ApplyForce(b2Vec2(vector.x, vector.y), body->GetWorldPoint((b2Vec2(offset.x, offset.y))), true);
	}

	void PhysicsUtils::ApplyImpulse(b2Body* body, glm::vec2 vector, glm::vec2 offset) {
		body->ApplyLinearImpulse(b2Vec2(vector.x, vector.y), body->GetWorldPoint((b2Vec2(offset.x, offset.y))), true);
	}

	void PhysicsUtils::ApplyTorque(b2Body* body, float rotation) {
		body->ApplyTorque(rotation, true);
	}

	void PhysicsUtils::ApplyAngularImpulse(b2Body* body, float rotation) {
		body->ApplyAngularImpulse(rotation, true);
	}

	void PhysicsUtils::SetLinearVelocity(b2Body* body, glm::vec2 vector) {
		body->SetLinearVelocity(b2Vec2(vector.x, vector.y));
	}

	void PhysicsUtils::SetAngularVelocity(b2Body* body, float dps) {
		body->SetAngularVelocity(dps);
	}

	glm::vec2 PhysicsUtils::GetLinearVelocity(b2Body* body) {
		return { body->GetLinearVelocity().x, body->GetLinearVelocity().y };
	}

	float PhysicsUtils::GetAngularVelocity(b2Body* body) {
		return body->GetAngularVelocity();
	}
}