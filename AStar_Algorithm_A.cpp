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
#define ROW 512
#define COL 512
typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > pPair;
struct cell {
	int parent_i, parent_j;
	double f, g, h;
};
cell cellDetails[ROW][COL];
bool closedList[ROW][COL];
//****************************************************************************************//
bool isValid(int row, int col) { return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL); }
//****************************************************************************************//
bool isUnBlocked(char grid[][COL], int row, int col) { return (grid[row][col] == '.') ? (true) : (false); }
//****************************************************************************************//
bool isDestination(int row, int col, Pair dest) { return (row == dest.first && col == dest.second) ? (true) : (false); }
//****************************************************************************************//
double calculateHValue(int row, int col, Pair dest) {
	return ((double)sqrt(pow((row - dest.first), 2) + pow((col - dest.second), 2)));
}
//****************************************************************************************//
void tracePath(cell cellDetails[][COL], Pair dest) {
	printf("\nThe Path is ");
	int row = dest.first, col = dest.second;
	stack<Pair> Path;
	while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)) {
		Path.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
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
//****************************************************************************************//
void aStarSearch(char grid[][COL], Pair src, Pair dest)
{
	if (isValid(src.first, src.second) == false) {
		printf("Source is invalid\n");
		return;
	}
	if (isValid(dest.first, dest.second) == false) {
		printf("Destination is invalid\n");
		return;
	}
	if ((isUnBlocked(grid, src.first, src.second) == false) || (isUnBlocked(grid, dest.first, dest.second) == false)) {
		printf("Source or the destination is blocked\n");
		return;
	}
	if ((isDestination(src.first, src.second, dest) == true)) {
		printf("We are already at the destination\n");
		return;
	}
	memset(closedList, false, sizeof(closedList));
	int i, j;
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;  cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1; cellDetails[i][j].parent_j = -1;
		}
	}
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;     cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i; cellDetails[i][j].parent_j = j;
	set<pPair> openList;
	openList.insert(make_pair(0.0, make_pair(i, j)));
	bool foundDest = false;
	while (!openList.empty()) {
		pPair p = *openList.begin();
		openList.erase(openList.begin());
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;
		double gNew, hNew, fNew;
		if (isValid(i - 1, j) == true) {
			if (isDestination(i - 1, j, dest) == true) {
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if ((closedList[i - 1][j] == false) && (isUnBlocked(grid, i - 1, j) == true)) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i - 1, j, dest);
				fNew = gNew + hNew;
				if ((cellDetails[i - 1][j].f == FLT_MAX) || (cellDetails[i - 1][j].f > fNew)) {
					openList.insert(make_pair(fNew, make_pair(i - 1, j)));
					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;    cellDetails[i - 1][j].h = hNew;
					cellDetails[i - 1][j].parent_i = i; cellDetails[i - 1][j].parent_j = j;
				}
			}
		}
		if (isValid(i + 1, j) == true) {
			if (isDestination(i + 1, j, dest) == true) {
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if ((closedList[i + 1][j] == false) && (isUnBlocked(grid, i + 1, j) == true)) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i + 1, j, dest);
				fNew = gNew + hNew;
				if ((cellDetails[i + 1][j].f == FLT_MAX) || (cellDetails[i + 1][j].f > fNew)) {
					openList.insert(make_pair(fNew, make_pair(i + 1, j)));
					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;    cellDetails[i + 1][j].h = hNew;
					cellDetails[i + 1][j].parent_i = i; cellDetails[i + 1][j].parent_j = j;
				}
			}
		}
		if (isValid(i, j + 1) == true) {
			if (isDestination(i, j + 1, dest) == true) {
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if ((closedList[i][j + 1] == false) && (isUnBlocked(grid, i, j + 1) == true)) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j + 1, dest);
				fNew = gNew + hNew;
				if ((cellDetails[i][j + 1].f == FLT_MAX) || (cellDetails[i][j + 1].f > fNew)) {
					openList.insert(make_pair(fNew, make_pair(i, j + 1)));
					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;    cellDetails[i][j + 1].h = hNew;
					cellDetails[i][j + 1].parent_i = i; cellDetails[i][j + 1].parent_j = j;
				}
			}
		}
		if (isValid(i, j - 1) == true) {
			if (isDestination(i, j - 1, dest) == true) {
				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if ((closedList[i][j - 1] == false) && (isUnBlocked(grid, i, j - 1) == true)) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j - 1, dest);
				fNew = gNew + hNew;
				if ((cellDetails[i][j - 1].f == FLT_MAX)
					|| (cellDetails[i][j - 1].f > fNew)) {
					openList.insert(make_pair(fNew, make_pair(i, j - 1)));
					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;    cellDetails[i][j - 1].h = hNew;
					cellDetails[i][j - 1].parent_i = i; cellDetails[i][j - 1].parent_j = j;
				}
			}
		}
		if (isValid(i - 1, j + 1) == true) {
			if (isDestination(i - 1, j + 1, dest) == true) {
				cellDetails[i - 1][j + 1].parent_i = i;
				cellDetails[i - 1][j + 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if ((closedList[i - 1][j + 1] == false) && (isUnBlocked(grid, i - 1, j + 1) == true)) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i - 1, j + 1, dest);
				fNew = gNew + hNew;
				if ((cellDetails[i - 1][j + 1].f == FLT_MAX) || (cellDetails[i - 1][j + 1].f > fNew)) {
					openList.insert(make_pair(fNew, make_pair(i - 1, j + 1)));
					cellDetails[i - 1][j + 1].f = fNew;
					cellDetails[i - 1][j + 1].g = gNew;    cellDetails[i - 1][j + 1].h = hNew;
					cellDetails[i - 1][j + 1].parent_i = i; cellDetails[i - 1][j + 1].parent_j = j;
				}
			}
		}
		if (isValid(i - 1, j - 1) == true) {
			if (isDestination(i - 1, j - 1, dest) == true) {
				cellDetails[i - 1][j - 1].parent_i = i;
				cellDetails[i - 1][j - 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if ((closedList[i - 1][j - 1] == false) && (isUnBlocked(grid, i - 1, j - 1) == true)) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i - 1, j - 1, dest);
				fNew = gNew + hNew;
				if ((cellDetails[i - 1][j - 1].f == FLT_MAX) || (cellDetails[i - 1][j - 1].f > fNew)) {
					openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));
					cellDetails[i - 1][j - 1].f = fNew;
					cellDetails[i - 1][j - 1].g = gNew;    cellDetails[i - 1][j - 1].h = hNew;
					cellDetails[i - 1][j - 1].parent_i = i; cellDetails[i - 1][j - 1].parent_j = j;
				}
			}
		}
		if (isValid(i + 1, j + 1) == true) {
			if (isDestination(i + 1, j + 1, dest) == true) {
				cellDetails[i + 1][j + 1].parent_i = i;
				cellDetails[i + 1][j + 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if ((closedList[i + 1][j + 1] == false) && (isUnBlocked(grid, i + 1, j + 1) == true)) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i + 1, j + 1, dest);
				fNew = gNew + hNew;
				if ((cellDetails[i + 1][j + 1].f == FLT_MAX) || (cellDetails[i + 1][j + 1].f > fNew)) {
					openList.insert(make_pair(fNew, make_pair(i + 1, j + 1)));
					cellDetails[i + 1][j + 1].f = fNew;
					cellDetails[i + 1][j + 1].g = gNew;    cellDetails[i + 1][j + 1].h = hNew;
					cellDetails[i + 1][j + 1].parent_i = i; cellDetails[i + 1][j + 1].parent_j = j;
				}
			}
		}
		if (isValid(i + 1, j - 1) == true) {
			if (isDestination(i + 1, j - 1, dest) == true) {
				cellDetails[i + 1][j - 1].parent_i = i;
				cellDetails[i + 1][j - 1].parent_j = j;
				printf("The destination cell is found\n");
				tracePath(cellDetails, dest);
				foundDest = true;
				return;
			}
			else if ((closedList[i + 1][j - 1] == false) && (isUnBlocked(grid, i + 1, j - 1) == true)) {
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i + 1, j - 1, dest);
				fNew = gNew + hNew;
				if ((cellDetails[i + 1][j - 1].f == FLT_MAX)
					|| (cellDetails[i + 1][j - 1].f > fNew)) {
					openList.insert(make_pair(fNew, make_pair(i + 1, j - 1)));
					cellDetails[i + 1][j - 1].f = fNew;
					cellDetails[i + 1][j - 1].g = gNew;    cellDetails[i + 1][j - 1].h = hNew;
					cellDetails[i + 1][j - 1].parent_i = i; cellDetails[i + 1][j - 1].parent_j = j;
				}
			}
		}
	}
	if (foundDest == false) { printf("Failed to find the Destination Cell\n"); }
	return;
}
//****************************************************************************************//
int main() {
	char grid[512][512];
	ifstream map_file("Aftershock.map");
	if (map_file.is_open()) {
		for (int i = 0; i < 512; i++) {
			for (int j = 0; j < 512; j++) { map_file >> grid[j][i]; }
		}
	}
	Pair src = make_pair(250, 120);
	Pair dest = make_pair(256, 124);
	auto start = high_resolution_clock::now();
	aStarSearch(grid, src, dest);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << duration.count() << endl;
	return (0);
}
//****************************************************************************************//End