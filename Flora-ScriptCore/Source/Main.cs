using System;
using System.Runtime.CompilerServices;

namespace Flora
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CoreTrace(string log);
    }

    public struct Vector3
    {
        public float X, Y, Z;
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }

    public class Entity
    {
        public float FloatVar { get; set; }

        public Entity()
        {
            Console.WriteLine("Main Constructor!");

            Vector3 position = new Vector3(1, 2, 3);

            InternalCalls.CoreTrace("Foobar");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World From C#!");
        }

        public void PrintInt(int value)
        {
            Console.WriteLine($"C# says integer is: {value}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# Says: {message}");
        }
    }
}