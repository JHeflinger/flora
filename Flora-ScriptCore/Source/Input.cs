using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    public class Input
    {
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyDown(keycode);
        }

        public static bool IsMouseButtonPressed(MouseCode button)
        {
            return InternalCalls.Input_IsMouseButtonPressed((int)button);
        }

        public static float GetMouseX()
        {
            return InternalCalls.Input_GetMouseX();
        }

        public static float GetMouseY()
        {
            return InternalCalls.Input_GetMouseY();
        }

        public static Vector2 GetMousePosition()
        {
            InternalCalls.Input_GetMousePosition(out Vector2 position);
            return position;
        }
    }
}
