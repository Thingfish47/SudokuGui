#pragma once

#define		N	9
#define		NS	3

class SudokuGrid
{
public:
	SudokuGrid();
	SudokuGrid(SudokuGrid*);
	~SudokuGrid() {}

	void	printGrid();
	int		getValue(int row, int col) { return grid[row][col]; }
	bool	resolveSudoku();
	void	removeDigits(int numToRemove);
	//SudokuGrid& operator=(const SudokuGrid& C);
private:
	void	fillDiagonal();
	bool	isSafe(int row, int col, int num);
	bool	solveSudoku(int row, int col);
private:
	int		grid[N][N];
};

