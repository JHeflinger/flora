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
		uint cardCounter = 0;

		void OnCreate()
		{

		}

		void OnDestroy()
		{

		}

		void OnUpdate(float ts)
		{
			Entity templateCard = Scene.FindEntityByName("Stamina");
			if (copy || cardCounter < 300)
			{
				if (templateCard == null)
                {
					Console.WriteLine("its null...");
					return;
                }
				Entity copyCardEntity = Scene.CopyEntity(templateCard);
				cardCounter++;
				copyCardEntity.GetComponent<TagComponent>().Tag = "Stamina (" + cardCounter.ToString() + ")";
				copy = false;
			}
		}
	}
}