﻿// AForge Core Library
// AForge.NET framework
// http://www.aforgenet.com/framework/
//
// Copyright © AForge.NET, 2007-2011
// contacts@aforgenet.com
//

namespace AForge
{
    using System;
    using System.ComponentModel;

    /// <summary>
    /// Structure for representing a pair of coordinates of integer type.
    /// </summary>
    /// 
    /// <remarks><para>The structure is used to store a pair of integer coordinates.</para>
    /// 
    /// <para>Sample usage:</para>
    /// <code>
    /// // assigning coordinates in the constructor
    /// IPOINT p1 = new IPOINT( 10, 20 );
    /// // creating a point and assigning coordinates later
    /// IPOINT p2;
    /// p2.X = 30;
    /// p2.Y = 40;
    /// // calculating distance between two points
    /// float distance = p1.DistanceTo( p2 );
    /// </code>
    /// </remarks>
    /// 
    [Serializable]
    public struct IPOINT
    {
        /// <summary> 
        /// X coordinate.
        /// </summary> 
        /// 
        public int X;

        /// <summary> 
        /// Y coordinate.
        /// </summary> 
        /// 
        public int Y;

        /// <summary>
        /// Initializes a new instance of the <see cref="IPOINT"/> structure.
        /// </summary>
        /// 
        /// <param name="x">X axis coordinate.</param>
        /// <param name="y">Y axis coordinate.</param>
        /// 
        public IPOINT( int x, int y )
        {
            this.X = x;
            this.Y = y;
        }

        /// <summary>
        /// Calculate Euclidean distance between two points.
        /// </summary>
        /// 
        /// <param name="anotherPoint">Point to calculate distance to.</param>
        /// 
        /// <returns>Returns Euclidean distance between this point and
        /// <paramref name="anotherPoint"/> points.</returns>
        /// 
        public float DistanceTo( IPOINT anotherPoint )
        {
            int dx = X - anotherPoint.X;
            int dy = Y - anotherPoint.Y;

            return (float) sqrt( dx * dx + dy * dy );
        }

        /// <summary>
        /// Calculate squared Euclidean distance between two points.
        /// </summary>
        /// 
        /// <param name="anotherPoint">Point to calculate distance to.</param>
        /// 
        /// <returns>Returns squared Euclidean distance between this point and
        /// <paramref name="anotherPoint"/> points.</returns>
        /// 
        public float SquaredDistanceTo( Point anotherPoint )
        {
            float dx = X - anotherPoint.X;
            float dy = Y - anotherPoint.Y;

            return dx * dx + dy * dy;
        }

        /// <summary>
        /// Addition operator - adds values of two points.
        /// </summary>
        /// 
        /// <param name="point1">First point for addition.</param>
        /// <param name="point2">Second point for addition.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to sum of corresponding
        /// coordinates of specified points.</returns>
        /// 
        static IPOINT operator +( IPOINT point1, IPOINT point2 )
        {
            return new IPOINT( point1.X + point2.X, point1.Y + point2.Y );
        }

        /// <summary>
        /// Addition operator - adds values of two points.
        /// </summary>
        /// 
        /// <param name="point1">First point for addition.</param>
        /// <param name="point2">Second point for addition.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to sum of corresponding
        /// coordinates of specified points.</returns>
        /// 
        static IPOINT Add( IPOINT point1, IPOINT point2 )
        {
            return new IPOINT( point1.X + point2.X, point1.Y + point2.Y );
        }

        /// <summary>
        /// Subtraction operator - subtracts values of two points.
        /// </summary>
        /// 
        /// <param name="point1">Point to subtract from.</param>
        /// <param name="point2">Point to subtract.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to difference of corresponding
        /// coordinates of specified points.</returns>
        ///
        static IPOINT operator -( IPOINT point1, IPOINT point2 )
        {
            return new IPOINT( point1.X - point2.X, point1.Y - point2.Y );
        }

        /// <summary>
        /// Subtraction operator - subtracts values of two points.
        /// </summary>
        /// 
        /// <param name="point1">Point to subtract from.</param>
        /// <param name="point2">Point to subtract.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to difference of corresponding
        /// coordinates of specified points.</returns>
        ///
        static IPOINT Subtract( IPOINT point1, IPOINT point2 )
        {
            return new IPOINT( point1.X - point2.X, point1.Y - point2.Y );
        }

        /// <summary>
        /// Addition operator - adds scalar to the specified point.
        /// </summary>
        /// 
        /// <param name="point">Point to increase coordinates of.</param>
        /// <param name="valueToAdd">Value to add to coordinates of the specified point.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to coordinates of
        /// the specified point increased by specified value.</returns>
        /// 
        static IPOINT operator +( IPOINT point, int valueToAdd )
        {
            return new IPOINT( point.X + valueToAdd, point.Y + valueToAdd );
        }

        /// <summary>
        /// Addition operator - adds scalar to the specified point.
        /// </summary>
        /// 
        /// <param name="point">Point to increase coordinates of.</param>
        /// <param name="valueToAdd">Value to add to coordinates of the specified point.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to coordinates of
        /// the specified point increased by specified value.</returns>
        /// 
        static IPOINT Add( IPOINT point, int valueToAdd )
        {
            return new IPOINT( point.X + valueToAdd, point.Y + valueToAdd );
        }

        /// <summary>
        /// Subtraction operator - subtracts scalar from the specified point.
        /// </summary>
        /// 
        /// <param name="point">Point to decrease coordinates of.</param>
        /// <param name="valueToSubtract">Value to subtract from coordinates of the specified point.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to coordinates of
        /// the specified point decreased by specified value.</returns>
        /// 
        static IPOINT operator -( IPOINT point, int valueToSubtract )
        {
            return new IPOINT( point.X - valueToSubtract, point.Y - valueToSubtract );
        }

        /// <summary>
        /// Subtraction operator - subtracts scalar from the specified point.
        /// </summary>
        /// 
        /// <param name="point">Point to decrease coordinates of.</param>
        /// <param name="valueToSubtract">Value to subtract from coordinates of the specified point.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to coordinates of
        /// the specified point decreased by specified value.</returns>
        /// 
        static IPOINT Subtract( IPOINT point, int valueToSubtract )
        {
            return new IPOINT( point.X - valueToSubtract, point.Y - valueToSubtract );
        }

        /// <summary>
        /// Multiplication operator - multiplies coordinates of the specified point by scalar value.
        /// </summary>
        /// 
        /// <param name="point">Point to multiply coordinates of.</param>
        /// <param name="factor">Multiplication factor.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to coordinates of
        /// the specified point multiplied by specified value.</returns>
        ///
        static IPOINT operator *( IPOINT point, int factor )
        {
            return new IPOINT( point.X * factor, point.Y * factor );
        }

        /// <summary>
        /// Multiplication operator - multiplies coordinates of the specified point by scalar value.
        /// </summary>
        /// 
        /// <param name="point">Point to multiply coordinates of.</param>
        /// <param name="factor">Multiplication factor.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to coordinates of
        /// the specified point multiplied by specified value.</returns>
        ///
        static IPOINT Multiply( IPOINT point, int factor )
        {
            return new IPOINT( point.X * factor, point.Y * factor );
        }

        /// <summary>
        /// Division operator - divides coordinates of the specified point by scalar value.
        /// </summary>
        /// 
        /// <param name="point">Point to divide coordinates of.</param>
        /// <param name="factor">Division factor.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to coordinates of
        /// the specified point divided by specified value.</returns>
        /// 
        static IPOINT operator /( IPOINT point, int factor )
        {
            return new IPOINT( point.X / factor, point.Y / factor );
        }

        /// <summary>
        /// Division operator - divides coordinates of the specified point by scalar value.
        /// </summary>
        /// 
        /// <param name="point">Point to divide coordinates of.</param>
        /// <param name="factor">Division factor.</param>
        /// 
        /// <returns>Returns new point which coordinates equal to coordinates of
        /// the specified point divided by specified value.</returns>
        /// 
        static IPOINT Divide( IPOINT point, int factor )
        {
            return new IPOINT( point.X / factor, point.Y / factor );
        }

        /// <summary>
        /// Equality operator - checks if two points have equal coordinates.
        /// </summary>
        /// 
        /// <param name="point1">First point to check.</param>
        /// <param name="point2">Second point to check.</param>
        /// 
        /// <returns>Returns <see langword="true"/> if coordinates of specified
        /// points are equal.</returns>
        ///
        static bool operator ==( IPOINT point1, IPOINT point2 )
        {
            return ( ( point1.X == point2.X ) && ( point1.Y == point2.Y ) );
        }

        /// <summary>
        /// Inequality operator - checks if two points have different coordinates.
        /// </summary>
        /// 
        /// <param name="point1">First point to check.</param>
        /// <param name="point2">Second point to check.</param>
        /// 
        /// <returns>Returns <see langword="true"/> if coordinates of specified
        /// points are not equal.</returns>
        ///
        static bool operator !=( IPOINT point1, IPOINT point2 )
        {
            return ( ( point1.X != point2.X ) || ( point1.Y != point2.Y ) );
        }

        /// <summary>
        /// Check if this instance of <see cref="IPOINT"/> equal to the specified one.
        /// </summary>
        /// 
        /// <param name="obj">Another point to check equalty to.</param>
        /// 
        /// <returns>Return <see langword="true"/> if objects are equal.</returns>
        /// 
        public override bool Equals( object obj )
        {
            return ( obj is IPOINT ) ? ( this == (IPOINT) obj ) : false;
        }

        /// <summary>
        /// Get hash code for this instance.
        /// </summary>
        /// 
        /// <returns>Returns the hash code for this instance.</returns>
        /// 
        public override int GetHashCode( )
        {
            return X.GetHashCode( ) + Y.GetHashCode( );
        }

        /// <summary>
        /// Implicit conversion to <see cref="Point"/>.
        /// </summary>
        /// 
        /// <param name="point">Integer point to convert to single precision point.</param>
        /// 
        /// <returns>Returns new single precision point which coordinates are implicitly converted
        /// to floats from coordinates of the specified integer point.</returns>
        /// 
        static implicit operator Point( IPOINT point )
        {
            return new Point( point.X, point.Y );
        }

        /// <summary>
        /// Implicit conversion to <see cref="DoublePoint"/>.
        /// </summary>
        /// 
        /// <param name="point">Integer point to convert to double precision point.</param>
        /// 
        /// <returns>Returns new double precision point which coordinates are implicitly converted
        /// to doubles from coordinates of the specified integer point.</returns>
        /// 
        static implicit operator DoublePoint( IPOINT point )
        {
            return new DoublePoint( point.X, point.Y );
        } 

        /// <summary>
        /// Get string representation of the class.
        /// </summary>
        /// 
        /// <returns>Returns string, which contains values of the point in readable form.</returns>
        ///
        public override string ToString( )
        {
            return string.Format( System.Globalization.CultureInfo.InvariantCulture, "{0}, {1}", X, Y );
        }

        /// <summary>
        /// Calculate Euclidean norm of the vector comprised of the point's 
        /// coordinates - distance from (0, 0) in other words.
        /// </summary>
        /// 
        /// <returns>Returns point's distance from (0, 0) point.</returns>
        /// 
        public float EuclideanNorm( )
        {
            return (float) sqrt( X * X + Y * Y );
        }
    }    
}
