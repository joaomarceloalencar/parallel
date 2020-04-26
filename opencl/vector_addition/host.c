#include <stdio.h>
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_TARGET_OPENCL_VERSION 200
#include <CL/cl.h>
#include <stdlib.h>
#include <string.h>

#define TAM 1024

void error(cl_int status, const char *s){
    if (status != CL_SUCCESS) {
        printf("%s\n", s);
        exit(1);
    }
}

void errorsw(cl_int status) {
    switch(status) {
        case CL_SUCCESS: printf("CL_SUCCESS\n"); break;;
        case CL_INVALID_PROGRAM: printf("CL_INVALID_PROGRAM\n"); break;;
        case CL_INVALID_VALUE: printf("CL_INVALID_VALUE\n"); break;;
        case CL_INVALID_DEVICE: printf("CL_INVALID_DEVICE\n"); break;;
        case CL_INVALID_BINARY: printf("CL_INVALID_BINARY\n"); break;;
        case CL_INVALID_BUILD_OPTIONS: printf("CL_INVALID_BUILD_OPTIONS\n"); break;;
        case CL_INVALID_OPERATION: printf("CL_INVALID_OPERATION\n"); break;;
        case CL_COMPILER_NOT_AVAILABLE: printf("CL_COMPILER_NOT_AVAILABLE\n"); break;;
        case CL_BUILD_PROGRAM_FAILURE: printf("CL_BUILD_PROGRAM_FAILURE\n"); break;;
        case CL_OUT_OF_HOST_MEMORY: printf("CL_OUT_OF_HOST_MEMORY\n"); break;;
        default: printf("Um dos outros.\n");
    }

    return;
}

cl_int status;
cl_uint number_of_platforms;
cl_platform_id intel_platform_id;
cl_device_id intel_gpu;
cl_context context;
cl_command_queue queue;
cl_program program;
FILE *kernel_source;
cl_kernel kernel;

size_t globalSize = TAM;

int main(int argc, char *argv[]) {

    // ***************************************************************************************
    // Etapa 0: Recuperar informações das Plataformas.
    // ***************************************************************************************

    // Recupera a quantidade de plataformas.
    status = clGetPlatformIDs(0, 0, &number_of_platforms);
    error(status, "Erro ao recuperar o número de plataformas.");
    printf("Quantidade de Plataformas: %d\n", number_of_platforms);

    // Recupera os identificadores de plataformas.
    cl_platform_id *platforms = (cl_platform_id *) malloc(number_of_platforms * sizeof(cl_platform_id));
    status = clGetPlatformIDs(number_of_platforms, platforms, 0);
    error(status, "Erro ao recuperar as plataformas.");
    
    // Imprime o nome das plataformas
    for (cl_uint i = 0; i < number_of_platforms; i++) {
        size_t platform_name_length; 

        // Recupera o tamanho do nome e aloca a cadeia de caracteres.
        status = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 0, 0, &platform_name_length);
        error(status, "Erro ao recuperar o tamanho do nome da plataforma.");
        char *platform_name = (char *) malloc(platform_name_length * sizeof(char));

        // Recupera o nome
        status = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, platform_name_length, platform_name, 0);
        error(status, "Erro ao recuperar o nome da plataforma.");

        // Imprime o resultado.
        printf("Plataforma[%d]: %s\n", i, platform_name);
        if (strstr(platform_name, "Intel") != NULL) {
            intel_platform_id = platforms[i];
        }
        free(platform_name);
    }
    printf("Vamos usar a plataforma Intel.\n");

    // Exibir as CPUs disponíveis
    cl_int cpu_count;
    status = clGetDeviceIDs(intel_platform_id, CL_DEVICE_TYPE_CPU, 0, 0, &cpu_count);
    error(status, "Erro ao recuperar a quantidade de CPUS.");
    printf("CPUs: %d\n", cpu_count);

    // Exibir as GPUs disponíveis
    cl_int gpu_count;
    status = clGetDeviceIDs(intel_platform_id, CL_DEVICE_TYPE_GPU, 0, 0, &gpu_count);
    error(status, "Erro ao recuperar a quantidade de GPUS");
    printf("GPUs: %d\n", gpu_count);
    free(platforms);   
 
    // ***************************************************************************************
    // Etapa 1: Recuperar o dispositivo desejado.
    // ***************************************************************************************
    
    // Queremos utilizar a primeira GPU

    status = clGetDeviceIDs(intel_platform_id, CL_DEVICE_TYPE_GPU, 1, &intel_gpu, NULL);
    error(status, "Erro ao recuperar o dispositivo Intel GPU");


    // ***************************************************************************************
    // Etapa 2: Criar o contexto.
    // ***************************************************************************************
    context = clCreateContext(0, 1, &intel_gpu, NULL, NULL, &status);
    error(status, "Erro ao criar o contexto.\n");
    
    // ***************************************************************************************
    // Etapa 3: Criar fila de comandos.
    // ***************************************************************************************
    queue = clCreateCommandQueue(context, intel_gpu, 0, &status);
    error(status, "Erro ao criar a fila de comandos.\n");

    // ***************************************************************************************
    // Etapa 4: Carregar código do Kernel.
    // ***************************************************************************************
    
    // Descobrir o tamanho do arquivo.
    errno_t file_status = fopen_s(&kernel_source, "vector_addition.cl", "r");
    if (file_status) {
        printf("Problema ao abrir o fonte do kernel.\n");
        exit(1);
    }

    fseek(kernel_source, 0, SEEK_END);
    size_t kernel_size = ftell(kernel_source);
    rewind(kernel_source);
    printf("Tamanho do programa = %zu B\n", kernel_size);

    char *kernel_buffer = (char *) malloc((kernel_size+1)*sizeof(char));
    memset(kernel_buffer, ' ', kernel_size);
    fread(kernel_buffer, sizeof(char), kernel_size, kernel_source);
    kernel_buffer[kernel_size] = '\0';
    fclose(kernel_source);

    printf("%s\n --- \n", kernel_buffer);

    // program = clCreateProgramWithSource(context, 1, (const char **) &kernel_buffer, &kernel_size, &status);
    program = clCreateProgramWithSource(context, 1, (const char **) &kernel_buffer, NULL, &status);
    error(status, "Erro ao criar o programa a partir do fonte do kernel.");

    status = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    errorsw(status);
    
    /*
    size_t log;
    clGetProgramBuildInfo(program, NULL, CL_PROGRAM_BUILD_LOG, 0, NULL, &log);
    char *buildlog = malloc(log * sizeof(char));
    clGetProgramBuildInfo(program, NULL, CL_PROGRAM_BUILD_LOG, log, buildlog, NULL);
    printf(buildlog);
    */

    // error(status, "Erro ao construir o kernel.");
    free(kernel_buffer);

    kernel = clCreateKernel(program, "VectorAdd", &status);
    error(status, "Erro ao criar o objeto kernel.");

    int *vectorA = (int *) malloc(TAM * sizeof(int));
    int *vectorB = (int *) malloc(TAM * sizeof(int));
    int *vectorC = (int *) malloc(TAM * sizeof(int));

    for (int i = 0; i < TAM; i++)
        vectorA[i] = vectorB[i] = 1;

    cl_mem bufA = clCreateBuffer(context, CL_MEM_READ_ONLY, TAM * sizeof(int), NULL, &status);
    error(status, "Problema ao criar o Buffer A.");
     
    cl_mem bufB = clCreateBuffer(context, CL_MEM_READ_ONLY, TAM * sizeof(int), NULL, &status);
    error(status, "Problema ao criar o Buffer B.");

    cl_mem bufC = clCreateBuffer(context, CL_MEM_READ_WRITE, TAM * sizeof(int), NULL, &status);
    error(status, "Problema ao criar o Buffer B.");

    status = clEnqueueWriteBuffer(queue, bufA, CL_TRUE, 0, TAM * sizeof(int), vectorA, 0, NULL, NULL);   
    error(status, "Erro ao copiar o vetor A.");
    
    status = clEnqueueWriteBuffer(queue, bufB, CL_TRUE, 0, TAM * sizeof(int), vectorB, 0, NULL, NULL);
    error(status, "Erro ao copiar o vetor B.");
    
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufC);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), &globalSize);

    status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);
    error(status, "Erro ao executar o kernel.");

    clFinish(queue);

    status = clEnqueueReadBuffer(queue, bufC, CL_TRUE, 0, TAM*sizeof(int), vectorC, 0, NULL, NULL);
    error(status, "Erro ao ler o buffer C do dispositivo.");

    for (int i = 0; i < 5; i++)
        printf("%d + %d = %d\n", vectorA[i], vectorB[i], vectorC[i]);

    clReleaseMemObject(bufA);
    clReleaseMemObject(bufB);
    clReleaseMemObject(bufC);

    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    free(vectorA);
    free(vectorB);
    free(vectorC);
    return 0;

}