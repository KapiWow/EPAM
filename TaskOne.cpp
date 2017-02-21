#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <cfloat>

using namespace std;

int main()
{
	int countOfElement;
	cout << "count of elements" << endl;
	cin >> countOfElement;

	double *arr = new double[countOfElement];
	srand(time(NULL));
	for (int i = 0; i < countOfElement; i++)
		arr[i] = rand();

	int startTime, endTime;
	startTime = clock();
	double max = -DBL_MAX;

#pragma omp parallel shared(arr,max) 
	{
		double pMax = -DBL_MAX;
#pragma omp  for
		for (int i = 0; i < countOfElement; i++)
		{
			if (arr[i] > pMax)
				pMax = arr[i];
		}
#pragma omp critical
		if (pMax > max)
			max = pMax;
	}
	endTime = clock();
	cout << max << endl;
	cout << "parallel time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;
	
	startTime = clock();
	max = -DBL_MAX;

	for (int i = 0; i < countOfElement; i++)
	{
		if (arr[i] > max)
			max = arr[i];
	}


	endTime = clock();
	cout << max << endl;
	cout << "no parallel time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;
}
