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
		arr[i] = rand();
	cout << endl;
	double *arr2 = new double[countOfElement];
	for (int i = 0; i < countOfElement; i++)
		arr2[i] = rand();
	cout << endl;

	int startTime, endTime;
	startTime = clock();
	double sum = 0;

#pragma omp parallel shared(arr, arr2, sum)
	{
		double pSum = 0;
#pragma omp  for 
		for (int i = 0; i < countOfElement; i++)
			pSum += arr[i] * arr2[i];
#pragma omp critical
		sum += pSum;
	}
	endTime = clock();
	cout << sum << endl;
	cout << "parallel time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

	startTime = clock();
	sum = 0;
	for (int i = 0; i < countOfElement; i++)
		sum += arr[i] * arr2[i];

	endTime = clock();
	cout << sum << endl;
	cout << "no parallel time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;
}