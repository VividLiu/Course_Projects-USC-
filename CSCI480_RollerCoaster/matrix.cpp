#include"matrix.h"

//constructor, initialize all the entries in the matrix to 0.0
matrix::matrix(){
	nrow = 1;
	ncol = 1;
	vector<double> t;
	t.push_back(0.0);
	mat.push_back(t);
}

matrix::~matrix(){

}

matrix::matrix(int rows, int columns){
	nrow = rows;
	ncol = columns;
	
	vector<double> irow;
	for(int i = 0; i < nrow; i++){
		for(int j = 0; j < ncol; j++){
			irow.push_back(0.0);
		}
		mat.push_back(irow);
		irow.clear();
	}
}

void matrix::assign(double* data){//make sure the # of rows and columns match
	int k = 0;
	for(int i = 0; i < nrow; i++){
		for(int j = 0; j < ncol; j++){
			mat[i][j] = data[k++];
		}
	}
}

void matrix::copy(matrix mat_2){
	if(nrow != mat_2.nrow || ncol != mat_2.ncol){
		cout << "Error: cannot copy, the dimensions are different!" <<endl;
		return;
	}
	for (int i = 0; i < nrow; i++){
		for (int j = 0; j < ncol; j++){
			mat[i][j] = mat_2.mat[i][j];
		}
	}
	
}

matrix matrix::multiply(const matrix& mat_2){
	
	if(ncol != mat_2.nrow){//check if these two matrices are multiplable
		cout << "Error:these two matrices can not be multiplied!" << endl;
		matrix rmat;
		return rmat;
	}else{
		matrix rmat(nrow, mat_2.ncol);
		for(int i = 0; i < nrow; i++){
			for(int j = 0; j < mat_2.ncol; j++){
				for(int k = 0; k < ncol; k++){
					rmat.mat[i][j] += mat[i][k] * mat_2.mat[k][j];
				}
			}
		}
		return rmat;
	}
	
}

void matrix::printMat(){
	
	cout << nrow << " * " << ncol << endl;
	
	for(int i = 0; i < nrow; i++){
		for(int j = 0; j < ncol; j++){
			cout << mat[i][j] <<" ";
		}
		cout << endl;
	}
}