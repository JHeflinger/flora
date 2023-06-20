using System;
using System.Runtime.CompilerServices;

namespace Flora
{
    public class Main
    {
        public float FloatVar { get; set; }

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

        public Main()
        {
            Console.WriteLine("Main Constructor!");

            Vector3 position = new Vector3(1, 2, 3);
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

        /*
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void CppFunc();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void NativePrint(string text, int num);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void PrintVec(ref Vector3 vec);*/
    }
}