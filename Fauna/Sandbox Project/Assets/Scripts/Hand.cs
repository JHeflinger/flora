using System;
using Flora;
using System.Collections;

namespace Game
{
	public class Hand : Entity
	{
		ArrayList hand;
		Entity templateCard;
		public bool copy;

		void OnCreate()
		{

		}

		void OnDestroy()
		{

		}

		void OnUpdate(float ts)
		{
			Entity templateCard = Scene.FindEntityByName("Template Card");
			if (templateCard is Card card)
			{
				if (copy)
                {
					Console.WriteLine("copied template");
					Scene.CopyEntity(templateCard);
					copy = false;
                }
			}
		}
	}
}