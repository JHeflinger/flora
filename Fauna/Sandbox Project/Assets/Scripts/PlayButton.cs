using System;
using Flora;

namespace Game
{
	public class PlayButton : Entity
	{
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
				Entity highlight = cc.GetChild(0);
				if (highlight.HasComponent<SpriteRendererComponent>())
                {
					Entity hovered = Visual.GetHoveredEntity();
					if (hovered != null) {
						if (hovered.ID == this.ID || hovered.ID == highlight.ID)
						{
							highlight.GetComponent<SpriteRendererComponent>().Visible = true;
						}
						else
						{
							highlight.GetComponent<SpriteRendererComponent>().Visible = false;
						}
					}
				}
            }*/
		}
	}
}