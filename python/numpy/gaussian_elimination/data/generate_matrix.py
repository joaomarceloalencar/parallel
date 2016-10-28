#!/usr/bin/python
import random

matrix_filename = "dezpordez"
matrix_size = 10

_file = open(matrix_filename + ".mtx", "w")
_file_b = open(matrix_filename + "_b.mtx", "w")

_file.write("%%MatrixMarket matrix coordinate real general\n")
_file.write("%s-------------------------------------------------------------------------------\n")
header = str(matrix_size) + " " + str(matrix_size) + " " + str(matrix_size * matrix_size) + "\n"
_file.write(header)

_file_b.write("%%MatrixMarket matrix array real general\n")
_file_b.write("%s-------------------------------------------------------------------------------\n")
header = str(matrix_size) + " 1\n"
_file_b.write(header)

for i in range(matrix_size):
   _file_b.write(str(random.randint(1,99)) + "\n")
   for j in range(matrix_size):
      _file.write(str(i + 1) + " " + str(j + 1) + " " + str(random.randint(1,100)) + "\n")

_file.close()
_file_b.close()
