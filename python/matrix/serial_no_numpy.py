#!/usr/bin/python
import sys

# Read the Matrix Order from parameter
matrix_order = int(sys.argv[1])

# Read the files names
matrix_A_filename = sys.argv[2]
matrix_B_filename = sys.argv[3]
matrix_C_filename = sys.argv[4]

# Fill matrix_A from the first filename
matrix_A_file = open(matrix_A_filename, "r")
matrix_A = []
for line in matrix_A_file.readlines():
   matrix_A.append([float(n) for n in line.rstrip().split(" ")])
matrix_A_file.close()

# Fill matrix_B from the second filename
matrix_B_file = open(matrix_B_filename, "r")
matrix_B = []
for line in matrix_B_file.readlines():
   matrix_B.append([float(n) for n in line.rstrip().split(" ")])
matrix_B_file.close()

# Multiply matrix_A and matrix_B to matrix_C
matrix_C = [[0] * matrix_order for n in range(matrix_order)]
for i in range(matrix_order):
   for j in range(matrix_order):
      for k in range(matrix_order):
         matrix_C[i][j] += matrix_A[i][k] * matrix_B[k][j]

# Write matrix_C to the third filename
matrix_C_file = open(matrix_C_filename, "w")
for line in matrix_C:
   matrix_C_file.writelines([str(n) + " " for n in line])
   matrix_C_file.write('\n')
matrix_C_file.close()
