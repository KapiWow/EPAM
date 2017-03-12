#define _XOPEN_SOURCE 600
#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

int
main(int argc, char *argv[])
{
	srand(time(NULL));
	double * x = (double*) malloc(sizeof(double)*10000000);
	double  max, maxMain;
	double startwtime = 0.0, endwtime;
	int ProcRank, ProcNum, N=10000000, i=0;
	
	for (i = 0; i < N; i++)
	{
		x[i] = rand();
	}
	MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    
	int k = N/ProcNum;
	int i1 = k * ProcRank;
	int i2 = k * ( ProcRank + 1);

	startwtime = MPI_Wtime();
	max = x[0], maxMain = x[0];
	if (ProcRank == ProcNum-1) 
		i2 = N;
	for (i = i1; i < i2; i++)
		if (x[i] > max)
			max = x[i];
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
		printf("\nmax = %10.2f", maxMain);
		endwtime = MPI_Wtime();
		printf("\nSend time = %f", endwtime - startwtime);
	}
	
	startwtime = MPI_Wtime();
	max = x[0], maxMain = x[0];
	if (ProcRank == ProcNum-1) 
		i2 = N;
	for (i = i1; i < i2; i++)
		if (x[i] > max)
			max = x[i];
	MPI_Reduce(&max, &maxMain, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if (ProcRank == 0)
	{
		printf("\nmax = %10.2f", maxMain);
		endwtime = MPI_Wtime();
		printf("\nreduce time = %f", endwtime - startwtime);
	}

	startwtime = MPI_Wtime();
	max = x[0], maxMain = x[0];
	if (ProcRank == ProcNum-1) 
		i2 = N;
	for (i = i1; i < i2; i++)
		if (x[i] > max)
			max = x[i];
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
		MPI_Ssend(&max, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

	if (ProcRank == 0)
	{
		printf("\nmax = %10.2f", maxMain);
		endwtime = MPI_Wtime();
		printf("\nSsend time = %f", endwtime - startwtime);
	}
	
	MPI_Request Request;
	
	startwtime = MPI_Wtime();
	max = x[0], maxMain = x[0];
	if (ProcRank == ProcNum-1) 
		i2 = N;
	for (i = i1; i < i2; i++)
		if (x[i] > max)
			max = x[i];
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
		MPI_Isend(&max, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Request);

	if (ProcRank == 0)
	{
		printf("\nmax = %10.2f", maxMain);
		endwtime = MPI_Wtime();
		printf("\nIsend time = %f", endwtime - startwtime);
	}

	if (ProcRank == 0)
	{
		max = x[0];
		startwtime = MPI_Wtime();
		for (i = 1; i < N; i++)
		{
			if(x[i]>max)
				max = x[i];
		}
		endwtime = MPI_Wtime();
		printf("\nmax = %10.2f", max);
		printf("\nwall clock time = %f\n", endwtime - startwtime);
	}	
	MPI_Finalize();
	free(x);
    return 0;
}

