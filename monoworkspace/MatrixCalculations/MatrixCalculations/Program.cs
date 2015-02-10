using System;
using System.Runtime.Serialization;

namespace MatrixCalculations
{
	[Serializable()] 
	public class Matrix : ISerializable
	{
		private int rows;
		private int columns;
		private double [][] M;

		public Matrix(SerializationInfo info, StreamingContext ctxt)
		{
			rows = (int)info.GetValue ("rows", typeof(int));
			columns = (int)info.GetValue ("columns", typeof(int));

			M = new double[this.rows][];
			for (int i = 0; i < this.columns; i++) 
			{
				M[i] = new double[this.columns];
			}

			for (int i = 0; i < rows; i++)
				for (int j = 0; j < columns; j++)
					M[i][j] = (double) info.GetValue ("M" + i + "," + j, typeof(double));
		}

		public void GetObjectData(SerializationInfo info, StreamingContext ctxt)
		{
			info.AddValue("rows", rows);
			info.AddValue("columns", columns);

			for (int i = 0; i < rows; i++)
				for (int j = 0; j < columns; j++)
					info.AddValue ("M" + i + "," + j, M[i][j]);
		}

		public Matrix(int rows, int columns)
		{
			if ((rows <= 0) || (columns <= 0)) 
			{
				Console.WriteLine ("Matrix: Invalid value for column {0} or row {1}.", columns, rows);
				return;
			}
			this.rows = rows;
			this.columns = columns;
			M = new double[this.rows][];
			for (int i = 0; i < this.columns; i++) 
			{
				M[i] = new double[this.columns];
			}
		}

		public double this [int r, int c] 
		{
			get
			{
				return M [r] [c];
			}
			set 
			{
				M [r] [c] = value;
			}
		}

		public void printMatrix()
		{
			for (int i = 0; i < rows; i++) 
			{
				for (int j = 0; j < columns; j++) 
				{
					Console.Write ("{0} ", M[i][j]);
				}
				Console.WriteLine ("");
			}
		}

		public Matrix eliminateRowAndColumn(int row, int column)
		{
			if ((row < 0) || (row >= rows) || (column > columns) || (column < 0)) 
			{
				Console.WriteLine ("eliminateRowAndColumn:Invalid value for column {0} or row {1}.", column, row);
				return new Matrix (0, 0);
			}

			Matrix newMatrix = new Matrix (rows - 1, columns - 1);
			int newRows = 0;
			int newColumns = 0;
			for (int i = 0; i < rows; i++) 
			{
				if (i == row)
					continue;
				for (int j = 0; j < columns; j++) 
				{
					if (j == column)
						continue;
					// Console.WriteLine ("newsRows:{0} newColumns:{1} i:{2} j:{3}", newRows, newColumns, i, j);
					newMatrix [newRows, newColumns] = this[i, j];
					newColumns++;
				}
				newRows++;
				newColumns = 0;
			}
			return newMatrix;
		}
		
		public double determinant()
		{
			if (columns != rows) 
			{
				Console.WriteLine ("Cannot calculate determinant of non-square matrix.");
				return 0.0;
			}

			if (columns == 2) 
			{
				return this [0, 0] * this [1, 1] - this [1, 0] * this [0, 1];
			}
		
			double det = 0.0;
			for (int j = 0; j < columns; j++) 
			{
				Matrix cofactor = this.eliminateRowAndColumn (0, j);
				det += this[0,j] * Math.Pow (-1, j) * cofactor.determinant ();
			}

			return det;
		}

		public static double distributedDeterminant(Matrix m, ref string[] args)
		{
			double det = 0.0;
			using (new MPI.Environment(ref args)) 
			{
				MPI.Intracommunicator comm = MPI.Communicator.world;
				// Console.WriteLine("Hello, World! from rank " + MPI.Communicator.world.Rank + " (running on " + MPI.Environment.ProcessorName + ")");
				// Broadcast the Matrix
				comm.Broadcast<Matrix> (ref m, 0);

				// Each process calculate its share
				int myrank = comm.Rank;
				int nprocs = comm.Size;
				double localDet = 0.0;
				for (int i = myrank; i < m.columns; i += nprocs) 
				{
					// Console.WriteLine ("myrank:{0} i:{1}", myrank, i);
					Matrix cofactor = m.eliminateRowAndColumn (0, i);
					localDet += m [0, i] * Math.Pow (-1, i) * cofactor.determinant ();
				}
				// Reduce on determinant
				det = comm.Reduce(localDet, MPI.Operation<double>.Add, 0);
			}
			return det;
		}
	}

	class MainClass
	{
		public static void Main (string[] args)
		{
			int order = 12;
			Matrix A = new Matrix (order, order);
			for (int i = 0; i < order; i++)
				for (int j = 0; j < order; j++)
					A [i, j] = i * j + 1;

			/*
			A[0, 0] = 4; A[0, 1] = 5; A[0, 2] = -2;
			A[1, 0] = 7; A[1, 1] = -1; A[1, 2] = 2;
			A[2, 0] = 3; A[2, 1] = 1; A[2, 2] = 4;
			A.printMatrix ();
			Console.WriteLine("{0}", A.determinant ());
			*/

			Console.WriteLine("{0}", Matrix.distributedDeterminant (A, ref args));

		}
	}
}
