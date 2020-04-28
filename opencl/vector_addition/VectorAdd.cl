__kernel void VectorAdd(__global float* a, __global float* b, __global float* c, int iNumElements) {
	
	// Achar o índice global
	int iGID = get_global_id(0);

	if (iGID < iNumElements) {
		c[iGID] = a[iGID] + b[iGID];
	}
}