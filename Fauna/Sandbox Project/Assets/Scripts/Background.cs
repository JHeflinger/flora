using System;
using Flora;

namespace Game
{
	public class Background : Entity
	{
		TransformComponent tc;
		float width, height;

		void OnCreate()
		{
			if (HasComponent<TransformComponent>())
				tc = GetComponent<TransformComponent>();
			width = height = 2.0f * Scene.GetPrimaryCamera().GetComponent<CameraComponent>().OrthographicSize;
		}

		void OnDestroy()
		{
		
		}

		void OnUpdate(float ts)
		{
			if (HasComponent<TransformComponent>())
			{
				tc.Scale = new Vector3(width, height, 1.0f);
			}
		}
	}
}