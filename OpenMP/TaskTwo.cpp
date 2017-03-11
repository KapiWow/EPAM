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
	srand(clock());
	for (int i = 0; i < countOfElement; i++)
		arr[i] =(double) rand()/DBL_MAX;
	double *arr2 = new double[countOfElement];
	for (int i = 0; i < countOfElement; i++)
		arr2[i] =(double) rand()/DBL_MAX;

	double startTime, endTime;
	startTime = omp_get_wtime();
	double sum = 0;
#pragma omp parallel shared(arr, arr2, sum)
	{
		double pSum = 0;
#pragma omp for 
		for (int i = 0; i < countOfElement; i++)
			pSum += arr[i] * arr2[i];
#pragma op critical
		sum += pSum;
	}
	endTime = omp_get_wtime();
	cout << sum << endl;
	cout << "parallel time : " << (double)(endTime - startTime) << endl;

	startTime = omp_get_wtime();
	sum = 0;

	for (int i = 0; i < countOfElement; i++)
		sum += arr[i] * arr2[i];

	endTime = omp_get_wtime();
	cout << sum << endl;
	cout << "no parallel time : " << (double)(endTime - startTime) << endl;
}
