//#include <stdlib.h>
//#include <omp.h>
//#include <stdio.h>
//#include <iostream>
//#include <ctime>
//#include <cfloat>
//#include "math.h"
//
//using namespace std;
//
//
//int main()
//{
//	int countOfElement;
//	cout << "count of steps" << endl;
//	cin >> countOfElement;
//
//	double startTime, endTime;
//	double sum = 0;
//	double a = 0, b = 1;
//
//	startTime = omp_get_wtime();
//#pragma omp parallel shared(sum)
//	{
//		double pSum = 0;
//#pragma omp  for
//		for (int i = 0; i < countOfElement; i++)
//			pSum += (double)1 / countOfElement*f(a + (b - a) * 1 / countOfElement*i);
//#pragma omp critical
//		sum += pSum;
//	}
//
//	endTime = omp_get_wtime();
//	cout << sum << endl;
//	cout << "parallel time : " << (double)(endTime - startTime) << endl;
//
//	startTime = omp_get_wtime();
//
//	sum = 0;
//
//	for (int i = 0; i < countOfElement; i++)
//		sum += (double)1 / countOfElement*f(a + (b - a) * 1 / countOfElement*i);
//
//	endTime = omp_get_wtime();
//	cout << sum << endl;
//	cout << "no parallel time : " << (double)(endTime - startTime) << endl;
//}

#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cfloat>
#include <climits>
#include <sstream>
#include <random>

double f(double x)
{
	return sin(x);
}

int main(int argc, char** argv)
{
	using namespace std::chrono;
    std::stringstream ss;
	
	int countOfElement;
	ss << argv[1];
	ss >> countOfElement;
	int threads_count = 8;
	
	std::mt19937 mt_rand(time(0));

	//double *arr = new double[countOfElement];
	//srand(clock());
	//for (int i = 0; i < countOfElement; i++) {
	//	arr[i] =(double) (mt_rand()/(double)int_max)/2;
	//	//std::clog << arr[i] << std::endl;
	//}
	//double *arr2 = new double[countOfElement];
	//for (int i = 0; i < countOfElement; i++)
	//	arr2[i] =(double) (mt_rand()/(double)int_max)/2;
	//	//arr2[i] =(double) rand()/dbl_max;

	std::ofstream fout("test");

	int time;
	
	for (int count = 1; count <= threads_count; count++) {
		double sum = 0;
		double a = 0, b = 1;
		omp_set_num_threads(count);
		high_resolution_clock::time_point _t0{high_resolution_clock::now()};
		if (count > 1) {
			#pragma omp parallel shared(sum)
			{
				double pSum = 0;
				#pragma omp for
				for (int i = 0; i < countOfElement; i++)
					pSum += (double)1 / countOfElement*f(a + (b - a) * 1 / 
							countOfElement*i);
				#pragma omp critical
				sum += pSum;
			}
		} else {
			for (int i = 0; i < countOfElement; i++)
				sum += (double)1 / countOfElement*f(a + (b - a) * 1 / countOfElement*i);
		}
		high_resolution_clock::time_point _t1{high_resolution_clock::now()};
		
		std::cout << count << " : "<< sum << std::endl;
		auto dt = duration_cast<microseconds>(_t1 - _t0);
		time = dt.count();

		fout << time << std::endl;
	}

}
