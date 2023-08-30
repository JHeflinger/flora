using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    public class Developer
    {
        public static void Trace(string command)
        {
            InternalCalls.CoreTrace(command);
        }

        public static void SendCommand(string command)
        {
            InternalCalls.DevTools_SendCommand(command);
        }

        public static string GetQueuedCommand()
        {
            return InternalCalls.DevTools_GetQueuedCommand();
        }

        public static bool HasQueuedCommand()
        {
            return InternalCalls.DevTools_HasQueuedCommand();
        }

        public static void ResolveCommand()
        {
            InternalCalls.DevTools_ResolveCommand();
        }

        public uint CountQueuedCommands()
        {
            return InternalCalls.DevTools_CountQueuedCommands();
        }
    }
}
