import scipy.io as io
from scipy.sparse import coo_matrix

def max_col_loc(p, rank, a, k):
   """
   Determines the row index r of the element a[r][k], which has the largest local absolute value in column k for the rows >= k.
   input: number of processesp, the process rank, matrix a, integer k
   output: integer r
   """
   if a.ndim != 2:
      print "You need to provide a 2 dimensional array."
      return None
   elif k < 0 or k >= a.shape[0] :
      print "k is out of bound"
      return None

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


