#pragma once
#include "Flora/Scene/Entity.h"

namespace Flora {
	class ScriptableEntity {
	public:
		template<typename T>
		T& GetComponent() {
			return m_Entity.GetComponent<T>();
		}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}