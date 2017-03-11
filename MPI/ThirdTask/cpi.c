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
	int size = (sizeof(double)*1000000);
	double * x = (double*) malloc(size);
	double * y = (double*) malloc(size);
	double TotalSum, ProcSum = 0.0;
	double startwtime = 0.0, endwtime;
	int ProcRank, ProcNum, N = 1000000, i = 0;
	
	for (i = 0; i < N; i++)
	{
		x[i] = (double)rand()/INT_MAX;
		y[i] = (double)rand()/INT_MAX;
	}
	MPI_Status Status;
	MPI_Request Request;
	MPI_Request Request2;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    //Ssend
	startwtime = MPI_Wtime();
	for (i = 0; i<100; i++)
	{
		if (ProcRank == 0)	
		{
			MPI_Ssend(x, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
			MPI_Ssend(y, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
		}
		if (ProcRank == 1)	
		{
			MPI_Recv(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
			MPI_Recv(y, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
		}
	}

	if (ProcRank == 0)
	{
		endwtime = MPI_Wtime();
		printf("Ssend = %f\n", endwtime - startwtime);
	}
	//Rsend
	startwtime = MPI_Wtime();
	for (i = 0; i<100; i++)
	{
		if (ProcRank == 0)	
		{
			MPI_Rsend(x, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(y, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &Status);
		}
		if (ProcRank == 1)	
		{
			MPI_Recv(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
			MPI_Rsend(y, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}	
	}
	if (ProcRank == 0)
	{
		endwtime = MPI_Wtime();
		printf("Rsend = %f\n", endwtime - startwtime);
	}
	//BSend
	//double *buf;
	//int sizeBuf = N*sizeof(double)*1000;
	//buf = (double *)malloc(sizeBuf + MPI_BSEND_OVERHEAD);
	//MPI_Buffer_attach(buf, sizeBuf+MPI_BSEND_OVERHEAD);
	//startwtime = MPI_Wtime();
	//for (i = 0; i<100; i++)
	//{
	//	if (ProcRank == 0)	
	//		MPI_Bsend(x, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
	//	//	MPI_Recv(x, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &Status);
	//	if (ProcRank == 1)	
	//		MPI_Recv(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
	//	//	MPI_Bsend(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	//}

	//if (ProcRank == 0)
	//{
	//	endwtime = MPI_Wtime();
	//	printf("Bsend and recv on both = %f\n", endwtime - startwtime);
	//}

	//MPI_Buffer_detach(&buf, &sizeBuf);
	//Send
	startwtime = MPI_Wtime();
	for (i = 0; i<100; i++)
	{
		if (ProcRank == 0)	
		{
			MPI_Send(x, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(y, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &Status);
		}
		if (ProcRank == 1)	
		{
			MPI_Recv(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Status);
			MPI_Send(y, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}	
	}

	if (ProcRank == 0)
	{
		endwtime = MPI_Wtime();
		printf("Send = %f\n", endwtime - startwtime);
	}
	int test = 0, test2 = 0;
	//Isend
	startwtime = MPI_Wtime();
	for (i = 0; i<100; i++)
	{
	test = 0;
	test2 = 0;
		if (ProcRank == 0)	
		{
			MPI_Isend(x, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &Request);
			MPI_Irecv(y, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &Request2);
			while ((!test)&&(!test2))
			{
				MPI_Test(&Request, &test, &Status);
				MPI_Test(&Request2, &test2, &Status);
			}
		}
		if (ProcRank == 1)	
		{
			MPI_Irecv(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Request);
			MPI_Isend(y, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Request2);
			while ((!test)&&(!test2))
			{			
				MPI_Test(&Request, &test, &Status);
				MPI_Test(&Request2, &test2, &Status);
			}
		}
	}
	if (ProcRank == 0)
	{
		endwtime = MPI_Wtime();
		printf("Isend = %f\n", endwtime - startwtime);
	}
	//Irsend
	startwtime = MPI_Wtime();
	for (i = 0; i<100; i++)
	{
	test = 0;
	test2 = 0;
		if (ProcRank == 0)	
		{
			MPI_Irsend(x, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &Request);
			MPI_Irecv(y, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &Request2);
			while ((!test)&&(!test2))
			{
				MPI_Test(&Request, &test, &Status);
				MPI_Test(&Request2, &test2, &Status);
			}
		}
		if (ProcRank == 1)	
		{
			MPI_Irecv(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Request);
			MPI_Irsend(y, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Request2);
			while ((!test)&&(!test2))
			{			
				MPI_Test(&Request, &test, &Status);
				MPI_Test(&Request2, &test2, &Status);
			}
		}
	}
	if (ProcRank == 0)
	{
		endwtime = MPI_Wtime();
		printf("Irsend = %f\n", endwtime - startwtime);
	}
	//Issend
	startwtime = MPI_Wtime();
	for (i = 0; i<100; i++)
	{
	test = 0;
	test2 = 0;
		if (ProcRank == 0)	
		{
			MPI_Issend(x, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &Request);
			MPI_Irecv(y, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, &Request2);
			while ((!test)&&(!test2))
			{
				MPI_Test(&Request, &test, &Status);
				MPI_Test(&Request2, &test2, &Status);
			}
		}
		if (ProcRank == 1)	
		{
			MPI_Irecv(x, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Request);
			MPI_Issend(y, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &Request2);
			while ((!test)&&(!test2))
			{			
				MPI_Test(&Request, &test, &Status);
				MPI_Test(&Request2, &test2, &Status);
			}
		}
	}
	if (ProcRank == 0)
	{
		endwtime = MPI_Wtime();
		printf("Issend = %f\n", endwtime - startwtime);
	}
	//SendRecv
    startwtime = MPI_Wtime();
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

