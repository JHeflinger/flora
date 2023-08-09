using System;
using Flora;

namespace Game
{
	public class Card : Entity
	{
		public int cost;
		public bool isMagic;
		public bool active;
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
				for (uint i = 0; i < cc.GetChildrenSize(); i++)
                {
					Entity child = cc.GetChild(i);
					if (child.GetComponent<TagComponent>().Tag.Equals("Cost"))
					{
						if (child.HasComponent<SpriteRendererComponent>())
						{
							if (isMagic)
								child.GetComponent<SpriteRendererComponent>().Color = new Vector4(0.2f, 0.3f, 0.8f, 1.0f);
							else
								child.GetComponent<SpriteRendererComponent>().Color = new Vector4(0.8f, 0.3f, 0.2f, 1.0f);
							
							int newCoord = cost;
							if (newCoord > 9) newCoord = 9;
							if (newCoord <= 0) newCoord = 10;
							child.GetComponent<SpriteRendererComponent>().ColumnCoordinate = newCoord;
						}
					}
                }
            }
		}
	}
}