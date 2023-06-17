using System;

namespace Flora
{
    public class Main
    {
        public float FloatVar { get; set; }

        public Main()
        {
            Console.WriteLine("Main Constructor!");
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