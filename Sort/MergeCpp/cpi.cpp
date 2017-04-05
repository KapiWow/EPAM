#include <iostream>
#include "mpi.h"
#include <ctime>
#include <stdlib.h>

using namespace std;

int * merge(int * arr, int n)
{	
	if (n > 15)
	{
		int * b = new int[n / 2];
		b = &arr[0];
		int * c = new int[n - (n / 2)];
		c = &arr[n / 2];
		c = merge(c, n - n / 2);
		b = merge(b, n / 2);
		int * arr2 = new int[n];

		int n1 = 0;
		int n2 = 0;
		int n3 = 0;
		while ((n1 < n / 2) && (n2 < n - n / 2))
		{
			if (b[n1] < c[n2])
			{
				arr2[n3] = b[n1];
				n1++;
			}
			else
			{
				arr2[n3] = c[n2];
				n2++;
			}
			n3++;
		}

		while (n2 < n - n / 2)
		{
			arr2[n3] = c[n2];
			n3++;
			n2++;
		}
		while (n1 < n / 2)
		{
			arr2[n3] = b[n1];
			n3++;
			n1++;
		}

		if ((n1 != n / 2) || (n2 != n - n / 2))
			cout << "error" << endl;

		return arr2;
	}
	else
	{
		for (int k = 0; k < n - 1; k++)
			for (int i = 0; i < n - k - 1; i++)
				if (arr[i]> arr[i + 1])
				{
					int cp = arr[i];
					arr[i] = arr[i + 1];
					arr[i + 1] = cp;
				}

		return arr;
	}

}

int main(int argc, char *argv[])
{
	
	int ProcRank, ProcNum;
    int N[6] = { 100, 300, 1000, 3000, 10000, 30000 };
	int count = 10;

	MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    
	for (int q = 0; q < 6; q ++)
	{
		for (int k = 0; k < count; k++)
		{
			if (ProcRank == 0)
			{
				srand(time(NULL));
				int n = N[q];
				double sum = 0;
				int * arr = new int[n];
				for (int i = 0; i < n; i++)
					arr[i] = rand();

				double startTime, endTime;
				startTime = MPI_Wtime();
				arr = merge(arr, n);
				endTime = MPI_Wtime();
				sum += endTime - startTime;
				cout << N[q] << " : " << sum / count << endl;
			}
		}
	}
	MPI_Finalize();
	return 0;
}
