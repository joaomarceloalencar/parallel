__kernel void VectorAdd(__global int* a, __global int* b, __global int* c, int iNumElements) {
	
	// Achar o índice global
	int iGID = get_global_id(0);

	if (iGID < iNumElements) {
		c[iGID] = a[iGID] + b[iGID];
	}
}