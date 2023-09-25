#pragma once
#include "Flora/Scene/Components.h"

namespace Flora {
	class ComponentUtils {
	public:
		static glm::mat4 GetParentTransform(Entity entity);
		static glm::mat4 GetWorldTransform(Entity entity);
	};
}