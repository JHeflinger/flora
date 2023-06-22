using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Flora;

namespace Sandbox
{
    public class Player : Entity
    {
        void OnCreate()
        {
            Console.WriteLine("Player.OnCreate");
        }

        void OnDestroy()
        {
            Console.WriteLine("Player.OnDestroy");
        }

        void OnUpdate(float ts)
        {
            Console.WriteLine($"Plsyer.OnUpdate({ts})");
        }
    }
}
