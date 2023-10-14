#pragma once
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
#include "box2d/b2_contact.h"

namespace Flora {
	class PhysicsUtils {
	public:
		static void WarpBody(b2Body* body, glm::vec2 vector, float rotation = 0);
		static void ApplyForce(b2Body* body, glm::vec2 vector, glm::vec2 offset = { 0, 0 });
		static void ApplyImpulse(b2Body* body, glm::vec2 vector, glm::vec2 offset = { 0, 0 });
		static void ApplyTorque(b2Body* body, float rotation);
		static void ApplyAngularImpulse(b2Body* body, float rotation);
		static void SetLinearVelocity(b2Body* body, glm::vec2 vector);
		static void SetAngularVelocity(b2Body* body, float dps);
	public:
		static glm::vec2 GetLinearVelocity(b2Body* body);
		static float GetAngularVelocity(b2Body* body);
	};
}