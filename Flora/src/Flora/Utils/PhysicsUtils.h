#pragma once
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "glm/glm.hpp"

namespace Flora {
	class PhysicsUtils {
	public:
		static void WarpBody(b2Body* body, glm::vec2 vector, float rotation);
		static void ApplyForce(b2Body* body, glm::vec2 vector);
		static void ApplyImpulse(b2Body* body, glm::vec2 vector);
		static void SetLinearVelocity(b2Body* body, glm::vec2 vector);
		static void SetAngularVelocity(b2Body* body, float dps);
	};
}