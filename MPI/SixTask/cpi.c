#define _XOPEN_SOURCE 600
#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
	srand(time(NULL));
	double * y = (double*) malloc(sizeof(double) * 1000000);
	double TotalSum, ProcSum = 0.0;
	double startwtime = 0.0, endwtime;
	int ProcRank, ProcNum, N = 1000000, i = 0, j = 0;
	double max, maxMain;
	MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	
    int k = N/ProcNum;
	double ** x = (double**)malloc(ProcNum * sizeof(double*));
	int last = N - (k * (ProcNum - 1));

	if (ProcRank == 0)
	{
		int * counts = (int*) malloc(sizeof(int) * ProcNum);


		for (i = 1; i < ProcNum; i++)
		{
 			x[i] = (double*)malloc(k * sizeof(double));
		}    
 		x[0] = (double*)malloc(last * sizeof(double));
		for (i = 1; i < ProcNum; i++)
		{
			for (j = 0; j < k; j++)
				x[i][j] = (double)rand()/INT_MAX;
		}

		for (j = 0; j < last; j++)
			x[0][j] = (double)rand()/INT_MAX;	
	}	
	
	startwtime = MPI_Wtime();
	
	double * z = (double*)malloc(k * sizeof(double*));	

	if (ProcRank == 0)
	{
		for (i = 1; i < ProcNum; i++)
			MPI_Send(x[i], k, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		max = x[0][0];
		for (i = 0; i < last; i++)
        	if (x[0][i] > max)
            	max = x[0][i];
	} else
	{
		MPI_Recv(z, k, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
		for (i = 0; i < k; i++)
        	if (z[i] > max)
            	max = z[i];
	}

	if ( ProcRank == 0)
	    {
	        maxMain = max;
	        for (i = 1; i < ProcNum; i++)
	        {
	            MPI_Recv(&max, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Status);
	            if (max > maxMain)
	                maxMain = max;
	        }
	    } else
	        MPI_Send(&max, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);


	if (ProcRank == 0)
	{
		printf("\nmax = %f", maxMain);
		endwtime = MPI_Wtime();
		printf("\nsendRecv = %f\n", endwtime - startwtime);
	}

	if (ProcRank == 0)
	{	
	    startwtime = MPI_Wtime();
	    max = x[0][0];
		for (i = 0; i < last; i++)
	        if (x[0][i] > max)
                max = x[0][i];
		for (i = 1; i < ProcNum; i++)
			for (j = 0; j < k; j ++)
				if (x[i][j] > max)
					max = x[i][j];
	
	    endwtime = MPI_Wtime();
	    printf("\nmax = %f", max);
	    printf("\nwall clock time = %f\n", endwtime - startwtime);
	}

	
	MPI_Finalize();
	//free(x);
	free(y);
    return 0;
}

