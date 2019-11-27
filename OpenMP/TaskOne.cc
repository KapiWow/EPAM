#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <cfloat>
#include <sstream>

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

	double *arr = new double[countOfElement];
	srand(time(NULL));
	for (int i = 0; i < countOfElement; i++)
		arr[i] = rand();

	double max_val = -DBL_MAX;
	std::ofstream fout("test");

	int time;
	
#pragma omp parallel for reduction(max : max_val)
	for (int i = 0; i < countOfElement/100; i++)
		if (arr[i] > max_val)
			max_val = arr[i];
	max_val = -DBL_MAX;

	for (int count = 1; count <= threads_count; count++) {
		omp_set_num_threads(count);
		high_resolution_clock::time_point _t0{high_resolution_clock::now()};
		if (count > 1) {
//#pragma omp parallel for reduction(max : max_val) schedule(dynamic)
#pragma omp parallel for reduction(max : max_val) schedule(static)
			for (int i = 0; i < countOfElement; i++)
				if (arr[i] > max_val)
					max_val = arr[i];
		} else {
			for (int i = 0; i < countOfElement; i++)
				if (arr[i] > max_val)
					max_val = arr[i];
		}
		high_resolution_clock::time_point _t1{high_resolution_clock::now()};
		
		std::cout << max_val << std::endl;
		auto dt = duration_cast<microseconds>(_t1 - _t0);
		time = dt.count();

		max_val = -DBL_MAX;
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
