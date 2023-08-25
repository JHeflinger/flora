using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    public class Project
    {
        public static string GetAssetDirectory()
        {
            return InternalCalls.Project_GetAssetDirectory();
        }
    }
}
