/*
 *	pi.c
 *
 *      Computation of pi by a "montecarlo" method:
 *      Every thread computes a number of points in the unit square
 *      and returns the number of hits in the quarter circle
 *      pi is  #hits/#total * 4
 *      
 *      usage:
 *		pi [total]
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define DEFAULT_ANZAHL 10000L       /* default value for total */
#define NUM_THREADS  4              /* number of worker threads */

long   mytotal;                     /* number of points per thread */

void 
main(int argc, char *argv[])
{

   int        i;
   int        nthreads;                      /* number of threads */
   long       total;                         /* total number of points */
   pthread_t  thread_id[NUM_THREADS];        /* ids of all threads */
   long       totalhits = 0;                 /* total number of hits */
   double     pi;                            /* result */
   void *     myhits_p;
   void *     calc(void *);                  /* calculation */

   nthreads = NUM_THREADS;   /* or command line arg or runtime function */

   /* compute number of points per thread */
   if (argc != 2) {
     total = DEFAULT_ANZAHL;
   }
   else {
     total = 1000 * atol(argv[1]);
   }
   mytotal = (long) ceil(total/nthreads);
   total = nthreads * mytotal;              /* correct total */
   
   /* start worker threads */
   for (i=0; i<nthreads; i++) {
     pthread_create(&thread_id[i], NULL, calc, NULL);
   }   

   /*  wait for worker threads and combine partial results */
   for (i=0; i<nthreads; i++) {
     pthread_join(thread_id[i], &myhits_p);
     totalhits +=  * (long *)myhits_p;
   }   
   
   /* compute final result */
   pi = totalhits/(double)total*4;
   printf("\nPI = %lf\n", pi);
}


#include "rand.h"

void * calc(void *args)
{
   /* 
    * compute total random points in the unit square
    * and return the number of hits in the sector (x*x + y*y < 1)
    *
    * uses global mytotal
    */

   double  x, y;                     /* random coordinates */
   long *  hits_p;                   /* pointer to number of hits */
   int     seed;                     /* seed for random generator */
   long    i;
   
   /* get memory for return value */
   hits_p = (long *) calloc(1, sizeof(long));        /* initialized  to 0 */

   /* initialize random generator (otherwise all return the same result!) */
   seed = (int) pthread_self();  /* !! ERROR, if pthread_t is a struct */
   for(i=0; i<mytotal; i++)
   {
      x = ((double) myrand_r(&seed))/RAND_R_MAX;
      y = ((double) myrand_r(&seed))/RAND_R_MAX;
      
      if ( x*x + y*y <= 1.0 )
      {
	 (*hits_p)++;
      }
   }

   return(hits_p); 
}
