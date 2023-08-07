using System;
using Flora;

namespace Game
{
	public class Stamina : Entity
	{
		public int stamina;
		ChildComponent cc;

		void OnCreate()
		{
			if (HasComponent<ChildComponent>())
				cc = GetComponent<ChildComponent>();
		}

		void OnDestroy()
		{
		
		}

		void OnUpdate(float ts)
		{
			if (HasComponent<ChildComponent>())
            {
				Entity counter = cc.GetChild(0);
				if (counter.HasComponent<SpriteRendererComponent>())
                {
					int newCoord = stamina;
					if (newCoord > 9) newCoord = 9;
					if (newCoord <= 0) newCoord = 10;
					counter.GetComponent<SpriteRendererComponent>().ColumnCoordinate = newCoord;
                }
            }
		}
	}
}