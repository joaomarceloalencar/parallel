
#define TILE_WIDTH 16

__kernel void MatrixMultTiled(__global float* matrixA, __global float* matrixB, __global float* matrixC, int N) {
	
    // memória local para encaixar as partições
	__local float matrixAsub[TILE_WIDTH][TILE_WIDTH];
	__local float matrixBsub[TILE_WIDTH][TILE_WIDTH];

    // Índice global do thread
	int xGID = get_global_id(0); // coluna em NDRange
	int yGID = get_global_id(1); // linha em NDRange

	// Índice local do thread
	int xLID = get_local_id(0);  // coluna na partição
	int yLID = get_local_id(1);  // linha na partição

	float dotprod = 0.0;

	for (int tile = 0; tile < N/TILE_WIDTH; tile++) {
		// Cada work-item irá carregar um elemento da matriz para a memória
		// Carregando a partição de A para a memória local
		matrixAsub[yLID][xLID] = matrixA[yGID * N + (xLID + tile * TILE_WIDTH)];
		// Carregando a partição de B para a memória local
		matrixBsub[yLID][xLID] = matrixB[(yLID + tile *TILE_WIDTH) * N + xGID];

		// Sincronizar para ter certeza que as partições estão carregadas
		barrier(CLK_LOCAL_MEM_FENCE);

		for (int i = 0; i < TILE_WIDTH; i++) {
			dotprod += matrixAsub[yLID][i] * matrixBsub[i][xLID];
		}

		barrier(CLK_LOCAL_MEM_FENCE);
	}

	matrixC[yGID * N + xGID] = dotprod;
	
}