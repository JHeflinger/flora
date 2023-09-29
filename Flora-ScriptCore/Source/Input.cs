using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    /// <summary>
    /// This class contains static functions to determine what user inputs
    /// are being used, such as the keyboard or mouse. Note that there is not controller
    /// support at the moment, but it is planned for future releases.
    /// </summary>
    public class Input
    {
        /// <summary>
        /// Determines if a specified key is being pressed
        /// </summary>
        /// <param name="keycode">The keycode that corresponds to the desired keyboard key</param>
        /// <returns>true if the key is being pressed, false otherwise.</returns>
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyDown(keycode);
        }

        /// <summary>
        /// Determines if a specific mouse button is being pressed
        /// </summary>
        /// <param name="button">The mousecode that corresponds to the desired mouse button</param>
        /// <returns>true of the mouse button is being pressed, false otherwise.</returns>
        public static bool IsMouseButtonPressed(MouseCode button)
        {
            return InternalCalls.Input_IsMouseButtonPressed((int)button);
        }

        /// <summary>
        /// Determines the mouse x position.
        /// </summary>
        /// <returns>The mouse x position</returns>
        public static float GetMouseX()
        {
            return InternalCalls.Input_GetMouseX();
        }

        /// <summary>
        /// Determines the mouse y position.
        /// </summary>
        /// <returns>The mouse y position</returns>
        public static float GetMouseY()
        {
            return InternalCalls.Input_GetMouseY();
        }

        /// <summary>
        /// Determines the mouse x and y position.
        /// </summary>
        /// <returns>A 2 element vector containing the x and y position</returns>
        public static Vector2 GetMousePosition()
        {
            InternalCalls.Input_GetMousePosition(out Vector2 position);
            return position;
        }

        /// <summary>
        /// Determines the mouse x and y position in reference to the given camera plane.
        /// </summary>
        /// <returns>A 2 element vector containing the mouse x and y position in reference to the given camera plane.</returns>
        public static Vector2 GetMousePosition(Entity camera)
        {
            InternalCalls.Input_GetMousePositionFromCamera(camera.ID, out Vector2 position);
            return position;
        }
    }
}
