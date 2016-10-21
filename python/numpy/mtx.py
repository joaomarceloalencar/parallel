#!/usr/bin/python
import numpy as np


def load_mtx(_file):
   """
   Loads a matrix from .mtx format to a numpy arry 
   Input: string _file 
   Output: numpy array
   """
   matrix_file = open(_file, 'r')
   mtx = np.array([])
   line = "empty"
   dims = []
   while line != "":
      if line.startswith("%"):
         continue 
      if len(dims) == 0:
         # Deal with the dimensions line
	 dims = [ int(d) for d in line.split() ]
      elif len(dims) == 2:
         # it is a one dimensional array
	 2
      elif len(dims) == 3:
         # it is a two dimensional array
          	 
      line = matrix_file.readline()
	 
   matrx_file.close()

if __name__="__main__":

