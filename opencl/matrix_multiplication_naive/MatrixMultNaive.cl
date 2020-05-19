__kernel void MatrixMultNaive(__global float* matrixA, __global float* matrixB, __global float* matrixC, int N) {
	
	// Achar os índices globais
	int xGID = get_global_id(0); // coluna  
	int yGID = get_global_id(1); // linha 

	float dotprod = 0.0;

	for (int i = 0; i < N; i++) {
		dotprod += matrixA[yGID * N + i] * matrixB[i * N + xGID];
	}

	matrixC[yGID * N + xGID] = dotprod;
}