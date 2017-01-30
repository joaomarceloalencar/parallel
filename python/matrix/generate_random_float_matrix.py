#!/usr/bin/python
import sys
import random

matrix_order = int(sys.argv[1])
filename = sys.argv[2]

f = open(filename, "w+")

for i in range(matrix_order):
   for j in range(matrix_order):
      f.write(str("%.2f " % random.uniform(0.0, 100.0)))
   f.write("\n")

f.close()
