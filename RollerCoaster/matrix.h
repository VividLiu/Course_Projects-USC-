#ifndef _matrix_H
#define _matrix_H

#include<vector>
#include<iostream>

using namespace std;

class matrix{
	public:
		//data
		int nrow;
		int ncol;
		vector<vector<double>> mat;
		
		//constructor and destructor		
		matrix();
		matrix(int rows, int columns);
		
		
		~matrix();
		
		//member function
		void assign(double* data);
		void copy(matrix mat_2);
        //return the product of mat*mat_2;
		matrix multiply(const matrix& mat_2);
		void printMat();
		
			
};

#endif















