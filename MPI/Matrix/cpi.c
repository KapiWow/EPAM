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
	int ProcRank, ProcNum, N = 500, i = 0, j = 0, k = 0;
	double max, maxMain;
	double ** z, ** x, ** a, **line, **colomn, **rez;
	MPI_Status Status;
	MPI_Request Request;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	
	int count = (N/ProcNum);
	if (ProcRank == 0)
	{
		x = (double**)malloc(N * sizeof(double*));
		z = (double**)malloc(N * sizeof(double*));
		a = (double**)malloc(N * sizeof(double*));
		for (i = 0; i < N; i++)
		{
			x[i] = (double*)malloc(N * sizeof(double));
			z[i] = (double*)malloc(N * sizeof(double));
			a[i] = (double*)malloc(N * sizeof(double));
			for (j = 0; j < N; j++)
			{
				 x[i][j] = (double)rand()/INT_MAX;
				 z[i][j] = (double)rand()/INT_MAX;
			}
		}
	} 
	
	line = (double**)malloc(count * sizeof(double*));
	colomn = (double**)malloc(count * sizeof(double*));
	rez = (double**)malloc(count * sizeof(double*));
	for (i = 0; i < count; i++)
	{
		rez[i] = (double*)malloc(N * sizeof(double));
		line[i] = (double*)malloc(N * sizeof(double));
		colomn[i] = (double*)malloc(N * sizeof(double));
	}
	
	if (ProcRank == 0)
	{
		startwtime = MPI_Wtime();
		for (i = 1; i < ProcNum; i++)
			for (j = 0; j < count; j++)
			{
				MPI_Isend(x[i * count + j], N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &Request);
				MPI_Isend(z[i * count + j], N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &Request);
			}
	} else
	{
		for (i = 0; i < count; i++)
		{
			MPI_Recv(line[i], N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
			MPI_Recv(colomn[i], N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
		}	
	}
	
	int num = 0;
	while (num < ProcNum)
	{
		for (i = 0; i < count; i++)
			for ( j = 0; j < count; j++)
			{
				double sum = 0;
				for (k = 0; k < N; k++)
					sum += line[i][k] * colomn[j][k];
				rez[i][num * count + j] = sum;
			}
		for (i = 0; i < count; i++)
			MPI_Isend(colomn[i], N, MPI_DOUBLE, (ProcRank + 1) % ProcNum, 0, MPI_COMM_WORLD, &Request);
		for (i = 0; i < count; i++)
			 MPI_Recv(colomn[i], N, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Status);
		num++;
	}
	if (ProcRank == 0)
	{
		for (i = 0; i < count; i++)
			a[i] = rez[i];
		for (i = count; i < ProcNum * count; i ++)
		{
	    	MPI_Recv(rez[0], N, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			a[Status.MPI_SOURCE * count + Status.MPI_TAG] = rez[0];
		}

	} else
	{
		for (i = 0; i < count; i++)
			MPI_Isend(rez[i], N, MPI_DOUBLE, 0, i, MPI_COMM_WORLD, &Request);
	}

	if (ProcRank == 0)
    {
     //   startwtime = MPI_Wtime();
        for (i = N - ProcNum * count; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                double sum = 0;
                for (k = 0; k < N; k++)
                    sum += x[i][k] * z[j][k];
                a[i][j] = sum;
            }
        }
    }

	
	if (ProcRank == 0)
	{
		endwtime = MPI_Wtime();
		printf("\nparallel = %f\n", endwtime - startwtime);
	}
	
	if (ProcRank == 0)
	{
		startwtime = MPI_Wtime();
		for (i = 0; i < N; i++)
		{
			for (j = 0; j < N; j++)
			{
				double sum = 0;
				for (k = 0; k < N; k++)
					sum += x[i][k] * z[j][k];
				a[i][j] = sum;
			}	
		}
	}

	if (ProcRank == 0)
	{
		endwtime = MPI_Wtime();
		printf("\nno parallel = %f\n", endwtime - startwtime);
	}
	
	MPI_Finalize();
	free(y);
    return 0;
}

