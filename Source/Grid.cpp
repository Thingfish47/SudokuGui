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

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return false;

    return true;
}

void SudokuGrid::fillDiagonal()
{
    for (int i = 0; i < N; i += 3)
    {
        std::vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        //std::shuffle(nums.begin(), nums.end());
        for (int k = 0; k < N; k++) 
        {
            int r = k + rand() % (N - k); // careful here!
            int x = nums[k];
            nums[k] = nums[r];
            nums[r] = x;
            //swap(nums[k], nums[r]);
        }
        for (int row = 0; row < 3; row++)
            for (int col = 0; col < 3; col++)
                grid[i + row][i + col] = nums[row * 3 + col];
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


