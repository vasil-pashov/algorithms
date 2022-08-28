// Link to task: https://action.informatika.bg/problems/67
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <array>
#include <cstring>
const int mod = 1000000007;
const int MAXN = 100;

/// @brief Convert two dimensional coordinates to graph node indes
/// @param row The row in the matrix (x coordinate)
/// @param col The col in the matrix (y coordinate)
/// @param rowCnt The total number of rows in the matrix
/// @param colCnt The total number of columns in the matrix
/// @return Integer index of the (row, col)
int nodeIndex(const int row, const int col, const int rowCnt, const int colCnt) {
	return row * colCnt + col;
}


/// @brief Convert integer index in a matrix to row and col pair
/// @param nodeIndex The 1D index to be converted
/// @param rowCnt Total number of rows in the matrix
/// @param colCnt Total number of cols in the matrix
/// @return Pair containing the row and col of the index
inline std::pair<int, int> coordsFromNodeIndex(const int nodeIndex, const int rowCnt, const int colCnt) {
	return std::make_pair<int, int>(nodeIndex / colCnt, nodeIndex % colCnt);
}

/// @brief Read the input and then solve the problem via BFS
/// This uses a small modification of the BFS algorithm, it does not
/// mark the node as used when it's put into the queue, it marks it as
/// used when it's popped from the queue, this we can have the same node in
/// the queue several times. We also keep an array which counts how many times
/// node i was reached by any of its neighbors.
/// @note This solution is for 75/100 points. The failing test cases are due to time limit.
/// @return How total count of paths with minimal length are there to the end node
int bfsSolve() {
	int n, m;
	std::cin >> n >> m;
	// Initialize adjacency matrix
	const int nodeCount = m * n;

	// Read the input
	int start, end;
	char pattern[MAXN][MAXN];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			char symbol;
			std::cin >> symbol;
			if (symbol == 'E') {
				start = nodeIndex(i, j, n, m);
			} else if (symbol == 'X') {
				end = nodeIndex(i, j, n, m);
			}
			pattern[i][j] = symbol;
		}
	}

	std::vector<int> pathCnt(nodeCount, 0);
	std::vector<bool> used(nodeCount, 0);
	std::queue<int> bfsQueue;
	bfsQueue.push(start);
	while (!bfsQueue.empty()) {
		const int currentNode = bfsQueue.front();
		bfsQueue.pop();
		used[currentNode] = true;
		if (currentNode == end) {
			break;
		}

		std::pair<int, int> coords = coordsFromNodeIndex(currentNode, n, m);
		if (coords.first > 0) {
			const int symbol = pattern[coords.first - 1][coords.second];
			if ((symbol == '.' || symbol == 'X') && !used[currentNode - m]) {
				bfsQueue.push(currentNode - m);
				pathCnt[currentNode - m]++;
			}
		}
		if (coords.first < n - 1) {
			const int symbol = pattern[coords.first + 1][coords.second];
			if ((symbol == '.' || symbol == 'X') && !used[currentNode + m]) {
				bfsQueue.push(currentNode + m);
				pathCnt[currentNode + m]++;
			}
		}

		if (coords.second > 0) {
			const int symbol = pattern[coords.first][coords.second- 1];
			if ((symbol == '.' || symbol == 'X') && !used[currentNode - 1]) {
				bfsQueue.push(currentNode - 1);
				pathCnt[currentNode - 1]++;
			}
		}
		if (coords.second < m - 1) {
			const int symbol = pattern[coords.first][coords.second + 1];
			if ((symbol == '.' || symbol == 'X') && !used[currentNode + 1]) {
				bfsQueue.push(currentNode + 1);
				pathCnt[currentNode + 1]++;
			}
		}
	}

	return pathCnt[end];
}

/// @brief Used by the DPSolve function. A straightforward DP counting with memoization.
/// Counts how many paths are there from the current node -- node to the end.
/// @param node The node from we count how many paths are there to the end
/// @param n Max rows in the matrix
/// @param m Maxs cols in the matrix
/// @param endToStart Array whose i,j entry is the length of the shortest path from
/// the end of the maze to i, j
/// @param cache Memoization table for the number of paths
/// @return 
int count(std::pair<int, int> node, int n, int m, int endToStart[MAXN][MAXN], int cache[MAXN][MAXN]) {
	if (node.first >= n || node.second >= m || node.first < 0 || node.second < 0) {
		return 0;
	}

	if (endToStart[node.first][node.second] == 0) {
		return 1;
	}

	const std::array<std::pair<int, int>, 4> moves = {
		std::make_pair(-1, 0),
		std::make_pair(1, 0),
		std::make_pair(0, -1),
		std::make_pair(0, 1)
	};

	const int currentToEnd = endToStart[node.first][node.second];
	int cnt = 0;
	for (const auto& move : moves) {
		const std::pair<int, int> child(node.first + move.first, node.second + move.second);
		const int childToStart = endToStart[child.first][child.second];
		if (childToStart != -1 && childToStart == currentToEnd - 1) {
			int& memo = cache[child.first][child.second];
			if (memo == -1) {
				memo = count(child, n, m, endToStart, cache);
			}
			cnt = (cnt + memo) % mod;
		}
	}
	return cnt;
}

/// @brief This will combine BFS + Dynamic programming
/// The tasks asks to count all possible ways to do something. This sounds like a DP problem.
/// The difficult part is to figure out when we can call the counting routine recursively.
/// If we start a bfs from the beginning of the maze we will know the shortest path, but we
/// won't be able to know if we can reach the end from a given field. However, we can start the
/// BFS from the end to the beginning and write down in each cell the length of the path from the
/// end. Then we start counting from the beginning. Assume we're at position (i, j). We know that
/// a child of (i, j) is on a path reaching the end iff the length of the path (starting from the end)
/// decreases by one from (i, j) to the child. This is the condition under which we can invoke the
/// counting function recursively.
void DPSolve() {
	int n, m;
	std::cin >> n >> m;
	// X - end
	// E - beginning
	// . - can pass
	// # - cannot pass
	std::pair<int, int> start;
	std::pair<int, int> end;
	char pattern[MAXN][MAXN];
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			std::cin >> pattern[i][j];
			if (pattern[i][j] == 'E') {
				start = {i, j};
			} else if (pattern[i][j] == 'X') {
				end = {i, j};
			}
		}
	}

	const std::array<std::pair<int, int>, 4> moves = {
		std::make_pair(-1, 0),
		std::make_pair(1, 0),
		std::make_pair(0, -1),
		std::make_pair(0, 1)
	};

	int endToStart[MAXN][MAXN];
	memset(endToStart, -1, MAXN * MAXN * sizeof(int));

	endToStart[end.first][end.second] = 0;
	std::queue<std::pair<int, int>> q;
	q.push(end);
	while (!q.empty()) {
		const std::pair<int, int> parent = q.front();
		q.pop();
		
		const int pathLength = endToStart[parent.first][parent.second];
		for (const auto& move : moves) {
			const std::pair<int, int> child(parent.first + move.first, parent.second + move.second);
			if (child.first >= n || child.second >= m || child.first < 0 || child.second < 0) {
				continue;
			}

			const char symbol = pattern[child.first][child.second];
			if (symbol == '#') {
				continue;
			}

			int& lengthAtChild = endToStart[child.first][child.second];
			if ((symbol == '.' || symbol == 'E') && lengthAtChild == -1) {
				q.push(child);
				lengthAtChild = pathLength + 1;
			}
		}
	}

	int cache[MAXN][MAXN];
	memset(cache, -1, MAXN * MAXN * sizeof(int));

	std::cout << count(start, n, m, endToStart, cache);
}

int main() {
	DPSolve();
}