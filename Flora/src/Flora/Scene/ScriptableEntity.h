#pragma once
#include "Flora/Scene/Entity.h"

namespace Flora {
	class ScriptableEntity {
	public:
		virtual ~ScriptableEntity() {}
		template<typename T>
		T& GetComponent() {
			return m_Entity.GetComponent<T>();
		}
		Scene* GetScene() {
			return m_Entity.GetScene();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}