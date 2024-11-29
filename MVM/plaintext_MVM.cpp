
#include <iostream>
#include <vector>

using Matrix = std::vector<std::vector<int>>;
using Diagonals = std::vector<std::vector<int>>;
using Vector = std::vector<int>;
using MatrixList = std::vector<Matrix>;


std::vector<Matrix> splitIntoSquareMatrices(const Matrix& matrix, int k) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    std::vector<Matrix> result;

    // Split the matrix into k x k square matrices
    for (int i = 0; i < rows; i += k) {
        Matrix squareMatrix;

        for (int j = 0; j < k; ++j) {
            std::vector<int> row;
            for (int col = 0; col < cols; ++col) {
                row.push_back(matrix[i + j][col]);
            }
            squareMatrix.push_back(row);
        }

        result.push_back(squareMatrix);
    }

    return result;
}


// pre-process the matrix into diagonal form
Diagonals preprocessToDiagonalForm(const Matrix& matrix) {
    int K = matrix.size();        // number of rows
    int N = matrix[0].size();      // number of columns

    Diagonals diagonals(N, std::vector<int>(K));

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
std::vector<Vector> concatenateRows(const std::vector<Matrix>& matrices) {
    std::vector<Vector> concatenatedRows;

    if (matrices.empty()) return concatenatedRows;

    // Assume all matrices have the same number of rows
    int numRows = matrices[0].size();

    // For each row in the matrices
    for (int rowIndex = 0; rowIndex < numRows; ++rowIndex) {
        Vector concatenatedRow;

        // Concatenate the rowIndex-th row of each matrix
        for (const auto& matrix : matrices) {
            const auto& row = matrix[rowIndex];
            concatenatedRow.insert(concatenatedRow.end(), row.begin(), row.end());
        }

        concatenatedRows.push_back(concatenatedRow);
    }

    return concatenatedRows;
}


// rotate vector left 
Vector rotateLeft(const Vector& vec, int shift) {
    int size = vec.size();
    Vector rotated(size);
    for (int i = 0; i < size; ++i) {
        rotated[i] = vec[(i + shift) % size]; 
    }

    // print 
    std::cout << "rotated " << ": ";
    for (int value : rotated) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    return rotated;
}

// multiply two vectors element-wise
Vector elementwiseMultiply(const Vector& v1, const Vector& v2) {
    int size = v1.size();
    Vector result(size);
    for (int i = 0; i < size; ++i) {
        result[i] = v1[i] * v2[i];
    }

    // print
    std::cout << "vector multiplication " << ": ";
    for (int value : result) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    return result;
}

// sum all elements in a vector
Vector sumVectors(const Vector& v1, const Vector& v2) {
    int N = v1.size();
    Vector result(N);
    for (int i = 0; i < N; ++i) {
        result[i] = v1[i] + v2[i];
    }
    return result;
}

// matrix-vector multiplication 
Vector matrixVectorMultiply(const Matrix& matrix, const Vector& vector) {
    // pre-process the matrix to get diagonals
    Diagonals diagonals = preprocessToDiagonalForm(matrix);

    int K = matrix.size();
    int N = matrix[0].size();

    Vector finalVector(N,0);

    // perform diagonal multiplication and rotation sum
    for (int i = 0; i < N; ++i) {
        Vector rotatedVector = rotateLeft(vector, i);       // rotate vector by i positions
        Vector product = elementwiseMultiply(diagonals[i], rotatedVector); // element-wise multiply
        finalVector = sumVectors(finalVector, product);  // add those prduct elements to the final vector
    }

    return finalVector;
}

// function to print the matrix 
void printMatrix(const Matrix& matrix) {
    for (const auto& row : matrix) {
        for (int value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

// function to print diagonals
void printDiagonals(const Diagonals& diagonals) {
    for (int i = 0; i < diagonals.size(); ++i) {
        std::cout << "Diagonal " << i << ": ";
        for (int value : diagonals[i]) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int K = 16;  // rows 
    int N = 4;  // columns 

    Matrix matrix(K, std::vector<int>(N));
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = i * N + j + 1;
        }
    }

    std::cout << "Original Matrix:" << std::endl;
    printMatrix(matrix);
    std::cout << std::endl;

    // Split the matrix into 4x4 square matrices
    std::vector<Matrix> squareMatrices = splitIntoSquareMatrices(matrix, N);

    // Print each matrix
    for (const auto& squareMatrix : squareMatrices) {
        printMatrix(squareMatrix);
        std::cout << std::endl;  // Space between matrices for readability
    }

    // Array (vector) to hold all diagonalized matrices
    std::vector<Diagonals> allDiagonalMatrices;

    // Process and print the diagonal form of each square matrix
    for (const auto& squareMatrix : squareMatrices) {
        std::cout << "\nDiagonalized Form of Matrix:" << std::endl;
        Diagonals diagonals = preprocessToDiagonalForm(squareMatrix);
        printDiagonals(diagonals);
        
        allDiagonalMatrices.push_back(diagonals);
    }

    // Concatenate the rows from all matrices
    std::vector<Vector> concatenatedRows = concatenateRows(allDiagonalMatrices);

    // Print the concatenated rows
    std::cout << "\nConcatenated Rows:" << std::endl;
    for (const auto& row : concatenatedRows) {
        for (int value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    /*
    // pre-process the matrix to get the diagonals
    Diagonals diagonals = preprocessToDiagonalForm(matrix);

    //std::cout << "\nDiagonals of the Matrix:" << std::endl;
    //printDiagonals(diagonals);

    Vector vector(N);
    for (int i = 0; i < N; ++i) {
        vector[i] = i + 1;
    }

    // matrix-vector multiplication
    Vector result = matrixVectorMultiply(matrix, vector);

    std::cout << "\nFinal result of matrix-vector multiplication: " << std::endl;
    for (int value : result) {
        std::cout << value << " ";
    }
    std::cout << std::endl;*/

    return 0;
}