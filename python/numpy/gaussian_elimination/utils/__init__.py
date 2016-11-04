import scipy.io as io
import numpy as np
from scipy.sparse import coo_matrix

def max_col_loc(p, rank, a, k):
   """
   Determines the row index r of the element a[r][k], which has the largest local absolute value in column k for the rows >= k.
   input: number of processesp, the process rank, matrix a, integer k
   output: integer r
   """
   assert(a.ndim == 2)
   assert(k >= 0 and k < a.shape[0])

   # Find out which is the first row greater than k assigned to the process
   _row = k
   while _row % p != rank :
      _row = _row + 1
  
   # In this scenario, there are no elements of column k for rows >= k
   if _row >= a.shape[0]:
      return -1

   # Still are elements from the column k left
   _max = abs(a[_row, k])
   r = _row
   
   # For each row of the matrix, starting from row k
   for row in range(k, a.shape[0]):
      # If this row is mapped to this process
      if row % p == rank:
         if abs(a[row, k]) > _max:
            _max = a[row, k]
            r = row
   return r

def max_col(a, k):
   """
   Returns the number of the row of the highest element from column k
   input: array a, column k
   output: r the row number
   """
   assert(a.ndim == 2)
   assert(k >= 0 and k < a.shape[0])

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
   assert(b.shape[0] == a.shape[1])
   assert(r >= 0 and r < a.shape[1])
   assert(k >= 0 and k < a.shape[1])
   
   # Exchange b
   temp = b[r].copy()
   b[r] = b[k].copy()
   b[k] = temp

   # Exchange a
   temp_slice = a[r].copy()
   a[r] = a[k].copy()
   a[k] = temp_slice

   return

def copy_row(a, b, k):
   """ 
   Joins together a row of a with an element of b and returns
   input: matrix a, array b and row k
   output: a[k] + b[k]
   """
   assert buf.shape[0] = a.shape[0] + b.shape[0]
   return numpy.append(a[k], b[k]) 

def copy_exchange_row(a, b, r, buf, k);
   """
   Exchanges the row r and the buffer buf. I don't know if I need k
   input: matrix a, vector b, index r, a vector buf and another index k
   output: none
   """
   # Exchange buf and row r and updating row k
   temp = buf.copy()
   for i in range(a.shape[0]):
      buf[i] = a[r,i]
      a[k,i] = a[r,i]
      a[r,i] = temp[i]
   buf[a.shape[0] + 1] = b[r]
   b[k] = b[r]
   b[r] = temp[a.shape[0] + 1]
   return

def copy_back_row(a, b, buf, k):
   """
   The content of buf is copied into row k 
   input: matrix a, array b, array buffer, index k
   output: none
   """
   for i in range(a.shape[0]):
      a[k,i] = buf[i]
   b[k] = buf[a.shape[0] + 1)   
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
   test_mtx_file = "data/teste/teste_unit.mtx"
   test_mtx_b_file = "data/teste/teste_unit_b.mtx"
   test_mtx  = load_mtx(test_mtx_file)
   test_mtx_b = load_mtx(test_mtx_b_file)
   print "Basic Matrix:"
   print test_mtx.toarray()
   print "Index of maximum element from the 2nd column:"
   print max_col(test_mtx, 1)
   print "Exchanging the first and the third row:"
   exchange_row(test_mtx, test_mtx_b, 0, 2)
   print test_mtx.toarray()
   exchange_row(test_mtx, test_mtx_b, 2, 0)
   print "With 2 processes, a process with rank 1, has for maximum value on column 1 the element at row:"
   print max_col_loc(2, 1, test_mtx, 1)


