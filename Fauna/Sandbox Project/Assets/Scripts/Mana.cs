using System;
using Flora;

namespace Game
{
	public class Mana : Entity
	{
		public int mana;
		ChildComponent cc;

		void OnCreate()
		{
			/*if (HasComponent<ChildComponent>())
				cc = GetComponent<ChildComponent>();*/
		}

		void OnDestroy()
		{

		}

		void OnUpdate(float ts)
		{
			/*if (HasComponent<ChildComponent>())
			{
				Entity counter = cc.GetChild(0);
				counter.GetComponent<TagComponent>();
				if (counter.HasComponent<SpriteRendererComponent>())
				{
					int newCoord = mana;
					if (newCoord > 9) newCoord = 9;
					if (newCoord <= 0) newCoord = 10;
					counter.GetComponent<SpriteRendererComponent>().ColumnCoordinate = newCoord;
				}
			}*/
		}
	}
}