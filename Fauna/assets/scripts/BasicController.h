#pragma once
#include "Flora/Scene/ScriptableEntity.h"
#include "Flora/Core/Input.h"

namespace Flora {
	class BasicController : public ScriptableEntity {
	public:
		void OnCreate() {}

		void OnDestroy() {}

		void OnUpdate(Timestep ts) {
			auto& translation = GetComponent<TransformComponent>().Translation;
			float speed = 5.0f;
			if (Input::IsKeyPressed(Key::W))
				translation.y += speed * ts;
			if (Input::IsKeyPressed(Key::A))
				translation.x -= speed * ts;
			if (Input::IsKeyPressed(Key::S))
				translation.y -= speed * ts;
			if (Input::IsKeyPressed(Key::D))
				translation.x += speed * ts;
		}
	};
}