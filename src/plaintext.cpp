
#include <iostream>
#include <vector>
/*
using Matrix = std::vector<std::vector<int>>;
using Diagonals = std::vector<std::vector<int>>;
using Vector = std::vector<int>;
using MatrixList = std::vector<Matrix>;
*/
std::vector<std::vector<std::vector<double>>> splitIntoSquareMatrices(std::vector<std::vector<double> > &matrix, int k) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    std::vector<std::vector<std::vector<double> > > result;

    // Split the matrix into k x k square matrices
    for (int i = 0; i < rows; i += k) {
	    std::vector<std::vector<double> > squareMatrix;

        for (int j = 0; j < k; ++j) {
            std::vector<double> row;
            for (int col = 0; col < cols; ++col) {
                row.push_back(matrix[i + j][col]);
            }
            squareMatrix.push_back(row);
        }

        result.push_back(squareMatrix);
    }

    return result;
}

// function to print the matrix
void printMatrix(std::vector<std::vector<double>> matrix) {
    for (const auto& row : matrix) {
        for (double value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

// pre-process the matrix into diagonal form
std::vector<std::vector<double>> preprocessToDiagonalForm(std::vector<std::vector<double>> &matrix) {
    int K = matrix.size();        // number of rows
    int N = matrix[0].size();      // number of columns

    std::vector<std::vector<double>> diagonals(N, std::vector<double>(K));

    // fill each diagonal vector
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < K; ++j) {
            // calculate the column index for diagonal i in row j
            int colIndex = (j + i) % N;
            diagonals[i][j] = matrix[j][colIndex];
        }
    }

    return diagonals;
}

// Function to concatenate rows from multiple matrices
std::vector<std::vector<double>> concatenateRows(const std::vector<std::vector<std::vector<double>>> &matrices) {
    std::vector<std::vector<double>> concatenatedRows;

    if (matrices.empty()) return concatenatedRows;

    // Assume all matrices have the same number of rows
    int numRows = matrices[0].size();

    // For each row in the matrices
    for (int rowIndex = 0; rowIndex < numRows; ++rowIndex) {
	std::vector<double> concatenatedRow;

        // Concatenate the rowIndex-th row of each matrix
        for (const auto& matrix : matrices) {
            const auto& row = matrix[rowIndex];
            concatenatedRow.insert(concatenatedRow.end(), row.begin(), row.end());
        }

        concatenatedRows.push_back(concatenatedRow);
    }

    return concatenatedRows;
}
