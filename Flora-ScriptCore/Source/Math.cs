using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    public class FMath
    {
        public static float Radians(float degrees)
        {
            return ((float)(Math.PI) / 180.0f) * degrees;
        }

        public static float Degrees(float radians)
        {
            return (180.0f * radians) / ((float)(Math.PI));
        }
    }

    public struct Vector4
    {
        public float X, Y, Z, W;

        public static Vector4 Zero => new Vector4(0.0f);

        public Vector4 XYZW
        {
            get => new Vector4(X, Y, Z, W);
            set
            {
                X = value.X;
                Y = value.Y;
                Z = value.Z;
                W = value.W;
            }
        }

        public Vector3 XYZ
        {
            get => new Vector3(X, Y, Z);
            set
            {
                X = value.X;
                Y = value.Y;
                Z = value.Z;
            }
        }

        public Vector2 XY
        {
            get => new Vector2(X, Y);
            set
            {
                X = value.X;
                Y = value.Y;
            }
        }

        public Vector4(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
            W = scalar;
        }

        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public static Vector4 operator *(Vector4 vector, float scalar)
        {
            return new Vector4(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
        }

        public static Vector4 operator +(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
        }

        public static Vector4 operator -(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);
        }
    }

    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0.0f);

        public Vector3 XYZ
        {
            get => new Vector3(X, Y, Z);
            set
            {
                X = value.X;
                Y = value.Y;
                Z = value.Z;
            }
        }

        public Vector2 XY
        {
            get => new Vector2(X, Y);
            set
            {
                X = value.X;
                Y = value.Y;
            }
        }

        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }
    }

    public struct Vector2
    {
        public float X, Y;

        public static Vector2 Zero => new Vector2(0.0f);

        public Vector2 XY
        {
            get => new Vector2(X, Y);
            set
            {
                X = value.X;
                Y = value.Y;
            }
        }

        public Vector2(float scalar)
        {
            X = scalar;
            Y = scalar;
        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public Vector2(Vector3 vec3)
        {
            X = vec3.X;
            Y = vec3.Y;
        }

        public static Vector2 operator *(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X * scalar, vector.Y * scalar);
        }

        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        public static Vector2 operator -(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }
    }
}
