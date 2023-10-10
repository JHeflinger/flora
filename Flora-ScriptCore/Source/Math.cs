using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Flora
{
    /// <summary>
    /// Contains Flora specific math functions for ease of use.
    /// </summary>
    public class FMath
    {
        /// <summary>
        /// Normalizes a Vector2 so its magnitude has a value of 1.
        /// </summary>
        /// <param name="vector">the vector to normalize</param>
        /// <returns>the normalized vector</returns>
        public static Vector2 Normalize(Vector2 vector)
        {
            double magnitude = Math.Sqrt(vector.X * vector.X + vector.Y * vector.Y);
            if (magnitude == 0) return new Vector2(0.0f);
            return vector * (1.0f / (float)magnitude);
        }

        /// <summary>
        /// Returns a radian value given a degree value
        /// </summary>
        /// <param name="degrees">given degrees</param>
        /// <returns>converted radians</returns>
        public static float Radians(float degrees)
        {
            return ((float)(Math.PI) / 180.0f) * degrees;
        }

        /// <summary>
        /// Returns a degree value given a radian value
        /// </summary>
        /// <param name="radians">given radians</param>
        /// <returns>converted degrees</returns>
        public static float Degrees(float radians)
        {
            return (180.0f * radians) / ((float)(Math.PI));
        }
    }

    /// <summary>
    /// A 4 element vector to represent a series of 4 floating point values
    /// </summary>
    public struct Vector4
    {
        public float X, Y, Z, W;

        /// <summary>
        /// A Vector4 with all values initialized as 0.0f.
        /// </summary>
        public static Vector4 Zero => new Vector4(0.0f);

        /// <summary>
        /// A complete Vector4 representation.
        /// </summary>
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

        /// <summary>
        /// A Vector3 representation of the first 3 elements of this Vector4
        /// </summary>
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

        /// <summary>
        /// A Vector2 representation of the first 2 elements of this Vector4
        /// </summary>
        public Vector2 XY
        {
            get => new Vector2(X, Y);
            set
            {
                X = value.X;
                Y = value.Y;
            }
        }

        /// <summary>
        /// Creates a new vector with all 4 values initialized to a given scalar.
        /// </summary>
        /// <param name="scalar">Initial value of the elements</param>
        public Vector4(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
            W = scalar;
        }

        /// <summary>
        /// Creates a new Vector4 with specified values for each element
        /// </summary>
        /// <param name="x">X value</param>
        /// <param name="y">Y value</param>
        /// <param name="z">Z value</param>
        /// <param name="w">W value</param>
        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        /// <summary>
        /// Allows you to multiply all vector values by a scalar via standard operation. (E.G. ourvector = myvector*3.0f;)
        /// </summary>
        /// <param name="vector">vector to multiply</param>
        /// <param name="scalar">scalar to multiply the vector by</param>
        /// <returns>a new multiplied vector</returns>
        public static Vector4 operator *(Vector4 vector, float scalar)
        {
            return new Vector4(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
        }

        /// <summary>
        /// Allows you to add 2 vector values together via standard operation. (E.G. ourvector = myvector + yourvector;)
        /// </summary>
        /// <param name="a">vector to add</param>
        /// <param name="b">vector to add</param>
        /// <returns>resulting vector</returns>
        public static Vector4 operator +(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
        }

        /// <summary>
        /// Allows you to subtract 2 vector values together via standard operation. (E.G. ourvector = myvector - yourvector;)
        /// </summary>
        /// <param name="a">vector to subtract</param>
        /// <param name="b">vector to subtract</param>
        /// <returns>resulting vector</returns>
        public static Vector4 operator -(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);
        }
    }

    /// <summary>
    /// A 3 element vector to represent a series of 3 floating point values
    /// </summary>
    public struct Vector3
    {
        public float X, Y, Z;

        /// <summary>
        /// A Vector3 with all values initialized as 0.0f.
        /// </summary>
        public static Vector3 Zero => new Vector3(0.0f);

        /// <summary>
        /// A full Vector3 representation of all 3 elements
        /// </summary>
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

        /// <summary>
        /// A Vector2 representation of the first 2 elements
        /// </summary>
        public Vector2 XY
        {
            get => new Vector2(X, Y);
            set
            {
                X = value.X;
                Y = value.Y;
            }
        }

        /// <summary>
        /// Creates a new vector with all 3 values initialized to a given scalar.
        /// </summary>
        /// <param name="scalar">Initial value of the elements</param>
        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        /// <summary>
        /// A Vector3 with specified values for each element
        /// </summary>
        /// <param name="x">X value</param>
        /// <param name="y">Y value</param>
        /// <param name="z">Z value</param>
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        /// <summary>
        /// Allows you to multiply all vector values by a scalar via standard operation. (E.G. ourvector = myvector*3.0f;)
        /// </summary>
        /// <param name="vector">vector to multiply</param>
        /// <param name="scalar">scalar to multiply the vector by</param>
        /// <returns>a new multiplied vector</returns>
        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        /// <summary>
        /// Allows you to add 2 vector values together via standard operation. (E.G. ourvector = myvector + yourvector;)
        /// </summary>
        /// <param name="a">vector to add</param>
        /// <param name="b">vector to add</param>
        /// <returns>resulting vector</returns>
        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        /// <summary>
        /// Allows you to subtract 2 vector values together via standard operation. (E.G. ourvector = myvector - yourvector;)
        /// </summary>
        /// <param name="a">vector to subtract</param>
        /// <param name="b">vector to subtract</param>
        /// <returns>resulting vector</returns>
        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }
    }

    /// <summary>
    /// A 2 element vector to represent a series of 2 floating point values
    /// </summary>
    public struct Vector2
    {
        public float X, Y;

        /// <summary>
        /// A Vector2 with all values initialized as 0.0f.
        /// </summary>
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

        /// <summary>
        /// Creates a new vector with 2 values initialized to a given scalar.
        /// </summary>
        /// <param name="scalar">Initial value of the elements</param>
        public Vector2(float scalar)
        {
            X = scalar;
            Y = scalar;
        }

        /// <summary>
        /// A Vector2 with specified values for each element
        /// </summary>
        /// <param name="x">X value</param>
        /// <param name="y">Y value</param>
        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        /// <summary>
        /// Creates a Vector2 from the first 2 values of a Vector3
        /// </summary>
        /// <param name="vec3">given vector3</param>
        public Vector2(Vector3 vec3)
        {
            X = vec3.X;
            Y = vec3.Y;
        }

        /// <summary>
        /// Allows you to multiply all vector values by a scalar via standard operation. (E.G. ourvector = myvector*3.0f;)
        /// </summary>
        /// <param name="vector">vector to multiply</param>
        /// <param name="scalar">scalar to multiply the vector by</param>
        /// <returns>a new multiplied vector</returns>
        public static Vector2 operator *(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X * scalar, vector.Y * scalar);
        }

        /// <summary>
        /// Allows you to add 2 vector values together via standard operation. (E.G. ourvector = myvector + yourvector;)
        /// </summary>
        /// <param name="a">vector to add</param>
        /// <param name="b">vector to add</param>
        /// <returns>resulting vector</returns>
        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        /// <summary>
        /// Allows you to subtract 2 vector values together via standard operation. (E.G. ourvector = myvector - yourvector;)
        /// </summary>
        /// <param name="a">vector to subtract</param>
        /// <param name="b">vector to subtract</param>
        /// <returns>resulting vector</returns>
        public static Vector2 operator -(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }
    }
}
