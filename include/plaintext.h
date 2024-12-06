#pragma once

#include "config.h"

using namespace std;

vector<vector<vector<double>>> splitIntoSquareMatrices(vector<vector<double> > &matrix, int k);
void printMatrix(vector<vector<double>> matrix);
vector<vector<double>> preprocessToDiagonalForm(vector<vector<double>> &matrix);
vector<vector<double>> concatenateRows(const vector<vector<vector<double>>> &matrices);
