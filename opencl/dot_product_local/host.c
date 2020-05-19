#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>

/* Só para saber se estou no Linux ou Windows. */
#ifdef __linux__
#define LINUX 1
#define WINDOWS 0
#elif  _WIN32
#define LINUX 0
#define WINDOWS 1
#endif


#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_TARGET_OPENCL_VERSION 200
#include <CL/cl.h>

cl_int status;
cl_int ciErr;
cl_platform_id *platforms = NULL;
cl_platform_id platform;
cl_uint numPlatforms = 0;
cl_device_id* devices = NULL;
cl_uint numDevices = 0;
char buffer[1000000];
cl_uint buf_uint;
cl_ulong buf_ulong;
size_t buf_sizet;
cl_int iNumElements = 512 * 512 * 256;
// ‭512 * 512 * 128 = 33.554.432‬

cl_float* srcA;
cl_float* srcB;
cl_float* srcC;
cl_double result;

FILE* programHandle; // Arquivo com funções kernel
size_t programSize;
char* programBuffer;
cl_program cpProgram; // Programa OpenCL
cl_kernel ckKernel;   // Kernel OpenCL

size_t szGlobalWorkSize; // global work size
size_t szLocalWorkSize; // local work size

// Função Main
// ***************************************************************************************
int main (int argc, char *argv[]) {
	// Verificar se são 2 parâmetros.
    if (argc != 3) {
		printf("Forneça dois parâmetros:\n");
		printf("<PLATAFORMA>  : INTEL, NVIDIA ou AMD para definir a plataforma.\n");
		printf("<DISPOSITIVO> :	GPU ou CPU para definir o dispositivo.\n");
		printf("%s <PLATAFORMA> <DISPOSITIVO>\n", argv[0]);
		exit(1);
	}

    // Qual plataforma o usuário deseja utiliza?
	char chosenPlatform[100];
    if (!strcmp(argv[1], "NVIDIA") || !strcmp(argv[1], "INTEL") || !strcmp(argv[1], "AMD") ) {
		strcpy(chosenPlatform, argv[1]);
	} else {
		printf("Plataforma Inválida.\n");
		exit(1);
	}

	// Dispositivo CPU ou GPU?
	char chosenDevice[100];
	if (!strcmp(argv[2], "CPU") || !strcmp(argv[2], "GPU")) {
		strcpy(chosenDevice, argv[2]);
	} else {
		printf("Dispositivo Inválido.\n");
		exit(1);
	}
    printf("Plataforma em procura: %s, Dispositivo em procura: %s.\n\n", chosenPlatform, chosenDevice);

	// Configurar as dimensões de trabalho Global e Local
	szLocalWorkSize = 256;
	szGlobalWorkSize = 512 * 256;

	// Alocar arrays no host
	srcA = (cl_float *) malloc(sizeof(cl_float) * iNumElements);
	srcB = (cl_float *) malloc(sizeof(cl_float) * iNumElements);
	srcC = (cl_float *) malloc(sizeof(cl_float) * szGlobalWorkSize/szLocalWorkSize);

	// Inicializar os arrays
	for (int i = 0; i < iNumElements; i++) {
		*(srcA + i) = 1.0;
		*(srcB + i) = 1.0;	
	}

	
	// ***************************************************************************************
	// Etapa 0: definir a plataforma 
	// ***************************************************************************************
	status = clGetPlatformIDs(0, 0, &numPlatforms);
	if (status != CL_SUCCESS) {
		printf("Erro: Falha ao recuperar a quantidade de plataformas.\n");
		return EXIT_FAILURE;
	}

	platforms = (cl_platform_id *) malloc(numPlatforms * sizeof(cl_platform_id));

	status = clGetPlatformIDs(numPlatforms, platforms, 0);
	if (status != CL_SUCCESS) {
		printf("Erro: Falha ao recuperar os dados das plataformas.\n");
		return EXIT_FAILURE;
	}

    // Se encontrar a plataforma o valor é 1, caso contrário 0.
    int matchPlatform = 0;
	for (cl_int i = 0; i < numPlatforms; i++) {
		size_t lengthNamePlatform;
		char *namePlatform;

		status = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, 0, &lengthNamePlatform);
		if (status != CL_SUCCESS) {
			printf("Erro: Falha ao recuperar o tamanho do nome da plataforma.\n");
			return EXIT_FAILURE;
		}

		namePlatform = (char *) malloc(lengthNamePlatform * sizeof(char));

		status = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, lengthNamePlatform, namePlatform, 0);
		if (status != CL_SUCCESS) {
			printf("Erro: Falha ao recuperar o nome da plataforma.\n");
			return EXIT_FAILURE;
		}
		printf("Plataforma encontrada: %s\n", namePlatform);

		if (strstr(namePlatform, "NVIDIA") != NULL) {
			if (!strcmp(chosenPlatform, "NVIDIA")) {
				printf("Escolhendo plataforma NVIDIA.\n");
				platform = platforms[i];
				matchPlatform = 1;
			}
		} else if (strstr(namePlatform, "Intel") != NULL) {
			if (!strcmp(chosenPlatform, "INTEL")) {
				printf("Escolhendo plataforma INTEL.\n");
				platform = platforms[i];
				matchPlatform = 1;
			}
		} else if (strstr(namePlatform, "Portable") != NULL) {
			if (!strcmp(chosenPlatform, "AMD")) {
				printf("Escolhendo plataforma AMD.\n");
				platform = platforms[i];
				matchPlatform = 1;
			}
		}

	    free(namePlatform);
	}
	
	if (matchPlatform) {
		printf("\nPlataforma procurada encontrada!\n");
	} else {
		printf("\nPlataforma procurada não encontrada!\n");
		exit(1);
	}


	// ***************************************************************************************
	// Etapa 1: descobrir e inicializar os dispositivos
	// ***************************************************************************************

	// Caso a opção fosse executar na CPU:
	if (!strcmp(chosenDevice,"CPU")) {
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 0, NULL, &numDevices);
	} else {
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices);
	}

	if (status != CL_SUCCESS) {
		printf("Erro: Falha ao recuperar a quantidade de dispositivos!\n");
		if (status == CL_INVALID_PLATFORM) printf("Plataforma Invalida.\n");
		if (status == CL_INVALID_DEVICE) printf("Dispositivo Invalido.\n");
		if (status == CL_INVALID_VALUE) printf("Valor Invalido.\n");
		if (status == CL_DEVICE_NOT_FOUND) printf("Dispositivo nao encontrado.\n");
		return EXIT_FAILURE;
	}

	printf("Quantidade de Dispositivos = %d\n", numDevices);

	// Alocar espaço suficiente para cada dispositivo.
	devices = (cl_device_id*) malloc(numDevices * sizeof(cl_device_id));

	// Preencher com informações dos dispositivos - Mesma função, parâmetros diferentes!!!
	// Caso a opção fosse executar na CPU:
	// status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, numDevices, devices, NULL); 
	if (!strcmp(chosenDevice,"CPU")) {
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, numDevices, devices, NULL);
	} else {
		status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, numDevices, devices, NULL);
	}
	if (status != CL_SUCCESS) {
		printf("Erro: Falha ao recuperar os dados dos dispositivos!\n");
		return EXIT_FAILURE;
	}

	// ***************************************************************************************
	// Etapa 2: Criar um contexto 
	// ***************************************************************************************

	cl_context context = NULL;

	// Criar um contexto e associá-lo aos dispositivos.
	context = clCreateContext(NULL, numDevices, devices, NULL, NULL, &status);

	if (!context) {
		printf("Erro: Falha ao criar contexto de computação!\n");
		return EXIT_FAILURE;
	}

	// ***************************************************************************************
	// Etapa 3: Criar uma filha de comandos
	// ***************************************************************************************
	
	cl_command_queue cmdQueue;

	// Criar uma fila e associá-la ao dispositivo. O dispositivo 1 deve ser a GPU.
	cmdQueue = clCreateCommandQueue(context, devices[0], CL_QUEUE_PROFILING_ENABLE, &status);

	if (!cmdQueue) {
		printf("Erro: falha a criar uma fila de comandos!\n");
		return EXIT_FAILURE;
	}

	// ***************************************************************************************
	// Etapa 4: Criar o objeto do programa para um contexto
	// ***************************************************************************************

	// 4.a: Ler o kernel OpenCL do arquivo e recuperar seu tamanho.
	if (WINDOWS) fopen_s(&programHandle, "DotProductNaive.cl", "r");             // Alterei para fopen_s
	if (LINUX) programHandle = fopen("DotProductNaive.cl", "r");
	fseek(programHandle, 0, SEEK_END);
	programSize = ftell(programHandle);
	rewind(programHandle);
	printf("Tamanho do programa = %zu B\n", programSize);

	// 4.b: Leia o código do kernel no buffer apropriado e marque seu final.
	programBuffer = (char*) malloc(programSize + 1);
	memset(programBuffer, ' ', programSize);                        // ATENÇÃo: o código do livro não tem esse trecho!
	fread(programBuffer, sizeof(char), programSize, programHandle); // Leio primeiro.
	programBuffer[programSize] = '\0';                              // Marco o final.
	fclose(programHandle);

	// 4.c: Criar o programa a partir da fonte.
	cpProgram = clCreateProgramWithSource(context, 1, (const char**)&programBuffer, &programSize, &ciErr);
	if (!cpProgram) {
		printf("Erro: falha ao criar o programa de computação!\n");
		return EXIT_FAILURE;
	}
	free(programBuffer);

	// ***************************************************************************************
	// Etapa 5: Compilar o Programa
	// ***************************************************************************************
	ciErr = clBuildProgram(cpProgram, 0, NULL, NULL, NULL, NULL);
	if (ciErr != CL_SUCCESS) {
		size_t len;
		char buffer[2048];

		printf("Erro: Falha ao construir o executável!\n");
		clGetProgramBuildInfo(cpProgram, devices[1], CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("%s\n", buffer);
		exit(1);
	}

	// ***************************************************************************************
	// Etapa 6: Criar os buffers no dispositivo - Aqui pode mudar de acordo com o kernel.
	// ***************************************************************************************
	
	cl_mem bufferA; // Array de entrada no dispositivo
	cl_mem bufferB; // Array de entrada no dispositivo
	cl_mem bufferC; // Array de saída no dispositivo

	// Tamanho dos dados
	size_t datasize = sizeof(cl_float) * iNumElements;
	size_t datasize_c = sizeof(cl_float) * (szGlobalWorkSize/szLocalWorkSize);
	
	// Aqui você está fazendo "malloc" no dispositivo
	bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, &status);
	if (status != CL_SUCCESS) {
		printf("Erro ao alocar vetor A no dispositivo.\n");
		exit(1);
	}

	// Aqui você está fazendo "malloc" no dispositivo
	bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, datasize, NULL, &status);
	if (status != CL_SUCCESS) {
		printf("Erro ao alocar vetor B no dispositivo.\n");
		exit(1);
	}

	// Aqui você está fazendo "malloc" no dispositivo
	bufferC = clCreateBuffer(context, CL_MEM_READ_WRITE, datasize_c, NULL, &status);
	if (status != CL_SUCCESS) {
		printf("Erro ao alocar vetor C no dispositivo.\n");
		exit(1);
	}


	// ***************************************************************************************
	// Etapa 7: Escrever dados do host para o dispositivo
	// ***************************************************************************************

	status = clEnqueueWriteBuffer(cmdQueue, bufferA, CL_FALSE, 0, datasize, srcA, 0, NULL, NULL);
	status = clEnqueueWriteBuffer(cmdQueue, bufferB, CL_FALSE, 0, datasize, srcB, 0, NULL, NULL);

	// ***************************************************************************************
	// Etapa 8: Criar o kernel a partir do código compilado
	// ***************************************************************************************

	ckKernel = clCreateKernel(cpProgram, "DotProductNaive", &ciErr);
	if (!ckKernel || ciErr != CL_SUCCESS) {
		printf("Erro: Falha ao criar o kernel!\n");
		exit(1);
	}

	// ***************************************************************************************
	// Etapa 9: Configure os argumentos do kernel
	// ***************************************************************************************
	
	ciErr = clSetKernelArg(ckKernel, 0, sizeof(cl_mem), (void*)&bufferA);
	ciErr |= clSetKernelArg(ckKernel, 1, sizeof(cl_mem), (void*)&bufferB);
	ciErr |= clSetKernelArg(ckKernel, 2, sizeof(cl_mem), (void*)&bufferC);
	ciErr |= clSetKernelArg(ckKernel, 3, sizeof(float) * szLocalWorkSize, NULL);
	ciErr |= clSetKernelArg(ckKernel, 4, sizeof(cl_int), (void*)&iNumElements);

	if (ciErr != CL_SUCCESS) {
		printf("Erro ao configurar os argumentos do Kernel.\n");
		exit(1);
	}

	// ***************************************************************************************
	// Etapa 10: Enfileirar o Kernel para execução
	// ***************************************************************************************

    cl_event kernelEvent;
	ciErr = clEnqueueNDRangeKernel(cmdQueue, ckKernel, 1, NULL, &szGlobalWorkSize, &szLocalWorkSize, 0, NULL, &kernelEvent);
	if (ciErr != CL_SUCCESS) {
		printf("Erro lançando o kernel!\n");
		exit(1);
	}

	// Aguarda a execução.
	ciErr = clWaitForEvents(1, &kernelEvent);
	if (ciErr != CL_SUCCESS) {
		printf("Erro na execução do Kernel.\n");
		exit(1);
	}

	unsigned long start = 0;
	unsigned long end = 0;

	clGetEventProfilingInfo(kernelEvent, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
	clGetEventProfilingInfo(kernelEvent, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
	float duration = (end - start) * 10e-9;

	clReleaseEvent(kernelEvent);
	printf("Tempo de Execução do Kernel = %f s\n", duration);


    clFinish(cmdQueue);
	// ***************************************************************************************
	// Etapa 11: Ler o resultado de volta para o host
	// ***************************************************************************************
	
	ciErr = clEnqueueReadBuffer(cmdQueue, bufferC, CL_TRUE, 0, datasize_c, srcC, 0, NULL, NULL);
	if (ciErr != CL_SUCCESS) {
		printf("Erro ao recuperar o vetor C.\n");
		switch (ciErr) 	{
		case CL_INVALID_COMMAND_QUEUE:
			printf("CL_INVALID_COMMAND_QUEUE\n");
			break;
		case CL_INVALID_CONTEXT: 
			printf("CL_INVALID_CONTEXT\n");
			break;
		case CL_INVALID_MEM_OBJECT:
			printf("CL_INVALID_MEM_OBJECT\n");
			break;	
		case CL_INVALID_VALUE:
			printf("CL_INVALID_VALUE\n");
			break;
		case CL_INVALID_EVENT_WAIT_LIST:
			printf("CL_INVALID_EVENT_WAIT_LIST\n");
			break;
		case CL_MISALIGNED_SUB_BUFFER_OFFSET:
			printf("CL_MISALIGNED_SUB_BUFFER_OFFSET\n");
			break;		
		case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
			printf("CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST\n");
			break;						
		case CL_MEM_OBJECT_ALLOCATION_FAILURE:
			printf("CL_MEM_OBJECT_ALLOCATION_FAILURE");
			break;									
		case CL_INVALID_OPERATION:
			printf("CL_INVALID_OPERATION\n");
			break;		
		case CL_OUT_OF_RESOURCES:
			printf("CL_OUT_OF_RESOURCES\n");
			break;							
		case CL_OUT_OF_HOST_MEMORY:
			printf("CL_OUT_OF_HOST_MEMORY\n");
			break;					
		default:
			printf("Nenhum valor?\n");
			break;
		}
		exit(1);
	}

	// Aguarda a cópia
	clFinish(cmdQueue);

  	// Verifica o resultado
	result = 0.0;
	int i;
	for (i = 0; i < (szGlobalWorkSize/szLocalWorkSize); i++) {
		result += srcC[i];
	}

	printf("iNumElements         : %d\n", iNumElements);
	printf("2 * iNumElements     : %d\n", 2 * iNumElements);
	printf("szGlobalWorkSize     : %zd\n", szGlobalWorkSize);
	printf("Result               : %.2f\n", result);
	
	// Limpar
	free(srcA);
	free(srcB);
	free(srcC);

	if (ckKernel) clReleaseKernel(ckKernel);
	if (cpProgram) clReleaseProgram(cpProgram);
	if (cmdQueue) clReleaseCommandQueue(cmdQueue);
	if (context) clReleaseContext(context);

	if (bufferA) clReleaseMemObject(bufferA);
	if (bufferB) clReleaseMemObject(bufferB);
	if (bufferC) clReleaseMemObject(bufferC);

	return 0;
}
