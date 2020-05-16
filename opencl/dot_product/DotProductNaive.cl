__kernel void DotProductNaive(__global float* a, 
							  __global float* b, 
							  __global float* c, 
							  int iNumElements) {
	
	// Achar o índice global
	int iGID = get_global_id(0);
	int index = iGID;
	
	while (index < iNumElements) {
		c[iGID] += a[index] * b[index];
		index += get_global_size(0);
	}
}