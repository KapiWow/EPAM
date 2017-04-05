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
	double * y = (double*) malloc(sizeof(double) * 1000000);
	double TotalSum, ProcSum = 0.0;
	double startwtime = 0.0, endwtime;
	int ProcRank, ProcNum, N = 125*8, i = 0, j = 0, k = 0, l = 0, m = 0, t = 0;
	int bil = 8; //count of blocks in line
	double max, maxMain;
	double **** z, **** x, **** r, **a, **b, **c;
	MPI_Status Status;
	MPI_Request Request;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	
	MPI_Comm GridComm; 
	int dims[2], periods[2], reorder = 1; 
	dims[0] = dims[1] = 8; 

	periods[0] = periods[1] = 1; 
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &GridComm); 
	
	if (ProcRank == 0)
	for (i = 0; i < bil; i++)
	{
		for (j = 0; j < bil; j++)
		{
			int rank = 0;
			int coord[2];
			coord[0] = i;
			coord[1] = j;
			MPI_Cart_rank(GridComm, coord, &rank);
			printf("%i ", rank); 
		}
		printf("\n");
	}
	int count = (N/bil);
	if (N != bil * count)
		count++;
	int newN = count * bil;
	if (ProcRank == 0)
	{
		x = (double****)malloc(bil * sizeof(double***));
		z = (double****)malloc(bil * sizeof(double***));
		r = (double****)malloc(bil * sizeof(double***));
		for (i = 0; i < bil; i++)
		{
			x[i] = (double***)malloc(bil * sizeof(double**));
			z[i] = (double***)malloc(bil * sizeof(double**));
			r[i] = (double***)malloc(bil * sizeof(double**));
			for (j = 0; j < bil; j++)
			{
				x[i][j] = (double**)malloc(count * sizeof(double*));
				z[i][j] = (double**)malloc(count * sizeof(double*));
				r[i][j] = (double**)malloc(count * sizeof(double*));
				for (k = 0; k < count; k++)
				{
					x[i][j][k] = (double*)malloc(count * sizeof(double));
					z[i][j][k] = (double*)malloc(count * sizeof(double));
					r[i][j][k] = (double*)malloc(count * sizeof(double));
					for (l = 0; l < count; l++)
					{
						x[i][j][k][l] = (double)rand()/INT_MAX;
						z[i][j][k][l] = (double)rand()/INT_MAX;
						r[i][j][k][l] = 0;
					}
				}
			}
		}
	} 
	
	a = (double**)malloc(count * sizeof(double*));
	b = (double**)malloc(count * sizeof(double*));
	c = (double**)malloc(count * sizeof(double*));
	for (i = 0; i < count; i++)
	{
		a[i] = (double*)malloc(count * sizeof(double));
		b[i] = (double*)malloc(count * sizeof(double));
		c[i] = (double*)malloc(count * sizeof(double));
	}
	if (ProcRank == 0)
	{
		printf("123\n");
		startwtime = MPI_Wtime();
		for (i = 0; i < bil; i++)
			for (j = 0; j < bil; j++)
				if ((i == 0) && (j == 0))	
				{	
					for (l = 0; l < count; l++)
					MPI_Isend(x[i][j][l], count, MPI_DOUBLE, i * bil + j, 0, MPI_COMM_WORLD, &Request);
					MPI_Isend(z[i][j][l], count, MPI_DOUBLE, i * bil + j, 0, MPI_COMM_WORLD, &Request);
				}
	} else
	{
		for (i = 0; i < count; i++)
		{
			MPI_Recv(a[i], count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
			MPI_Recv(b[i], count, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
		}	
	}
	printf("123\n");
	
//	int num = 0;
//	while (num < ProcNum)
//	{
//		for (i = 0; i < count; i++)
//			for ( j = 0; j < count; j++)
//			{
//				double sum = 0;
//				for (k = 0; k < N; k++)
//					sum += line[i][k] * colomn[j][k];
//				rez[i][num * count + j] = sum;
//			}
//		for (i = 0; i < count; i++)
//			MPI_Isend(colomn[i], N, MPI_DOUBLE, (ProcRank + 1) % ProcNum, 0, MPI_COMM_WORLD, &Request);
//		for (i = 0; i < count; i++)
//			 MPI_Recv(colomn[i], N, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Status);
//		num++;
//	}
//	if (ProcRank == 0)
//	{
//		for (i = 0; i < count; i++)
//			a[i] = rez[i];
//		for (i = count; i < ProcNum * count; i ++)
//		{
//	    	MPI_Recv(rez[0], N, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
//			a[Status.MPI_SOURCE * count + Status.MPI_TAG] = rez[0];
//		}
//
//	} else
//	{
//		for (i = 0; i < count; i++)
//			MPI_Isend(rez[i], N, MPI_DOUBLE, 0, i, MPI_COMM_WORLD, &Request);
//	}
//
//	if (ProcRank == 0)
//	{
//		endwtime = MPI_Wtime();
//		printf("parallel = %f\n", endwtime - startwtime);
//	}
	
	if (ProcRank == 0)
	{
		startwtime = MPI_Wtime();
		for (i = 0; i < bil; i++)
			for (j = 0; j < bil; j++)
				for (k = 0; k < bil; k++)
					for (l = 0; l < count; l++)
						for (m = 0; m < count; m++)
						{
							double sum = 0;
							for (t = 0; t < count; t++)
								sum += x[i][k][l][t] * z[k][j][m][t];
							r[i][j][l][m] += sum;
						}
	}

	if (ProcRank == 0)
	{
		endwtime = MPI_Wtime();
		printf("no parallel = %f\n", endwtime - startwtime);
	}
	
	MPI_Finalize();
	free(y);
    return 0;
}

