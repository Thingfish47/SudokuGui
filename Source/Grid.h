#pragma once

#define		N	9

class SudokuGrid
{
public:
	SudokuGrid();
	~SudokuGrid() {}

	void	printGrid();
	int getValue(int row, int col) { return grid[row][col]; }
private:
	void	fillDiagonal();
	bool	isSafe(int row, int col, int num);
	bool	solveSudoku(int row, int col);
private:
	int		grid[N][N];
};

