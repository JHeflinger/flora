#pragma once

namespace Flora::Math {
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
	glm::mat4 ComposeTransform(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale);
}