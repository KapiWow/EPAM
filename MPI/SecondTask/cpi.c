#define _XOPEN_SOURCE 600
#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	srand(time(NULL));
	double * x = (double*) malloc(sizeof(double)*10000000);
	double * y = (double*) malloc(sizeof(double)*10000000);
	double TotalSum, ProcSum = 0.0;
	double startwtime = 0.0, endwtime;
	int ProcRank, ProcNum, N=10000000, i=0;
	
	for (i = 0; i < N; i++)
	{
		x[i] = (double)rand()/INT_MAX;
		y[i] = (double)rand()/INT_MAX;
	}
	MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    
	int k = N/ProcNum;
	int i1 = k * ProcRank;
	int i2 = k * ( ProcRank + 1);

	startwtime = MPI_Wtime();
	TotalSum = 0, ProcSum = 0;
	if (ProcRank == ProcNum-1) 
		i2 = N;
	for (i = i1; i < i2; i++)
		ProcSum += x[i]*y[i];

	if ( ProcRank == 0)
	{
		TotalSum = ProcSum;
		for (i = 1; i < ProcNum; i++)
		{
			MPI_Recv(&ProcSum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Status);
			TotalSum += ProcSum;	
		}
	} else
		MPI_Send(&ProcSum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		
	if (ProcRank == 0)
	{
		printf("\nsum = %10.5f", TotalSum);
		endwtime = MPI_Wtime();
		printf("\nparallel send time = %f", endwtime - startwtime);
	}

	startwtime = MPI_Wtime();
	TotalSum = 0, ProcSum = 0;
	if (ProcRank == ProcNum-1) 
		i2 = N;
	for (i = i1; i < i2; i++)
		ProcSum += x[i]*y[i];

	if ( ProcRank == 0)
	{
		TotalSum = ProcSum;
		for (i = 1; i < ProcNum; i++)
		{
			MPI_Recv(&ProcSum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Status);
			TotalSum += ProcSum;	
		}
	} else
		MPI_Ssend(&ProcSum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		
	if (ProcRank == 0)
	{
		printf("\nsum = %10.5f", TotalSum);
		endwtime = MPI_Wtime();
		printf("\nparallel ssend time = %f", endwtime - startwtime);
	}

	startwtime = MPI_Wtime();
	TotalSum = 0, ProcSum = 0;
	if (ProcRank == ProcNum-1) 
		i2 = N;
	for (i = i1; i < i2; i++)
		ProcSum += x[i]*y[i];
	MPI_Reduce(&ProcSum, &TotalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 
		
	if (ProcRank == 0)
	{
		printf("\nsum = %10.5f", TotalSum);
		endwtime = MPI_Wtime();
		printf("\nreduce time = %f", endwtime - startwtime);
	}


	if (ProcRank == 0)
	{
		TotalSum = 0.0;
		startwtime = MPI_Wtime();
		for (i = 0; i < N; i++)
		{
			TotalSum += x[i]*y[i];
		}
		endwtime = MPI_Wtime();
		printf("\nsum = %10.5f", TotalSum);
		printf("\nwall clock time = %f\n", endwtime - startwtime);
	}	


	MPI_Finalize();
	free(x);
	free(y);
    return 0;
}

