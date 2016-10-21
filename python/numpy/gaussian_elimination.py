#!/usr/bin/python
import numpy as np

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
         l[i] = a[i][k] / a[k][k]
         for j in range(k, b.size):
	    a[i][j] = a[i][j] - l[i] * a[k][j]
         b[i] = b[i] - l[i] * b[k]
      print a
      print b

   if a[a.shape[0] - 1][a.shape[1] - 1] == 0:
      print "Sorry, you system either has many or none solution."
      return None

   for k in reversed(range(0, b.size)):
      _sum = 0.0
      for j in range(k+1, b.size):
         _sum = _sum + a[k][j] * x[j]
      x[k] = 1 / a[k][k] * (b[k] - _sum)

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

   _max = a[k][k]
   r = k
   for row in range(k, a.shape[0]):
      if a[row][k] > _max:
         _max = a[row][k]
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
   temp = b[r]
   b[r] = b[k]
   b[k] = temp

   # Exchange a
   temp_slice = a[r].copy()
   a[r] = a[k].copy()
   a[k] = temp_slice

   return

if __name__ == "__main__":
   A = np.array([2.0, 1, -1, 2, 4, 5, -3, 6, -2, 5, -2, 6, 4, 11, -4, 8]).reshape(4,4)
   b = np.array([5.0, 9, 4, 2])
   
   print "The Matrix:"
   print A
   print "The Vector:"
   print b
   x = gaussian_sequential(A, b)
   print x

   """
   A = np.array([2.0, 1, 3, 2, 6, 8, 6, 8, 18]).reshape(3,3)
   b = np.array([1.0, 3, 5])
   
   print A
   x = gaussian_sequential(A, b)
   print x

   A = np.array([3.0, 1, -6,
                 2, 1, -5, 
                 6, -3, 3 ]).reshape(3,3)

   b = np.array([-10.0, -8, 0])
   
   print A
   x = gaussian_sequential(A, b)
   print x
   """
