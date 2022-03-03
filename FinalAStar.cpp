//My implementation of A* algorithm with C++
#include <cfloat>
#include <set>
#include <stack>
#include<cmath>
#include <iostream>
#include<cmath>
#include <utility>
#include <stdio.h>
#include <fstream>
#include <chrono>
using namespace std;
using namespace std::chrono;
#define GETINDEX(y,x,COl)  ((y) * COL) + x

typedef pair<int, int> Pair;
typedef pair<float, pair<int, int> > pPair;

int ROW, COL;
int** grid;
bool* closedList;
float* gScore;
float* fScore;
int* parent_Y;
int* parent_X;

stack<Pair> Path;
//****************************************************************************************//
bool isValid(int row, int col) {
	return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}
//****************************************************************************************//
bool isUnBlocked(int row, int col) {
	return (grid[row][col] == 1) ? (true) : (false);
}
//****************************************************************************************//
bool isDestination(int row, int col, Pair dest) {
	return (row == dest.first && col == dest.second) ? (true) : (false);
}
//****************************************************************************************//
float calculateHValue(int row, int col, Pair dest) {
	return (sqrt(((row - dest.first) * (row - dest.first)) + ((col - dest.second) * (col - dest.second))));
}
//****************************************************************************************//
void tracePath(Pair dest) {
	printf("\nThe Path is ");
	int row = dest.first, col = dest.second;
	while (!(parent_Y[GETINDEX(row, col, COL)] == row && parent_X[GETINDEX(row, col, COL)] == col)) {
		Path.push(make_pair(row, col));
		int temp_row = parent_Y[GETINDEX(row, col, COL)];
		int temp_col = parent_X[GETINDEX(row, col, COL)];
		row = temp_row;
		col = temp_col;
	}
	Path.push(make_pair(row, col));
	while (!Path.empty()) {
		pair<int, int> p = Path.top();
		Path.pop();
		printf("-> (%d,%d) ", p.first, p.second);
	}
	return;
}

bool IsValidPath(Pair& src, Pair& dest)
{
	if (isValid(src.first, src.second) == false) {
		printf("Source is invalid\n");
		return false;
	}
	if (isValid(dest.first, dest.second) == false) {
		printf("Destination is invalid\n");
		return false;
	}
	if ((isUnBlocked(src.first, src.second) == false) || (isUnBlocked(dest.first, dest.second) == false)) {
		printf("Source or the destination is blocked\n");
		return false;
	}
	if (isDestination(src.first, src.second, dest) == true) {
		printf("We are already at the destination\n");
		return false;
	}

	return true;
}

void InitGScore()
{
	delete[] gScore;
	gScore = new float[ROW * COL];
	int block = 32, index = 0;
	int length = min(block, ROW * COL);

	while (index < length)
	{
		gScore[index] = FLT_MAX;
		index++;
	}

	length = ROW * COL;
	while (index < length)
	{
		copy(&gScore[0], &gScore[min(length - index, block)], &gScore[index]);
		index += block;
		block *= 2;
	}
}
void InitFScore()
{
	delete[] fScore;
	fScore = new float[ROW * COL];
	int block = 32, index = 0;
	int length = min(block, ROW * COL);

	while (index < length)
	{
		fScore[index] = FLT_MAX;
		index++;
	}

	length = ROW * COL;
	while (index < length)
	{
		copy(&fScore[0], &fScore[min(length - index, block)], &fScore[index]);
		index += block;
		block *= 2;
	}
}
void InitializeArray()
{
	delete[] closedList;
	closedList = new bool[ROW * COL];
	memset(closedList, false, sizeof(bool) * ROW * COL);

	delete[] parent_Y;
	parent_Y = new int[ROW * COL];
	memset(parent_Y, -1, sizeof(int) * ROW * COL);

	delete[] parent_X;
	parent_X = new int[ROW * COL];
	memset(parent_X, -1, sizeof(int) * ROW * COL);

	InitGScore();
	InitFScore();
}

//****************************************************************************************//
bool aStarSearch(Pair src, Pair dest)
{
	if (!IsValidPath(src, dest))
		return false;

	InitializeArray();

	int y, x;
	y = src.first, x = src.second;
	fScore[GETINDEX(y, x, COL)] = 0;
	gScore[GETINDEX(y, x, COL)] = 0;
	parent_Y[GETINDEX(y, x, COL)] = y;
	parent_X[GETINDEX(y, x, COL)] = x;

	set<pPair> openList;
	openList.insert(make_pair(0.0, make_pair(y, x)));
	while (!openList.empty()) {
		pPair p = *openList.begin();
		openList.erase(openList.begin());
		y = p.second.first;
		x = p.second.second;

		if (isDestination(y, x, dest))
			return true;

		closedList[(y * COL) + x] = true;
		float gNew = 0, fNew = 0;
		int count = 0;
		for (int a = -1; a <= 1; a++) {
			for (int b = -1; b <= 1; b++) {
				if ((a == 0 && b == 0) || !isValid(y + a, x + b))
					continue;

				if (!(closedList[((y + a) * COL) + x + b]) && (isUnBlocked(y + a, x + b)))
				{
					if (a == 0 || b == 0)
						gNew = gScore[GETINDEX(y, x, COL)] + 1.0;
					else
						gNew = gScore[GETINDEX(y, x, COL)] + 1.414;

					if ((gScore[GETINDEX(y + a, x + b, COL)] > gNew))
					{
						fNew = gNew + calculateHValue(y + a, x + b, dest);;

						openList.insert(make_pair(fNew, make_pair(y + a, x + b)));
						fScore[GETINDEX(y + a, x + b, COL)] = fNew;
						gScore[GETINDEX(y + a, x + b, COL)] = gNew;
						parent_Y[GETINDEX(y + a, x + b, COL)] = y;
						parent_X[GETINDEX(y + a, x + b, COL)] = x;
					}
				}

			}
		}
	}

	printf("Failed to find the Destination Cell\n");
	return false;
}
//****************************************************************************************//
int main() {
	//ifstream map_file("E:/Works/Project_C/AStar/TestMap.txt");
	ifstream map_file("E:/Works/Project_C/AStar/Aftershock.map");
	if (map_file.is_open())
	{
		map_file >> ROW;
		map_file >> COL;

		grid = new int* [ROW];
		for (int y = 0; y < ROW; ++y)
			grid[y] = new int[COL];

		char temp;
		for (int y = 0; y < ROW; y++)
		{
			for (int x = 0; x < COL; x++)
			{
				map_file >> temp;
				temp == '.' ? grid[y][x] = 1 : grid[y][x] = 0;
			}
		}
	}
	Pair src = make_pair(100, 30);
	Pair dest = make_pair(500, 500);
	//Pair src = make_pair(0, 0);
	//Pair dest = make_pair(3, 2);

	auto start = high_resolution_clock::now();
	bool findPath = false;
	for (size_t i = 0; i < 100; i++)
	{
		findPath = aStarSearch(src, dest);
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	if (findPath)
		tracePath(dest);

	cout << "\n\n" << "Time: " << duration.count() / 100 << endl;
	return (0);
}
//****************************************************************************************//End
