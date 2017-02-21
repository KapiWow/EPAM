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

	double **arr = new double*[countOfElement];
	srand(time(NULL));
	for (int i = 0; i < countOfElement; i++)
	{
		arr[i] = new double[countOfElement];
		for (int j = 0; j < countOfElement; j++)
			arr[i][j] = rand();
	}
	double startTime, endTime;
	startTime = omp_get_wtime();
	double max = -DBL_MAX;

#pragma omp parallel for
	for (int i = 0; i < countOfElement; i++)
	{
		double min = DBL_MAX;
		for (int j = 0; j < countOfElement; j++)
			if (arr[i][j] < min)
				min = arr[i][j];
#pragma omp critical
		if (min > max)
			max = min;
	}

	endTime = omp_get_wtime();
	cout << max << endl;
	cout << "parallel time : " << (double)(endTime - startTime) << endl;

	startTime = omp_get_wtime();
	max = -DBL_MAX;

	for (int i = 0; i < countOfElement; i++)
	{
		double min = DBL_MAX;
		for (int j = 0; j < countOfElement; j++)
			if (arr[i][j] < min)
				min = arr[i][j];
		if (min > max)
			max = min;
	}


	endTime = omp_get_wtime();
	cout << max << endl;
	cout << "no parallel time : " << (double)(endTime - startTime) << endl;
}
