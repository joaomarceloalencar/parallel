#!/usr/bin/python
# This is a serial version of Gaussian Elimination. It uses NumPy arrays and SciPy to read matrices from files in the mtx format.
import numpy as np
import scipy.io as io
from scipy.sparse import coo_matrix

def gaussian_sequential(a, b):
   """
   Solves the linear system [a] * [x] = [b]
   input: arrays a and b
   output: solution x
   """
   x = np.empty(b.size)
   l = np.empty(b.size)
   for k in range(b.size - 1):
      r = max_col(a, k)
      if (k != r):
         exchange_row(a, b, r, k)
      for i in range(k + 1, b.size):
         l[i] = a[i, k] / a[k, k]
         for j in range(k, b.size):
	    a[i, j] = a[i, j] - l[i] * a[k, j]
         b[i, 0] = b[i, 0] - l[i] * b[k, 0]

   if a[a.shape[0] - 1, a.shape[1] - 1] == 0:
      print "Sorry, you system either has many or none solution."
      return None

   for k in reversed(range(0, b.size)):
      _sum = 0.0
      for j in range(k+1, b.size):
         _sum = _sum + a[k, j] * x[j]
      x[k] = 1 / a[k, k] * (b[k,0] - _sum)

   return x

def max_col(a, k):
   """
   Returns the number of the row of the highest element from column k
   input: array a, column k
   output: r the row number
   """
   if a.ndim != 2:
      print "You need to provide a 2 dimensional array."
      return None
   elif k < 0 or k >= a.shape[0] :
      print "k is out of bound"
      return None

   _max = a[k,k]
   r = k
   for row in range(k, a.shape[0]):
      if a[row,k] > _max:
         _max = a[row,k]
	 r = row
   return r

def exchange_row(a, b, r, k):
   """
   Exchanges the rows r e k of array a and the corresponding elements of b
   input: array a, array b, row r, row k
   output: none
   """
   if b.shape[0] != a.shape[1] :
      print "Incompatible arrays and b."
      return None   
   elif (r < 0 or r >= a.shape[1]) or (k < 0 or k >= a.shape[1]):
      print "r or k out of the bounds of a."
      return None
   
   # Exchange b
   temp = b[r].copy()
   b[r] = b[k].copy()
   b[k] = temp

   # Exchange a
   temp_slice = a[r].copy()
   a[r] = a[k].copy()
   a[k] = temp_slice

   return

def load_mtx(_file):
   """
   Loads a matrix from .mtx format to a numpy array 
   Input: string _file 
   Output: a ndarray
   """
   matrix_raw = io.mmread(_file)
   if type(matrix_raw) == coo_matrix:
      mtx = matrix_raw.tocsr()
      return mtx
   return matrix_raw   

if __name__ == "__main__":
#   print "Pres_Poisson_b.mtx" 
#   mtx = load_mtx("Pres_Poisson/teste_b.mtx")
#   print mtx.size
#   print mtx.ndim
#   print mtx.shape[1]
#   print mtx[2,0]
   
   mtx = load_mtx("data/dezpordez.mtx")
   print "The Matrix:"
   print mtx.toarray()
#   print "The Maximum Value at Column 3 is at Row:"
#   print max_col(mtx, 3)
#   print "Exchaging rows 1 and 2"
   mtx_b = load_mtx("data/dezpordez_b.mtx")
#   print mtx_b
#   exchange_row(mtx, mtx_b, 0, 1)
#   print mtx
   print mtx_b
   print "The Solution is:"
   x = gaussian_sequential(mtx, mtx_b)
   print x
