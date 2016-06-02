//
//  Geometry.cpp
//  tinyRendererProj
//
//  Created by Maria Terraes on 02/06/2016.
//  Copyright © 2016 Maria Terraes. All rights reserved.
//

#include "Geometry.h"
#include <cassert>

Matrix::Matrix(int rows, int cols)
: mCols(cols)
, mRows(rows)
, mMatrix(std::vector<std::vector<float>>(rows, std::vector<float>(cols, 0.f)))
{
}

Matrix::Matrix(const std::vector<std::vector<float>>& matrix)
: mMatrix(matrix)
{
	mCols = matrix.size() > 0 ? static_cast<int>(matrix[0].size()) : 0;
	mRows = static_cast<int>(matrix.size());
}

Matrix::Matrix(const Matrix& matrix)
: mCols(matrix.getCols())
, mRows(matrix.getRows())
, mMatrix(std::vector<std::vector<float>>(matrix.getRows(), std::vector<float>(matrix.getCols(), 0.f)))
{
	for(int i = 0; i < mRows; i++)
	{
		std::copy(matrix[i].begin(),matrix[i].end(), mMatrix[i].begin());
	}
}
	
static Matrix identity(int dimensions)
{
	Matrix m(dimensions, dimensions);
	for(int i = 0; i < dimensions; i++)
	{
		m[i][i] = 1.f;
	}
	return m;
}
	
Matrix Matrix::operator*(const Matrix& lhs)
{
	Matrix result(mRows, lhs.getCols());
	for(int i = 0; i < result.getRows(); i++)
	{
		for(int j = 0; j < result.getCols(); j++)
		{
			result[i][j] = 0.f;
			for(int k = 0; k < mCols; k++)
			{
				result[i][j] += mMatrix[i][k]*lhs[k][j];
			}
		}
	}
	return result;
}

Matrix Matrix::getTranspose()
{
	Matrix transMatrix(mMatrix);
	for(int i = 0; i < mRows; i++)
	{
		for(int j = i; j < mCols; j++)
		{
			std::swap(transMatrix[i][j],transMatrix[j][i]);
		}
	}
	return transMatrix;
}

Matrix Matrix::getInverse()
{
	assert(mRows==mCols);
	// augmenting the square matrix with the identity matrix of the same dimensions a => [ai]
	Matrix result(mRows, mCols*2);
	for(int i=0; i<mRows; i++)
		for(int j=0; j<mCols; j++)
			result[i][j] = mMatrix[i][j];
	for(int i=0; i<mRows; i++)
		result[i][i+mCols] = 1;
	// first pass
	for (int i=0; i<mRows-1; i++) {
		// normalize the first row
		for(int j=result.mCols-1; j>=0; j--)
			result[i][j] /= result[i][i];
		for (int k=i+1; k<mRows; k++) {
			float coeff = result[k][i];
			for (int j=0; j<result.mCols; j++) {
				result[k][j] -= result[i][j]*coeff;
			}
		}
	}
	// normalize the last row
	for(int j=result.mCols-1; j>=mRows-1; j--)
		result[mRows-1][j] /= result[mRows-1][mRows-1];
	// second pass
	for (int i=mRows-1; i>0; i--) {
		for (int k=i-1; k>=0; k--) {
			float coeff = result[k][i];
			for (int j=0; j<result.mCols; j++) {
				result[k][j] -= result[i][j]*coeff;
			}
		}
	}
	// cut the identity matrix back
	Matrix truncate(mRows, mCols);
	for(int i=0; i<mRows; i++)
		for(int j=0; j<mCols; j++)
			truncate[i][j] = result[i][j+mCols];
	return truncate;
}
