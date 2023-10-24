using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    /// <summary>
    /// Class exclusive to creating and using Flora provided development tools. Note that although not recommended, this
    /// can be used in production, as it can only be accessed directly via the Fauna editor by default
    /// and will be locked to player interaction during a production build.
    /// </summary>
    public class Developer
    {
        /// <summary>
        /// Sends a trace string to be output to the Fauna console.
        /// </summary>
        /// <param name="trace">the string trace</param>
        public static void Trace(string trace)
        {
            InternalCalls.CoreTrace(trace);
        }

        /// <summary>
        /// Sends a command to the flora developer command queue. This command queue can
        /// then be read and handled by another class.
        /// </summary>
        /// <param name="command">the command to be sent to the queue</param>
        public static void SendCommand(string command)
        {
            InternalCalls.DevTools_SendCommand(command);
        }

        /// <summary>
        /// Gets the most first queued command from the developer command queue.
        /// </summary>
        /// <returns>the read command as a string. Returns an empty string if there are no available queued commands</returns>
        public static string GetQueuedCommand()
        {
            return InternalCalls.DevTools_GetQueuedCommand();
        }

        /// <summary>
        /// Determines if there are commands in the developer command queue.
        /// </summary>
        /// <returns></returns>
        public static bool HasQueuedCommand()
        {
            return InternalCalls.DevTools_HasQueuedCommand();
        }

        /// <summary>
        /// Resolves the first queued command from the developer command queue, effectively
        /// popping it off of the queue.
        /// </summary>
        public static void ResolveCommand()
        {
            InternalCalls.DevTools_ResolveCommand();
        }

        /// <summary>
        /// Determines how many queued commands are in the developer command queue.
        /// </summary>
        /// <returns>The number of queued commands</returns>
        public uint CountQueuedCommands()
        {
            return InternalCalls.DevTools_CountQueuedCommands();
        }
    }
}
