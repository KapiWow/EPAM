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
	double * x = (double*) malloc(sizeof(double)*1000000);
	double * y = (double*) malloc(sizeof(double)*1000000);
	double TotalSum, ProcSum = 0.0;
	double startwtime = 0.0, endwtime;
	int ProcRank, ProcNum, N = 1000000, i = 0;
	
	MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	
	startwtime = MPI_Wtime();
    int k = N/ProcNum;
    int i1 = k * ProcRank;
    int i2 = k * ( ProcRank + 1);
	if (ProcRank == 0)
	{
		
		for (i = 0; i < N; i++)
		{
			x[i] = (double)rand()/INT_MAX;
		}
	
	}	

	for (i = 0; i<100; i++)
	{
		if (ProcRank == 0)	
			MPI_Sendrecv(x, N, MPI_DOUBLE, 1, 0, y, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &Status);
		if (ProcRank == 1)	
			MPI_Sendrecv(y, N, MPI_DOUBLE, 0, 0, x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
	}

	if (ProcRank == 0)
	{
		endwtime = MPI_Wtime();
		printf("sendRecv = %f\n", endwtime - startwtime);
	}

	MPI_Finalize();
	free(x);
	free(y);
    return 0;
}

