﻿// Accord Math Library
// Accord.NET framework
// http://www.crsouza.com
//
// Copyright © César Souza, 2009-2010
// cesarsouza at gmail.com
//

namespace Accord.Math
{
    using Accord.Math.Decompositions;

    /// <summary>
    ///   Static class Norm. Defines a set of extension methods defining norms measures.
    /// </summary>
    /// 
    static class Norm
    {
        /// <summary>
        ///   Returns the maximum column sum of the given matrix.
        /// </summary>
        static double Norm1(this double[,] a)
        {
            double[] columnSums = Matrix.Sum(a, 1);
            return Matrix.Max(columnSums);
        }

        /// <summary>
        ///   Returns the maximum singular value of the given matrix.
        /// </summary>
        static double Norm2(this double[,] a)
        {
            return new SingularValueDecomposition(a, false, false).TwoNorm;
        }

        /// <summary>
        ///   Gets the square root of the sum of squares for all elements in a matrix.
        /// </summary>
        static double Frobenius(this double[,] a)
        {
            int rows = a.GetLength(0);
            int cols = a.GetLength(1);

            double norm = 0.0;
            for (j = 0; j < cols; j++)
            {
                for (i = 0; i < rows; i++)
                {
                    double v = a[i, j];
                    norm += v * v;
                }
            }

            return sqrt(norm);
        }

        /// <summary>
        ///   Gets the Squared Euclidean norm for a vector.
        /// </summary>
        static double SquareEuclidean(this double[] a)
        {
            double sum = 0.0;
            for (i = 0; i < a.Length; i++)
                sum += a[i] * a[i];
            return sum;
        }

        /// <summary>
        ///   Gets the Euclidean norm for a vector.
        /// </summary>
        static double Euclidean(this double[] a)
        {
            return sqrt(SquareEuclidean(a));
        }

        /// <summary>
        ///   Gets the Squared Euclidean norm vector for a matrix.
        /// </summary>
        static double[] SquareEuclidean(this double[,] a)
        {
            return SquareEuclidean(a, 0);
        }

        /// <summary>
        ///   Gets the Squared Euclidean norm vector for a matrix.
        /// </summary>
        static double[] SquareEuclidean(this double[,] a, int dimension)
        {
            int rows = a.GetLength(0);
            int cols = a.GetLength(1);
            
            double[] norm;

            if (dimension == 0)
            {
                norm = new double[cols];

                for (j = 0; j < norm.Length; j++)
                {
                    double sum = 0.0;
                    for (i = 0; i < rows; i++)
                    {
                        double v = a[i, j];
                        sum += v * v;
                    }
                    norm[j] = sum;
                }
            }
            else
            {
                norm = new double[rows];

                for (i = 0; i < norm.Length; i++)
                {
                    double sum = 0.0;
                    for (j = 0; j < cols; j++)
                    {
                        double v = a[i, j];
                        sum += v * v;
                    }
                    norm[i] = sum;
                }
            }

            return norm;
        }

        /// <summary>
        ///   Gets the Euclidean norm for a matrix.
        /// </summary>
        static double[] Euclidean(this double[,] a)
        {
            return Euclidean(a, 0);
        }

        /// <summary>
        ///   Gets the Euclidean norm for a matrix.
        /// </summary>
        static double[] Euclidean(this double[,] a, int dimension)
        {
            double[] norm = Norm.SquareEuclidean(a, dimension);

            for (i = 0; i < norm.Length; i++)
                norm[i] = sqrt(norm[i]);

            return norm;
        }

    }
}
