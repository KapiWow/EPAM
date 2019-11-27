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

int main(int argc, char** argv)
{
	using namespace std::chrono;
    std::stringstream ss;
	
	int countOfElement;
	ss << argv[1];
	ss >> countOfElement;
	//std::clog << countOfElement << std::endl;
	//bool seq;
	//ss << argv[2];
	//ss >> seq;
	int threads_count = 8;
	//ss << argv[2];
	//ss >> threads_count;

	//if (threads_count == 1)
	//	seq = true;

	//double *arr = new double[countOfElement];
	//srand(time(NULL));
	//for (int i = 0; i < countOfElement; i++)
	//	arr[i] = rand();
	
	std::mt19937 mt_rand(time(0));

	double dbl_max = DBL_MAX/10.0;
	int int_max = INT_MAX;
	double *arr = new double[countOfElement];
	srand(clock());
	for (int i = 0; i < countOfElement; i++) {
		arr[i] =(double) (mt_rand()/(double)int_max)/2;
		//std::clog << arr[i] << std::endl;
	}
	double *arr2 = new double[countOfElement];
	for (int i = 0; i < countOfElement; i++)
		arr2[i] =(double) (mt_rand()/(double)int_max)/2;
		//arr2[i] =(double) rand()/dbl_max;

	std::ofstream fout("test");

	int time;
	
	for (int count = 1; count <= threads_count; count++) {
		double sum = 0;
		omp_set_num_threads(count);
		high_resolution_clock::time_point _t0{high_resolution_clock::now()};
		if (count > 1) {
#pragma omp parallel shared(arr, arr2, sum)
			{
				double pSum = 0;
#pragma omp for 
				for (int i = 0; i < countOfElement; i++)
					pSum += arr[i] * arr2[i];
#pragma omp critical
				sum += pSum;
			}
		} else {
			for (int i = 0; i < countOfElement; i++)
				sum += arr[i] * arr2[i];
		}
		high_resolution_clock::time_point _t1{high_resolution_clock::now()};
		
		std::cout << count << " : "<< sum << std::endl;
		auto dt = duration_cast<microseconds>(_t1 - _t0);
		time = dt.count();

		fout << time << std::endl;
	}

	//_t0 = high_resolution_clock::now();
	////======================================
	////======================================
	//_t1 = high_resolution_clock::now();
	//dt = duration_cast<microseconds>(_t1 - _t0);
	//seq_time = dt.count();

	//std::cout << max_val << std::endl;

	//std::clog << "parallel time: " << parallel_time << std::endl;
	//std::clog << "sequence time: " << seq_time << std::endl;
	//fout << seq_time << std::endl;
}
