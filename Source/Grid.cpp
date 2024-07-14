#include "Grid.h"
#include <algorithm>
#include <random>
#include <vector>
#include <iostream>


SudokuGrid::SudokuGrid()
{
	memset(grid, 0, sizeof(grid));
    fillDiagonal();
    solveSudoku(0, 0);
}

SudokuGrid::SudokuGrid(SudokuGrid* original)
{
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            grid[row][col] = original->getValue(row, col);
}

//SudokuGrid& SudokuGrid::operator=(const SudokuGrid& C)
//{
//    for (int row = 0; row < N; row++)
//        for (int col = 0; col < N; col++)
//            grid[row][col] = C.getValue(row, col);
//    return *this;
//}

void SudokuGrid::removeDigits(int numToRemove)
{
    while (numToRemove > 0)
    {
        int cellId = rand() % (N * N);
        int row = cellId / N;
        int col = cellId % N;
        if (grid[row][col] != 0)
        {
            grid[row][col] = 0;
            numToRemove--;
        }
    }
}


void SudokuGrid::printGrid()
{
	for (int row = 0; row < N; row++) {
		for (int col = 0; col < N; col++)
			std::cout << grid[row][col] << " ";
        std::cout << std::endl;
	}
    std::cout << std::endl;
}

bool SudokuGrid::isSafe(int row, int col, int num)
{
    for (int x = 0; x < N; x++)
        if (grid[row][x] == num || grid[x][col] == num)
            return false;

    int startRow = row - row % NS, startCol = col - col % NS;
    for (int i = 0; i < NS; i++)
        for (int j = 0; j < NS; j++)
            if (grid[i + startRow][j + startCol] == num)
                return false;

    return true;
}

void SudokuGrid::fillDiagonal()
{
    for (int i = 0; i < N; i += NS)
    {
        std::vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        for (int k = 0; k < N; k++) 
        {
            int r = k + rand() % (N - k); // careful here!
            int x = nums[k];
            nums[k] = nums[r];
            nums[r] = x;
        }
        for (int row = 0; row < NS; row++)
            for (int col = 0; col < NS; col++)
                grid[i + row][i + col] = nums[row * NS + col];
    }
}

bool SudokuGrid::solveSudoku(int row, int col)
{
    if (row == N - 1 && col == N)
        return true;
    if (col == N) {
        row++;
        col = 0;
    }
    if (grid[row][col] != 0)
        return solveSudoku(row, col + 1);

    for (int num = 1; num <= 9; num++)
    {
        if (isSafe(row, col, num))
        {
            grid[row][col] = num;
            if (solveSudoku(row, col + 1))
                return true;
        }
        grid[row][col] = 0;
    }
    return false;
}

bool SudokuGrid::resolveSudoku()
{
    int row = -1, col = -1;
    bool isEmpty = true;

    // Find an empty cell
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 0) {
                row = i;
                col = j;
                isEmpty = false;
                break;
            }
        }
        if (!isEmpty) {
            break;
        }
    }

    // If no empty cell is found, we are done
    if (isEmpty) {
        return true;
    }

    // Try all possible numbers for the empty cell
    for (int num = 1; num <= N; num++) {
        if (isSafe(row, col, num)) {
            grid[row][col] = num;
            if (resolveSudoku()) {
                return true;
            }
            grid[row][col] = 0; // Backtrack
        }
    }

    return false;
}

//bool SudokuGrid::isSafe(int row, int col, int num)
//{
//    // Check if the number is not in the current row and column
//    for (int x = 0; x < N; x++) {
//        if (grid[row][x] == num || grid[x][col] == num) {
//            return false;
//        }
//    }
//
//    // Check if the number is not in the current 3x3 box
//    int startRow = row - row % NS, startCol = col - col % NS;
//    for (int i = 0; i < NS; i++) {
//        for (int j = 0; j < NS; j++) {
//            if (grid[i + startRow][j + startCol] == num) {
//                return false;
//            }
//        }
//    }
//
//    return true;
//}

