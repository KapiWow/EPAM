#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <cfloat>
#include "math.h"

using namespace std;

double f(double x)
{
	return sin(x);
}

int main()
{
	int countOfElement;
	cout << "count of steps" << endl;
	cin >> countOfElement;

	int startTime, endTime;
	startTime = clock();
	double sum = 0;
	double a = 0, b = 1;

#pragma omp parallel shared(sum)
	{
		double pSum = 0;
#pragma omp  for
		for (int i = 0; i < countOfElement; i++)
			pSum += (double)1 / countOfElement*f(a + (b - a) * 1 / countOfElement*i);
#pragma omp critical
		sum += pSum;
	}

	endTime = clock();
	cout << sum << endl;
	cout << "parallel time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;

	startTime = clock();

	sum = 0;

	for (int i = 0; i < countOfElement; i++)
		sum += (double)1 / countOfElement*f(a + (b - a) * 1 / countOfElement*i);

	endTime = clock();
	cout << sum << endl;
	cout << "no parallel time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << endl;
}
