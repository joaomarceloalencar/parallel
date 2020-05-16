__kernel void DotProductNaive(__global float* a, 
							  __global float* b, 
							  __global float* c, 
							  __local* ProductsWG,
							  int iNumElements) {
	
	// Recuperar o índice global
	int iGID = get_global_id(0);

    // Recuperar o índice local 
	int iLID = get_local_id(0);

    // Recuperar o índice do grupo
	int iWGID = get_group_id(0);

	// Recuperar o tamanho do grupo
	int iWGS = get_local_size(0);

	float temp = 0.0;
	while (iGID < iNumElements) {
		// Multiplica elementos adjacentes
		temp += a[iGID] * b[iGID];
		iGID += get_global_size(0);
	}
	
	// Armazenar o produto
	ProductsWG[iLID] = temp;

	// Esperar por todos os threads no grupo
	barrier(CLK_LOCAL_MEM_FENCE);

	// Redução
    int i = iWGS / 2;
	while (i != 0) {
		if (iLID < i) {
			ProductsWG[iLID] += ProductsWG[iLID+i];
		}
		barrier(CLK_LOCAL_MEM_FENCE);
		i = i / 2;
	}

	// Armazenar os resultados parciais do produto na memória global
	if (iLID == 0) {
		c[iWGID] = ProductsWG[0];
	}

} 