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
		arr[i] = DBL_MAX * rand()/ 32767.0;

	int startTime, endTime;
	startTime = clock();
	double max = arr[0];

#pragma omp parallel shared(arr,max) 
	{
		double pMax = arr[0];
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

	//for (int i = 0; i < countOfElement; i++)
	//{
	//	if (arr[i] > max)
	//		max = arr[i];
	//}

	endTime = clock();
	cout << max << endl;
	cout << "time : " << (endTime - startTime)/10000 << endl;
}
