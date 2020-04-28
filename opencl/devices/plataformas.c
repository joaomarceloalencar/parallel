#include <stdio.h>
#include <CL/cl.h>
#include <stdlib.h>
#include <string.h>

void error(cl_int status, const char *s){
    if (status) {
        printf("%s\n", s);
        exit(1);
    }
}


cl_int status;
cl_platform_id intel_platform_id;

int main(int argc, char *argv[]) {
    cl_uint number_of_platforms;

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
    return 0;
}