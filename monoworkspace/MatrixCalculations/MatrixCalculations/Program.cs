using System;

namespace MatrixCalculations
{
	public class Matrix 
	{
		private int rows;
		private int columns;
		private double [][] M;

		public Matrix(int rows, int columns)
		{
			if ((rows <= 0) || (columns <= 0)) 
			{
				Console.WriteLine ("Invalid value for column or row.");
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
		}

		public Matrix eliminateRowAndColumn(int row, int column)
		{
			if ((row == 0) || (row >= rows) || (column > columns) || (column == 0)) 
			{
				Console.WriteLine ("Invalid value for column or row.");
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
					newMatrix [newRows, newColumns] = this.M [i, j];
					newColumns++;
				}
				newRows++;
			}
			return newMatrix;
		}

	}
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.WriteLine ("Hello World!");
			Matrix m = new Matrix (5, 5);
			m [0, 0] = 5.0;
			Console.WriteLine ("{0}", m [0, 0]);
		}
	}
}
