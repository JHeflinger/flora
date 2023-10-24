#include "flpch.h"
#include "ComponentUtils.h"

namespace Flora {
	glm::mat4 ComponentUtils::GetParentTransform(Entity entity) {
		if (entity.HasComponent<ParentComponent>()) {
			ParentComponent pc = entity.GetComponent<ParentComponent>();
			if (pc.InheritAll || (pc.InheritTransform && !pc.InheritAll)) {
				Entity pe = pc.Parent;
				return GetParentTransform(pe) * pe.GetComponent<TransformComponent>().GetTransform();
			}
		}
		return glm::mat4(1);
	}

	glm::mat4 ComponentUtils::GetWorldTransform(Entity entity) {
		if (entity.HasComponent<ParentComponent>()) {
			ParentComponent pc = entity.GetComponent<ParentComponent>();
			if (pc.InheritAll || (pc.InheritTransform && !pc.InheritAll)) {
				Entity pe = pc.Parent;
				return (GetParentTransform(pe) * pe.GetComponent<TransformComponent>().GetTransform()) * entity.GetComponent<TransformComponent>().GetTransform();
			}
		}
		return entity.GetComponent<TransformComponent>().GetTransform();
	}
}