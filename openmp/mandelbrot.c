#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int max_iters;
    int i_size, j_size;
    double min_re, min_im, d_re, d_im;

    sscanf(argv[1], "%d", &max_iters);
    sscanf(argv[2], "%d", &i_size);
    sscanf(argv[3], "%d", &j_size);
    sscanf(argv[4], "%lf", &min_re);
    sscanf(argv[5], "%lf", &min_im);
    sscanf(argv[6], "%lf", &d_re);
    sscanf(argv[7], "%lf", &d_im);
  
    int **picture = (int **) malloc(i_size * sizeof(int *));
    for (int k = 0 ; k < i_size; k++) 
        picture[k] = (int *) malloc(j_size * sizeof(int));

    #pragma omp parallel for collapse(2) schedule(runtime)
    for (int i = 0; i < i_size; i++)  {
        for (int j = 0; j < j_size; j++)  {
            // printf("# (%d, %d) t=%d\n", i, j, omp_get_thread_num());
            double c_re = min_re + i * d_re;
            double c_im = min_im + j * d_im;

            double z_re = 0.0;
            double z_im = 0.0;
            int iters = 0;
            while ((z_re * z_re + z_im * z_im < 4.0) && (iters < max_iters))
            {
                double new_z_re = z_re * z_re - z_im * z_im + c_re;
                double new_z_im = 2 * z_re * z_im + c_im;
                z_re = new_z_re;
                z_im = new_z_im;
                iters = iters + 1;
            }
            picture[i][j] = iters;
        }
    }

    for (int k = 0 ; k < i_size; k++) 
        free(picture[k]);
    free(picture);

    return 0;
}
