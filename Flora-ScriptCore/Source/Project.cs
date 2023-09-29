using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    /// <summary>
    /// Class to interact with the current video game project
    /// </summary>
    public class Project
    {
        /// <summary>
        /// Gets the directory containing the assets of the game
        /// </summary>
        /// <returns>The asset directory</returns>
        public static string GetAssetDirectory()
        {
            return InternalCalls.Project_GetAssetDirectory();
        }
    }
}
