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
        private static Dictionary<KeyCode, bool> m_KeyPressMap = new Dictionary<KeyCode, bool>();
        private static Dictionary<MouseCode, bool> m_MousePressMap = new Dictionary<MouseCode, bool>();

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
        /// Determines if a specified key was just released. Note that the intention
        /// of this function is to be called every frame, as it needs to track which
        /// keys are being pressed to know when they are being released.
        /// </summary>
        /// <param name="keycode">The keycode that corresponds to the desired keyboard key</param>
        /// <returns>true of the key was released, false otherwise</returns>
        public static bool IsKeyReleased(KeyCode keycode)
        {
            bool outval;
            if (m_KeyPressMap.TryGetValue(keycode, out outval))
            {
                if (IsKeyDown(keycode))
                {
                    m_KeyPressMap[keycode] = true;
                    return false;
                } else
                {
                    if (outval)
                    {
                        m_KeyPressMap[keycode] = false;
                        return true;
                    }
                }
            } else m_KeyPressMap[keycode] = false;
            return false;
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
        /// Determines if a specified mouse button was just released. Note that the intention
        /// of this function is to be called every frame, as it needs to track which
        /// mouse buttons are being pressed to know when they are being released.
        /// </summary>
        /// <param name="button">The mousecode that corresponds to the desired mouse button</param>
        /// <returns>true of the button was released, false otherwise</returns>
        public static bool IsMouseButtonReleased(MouseCode button)
        {
            bool outval;
            if (m_MousePressMap.TryGetValue(button, out outval))
            {
                if (IsMouseButtonPressed(button))
                {
                    m_MousePressMap[button] = true;
                    return false;
                }
                else
                {
                    if (outval)
                    {
                        m_MousePressMap[button] = false;
                        return true;
                    }
                }
            }
            else m_MousePressMap[button] = false;
            return false;
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
