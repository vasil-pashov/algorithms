// Problem link: https://action.informatika.bg/problems/201

// Solution use Eratosthenes to precompute all primes and then use
// binary search to check if a number is prime
#include <vector>
#include <iostream> // cin, cout
#include <cctype> // int64_t
#include <algorithm> // binary_search
#include <cmath> // sqrt

using namespace std;

/// @brief Sieve of Eratosthenes
/// @param out Array where the primes will be placed in sorted order
/// @param n The number up to which primes will be generated
void genPrimes(std::vector<int>& out, int n) {
	std::vector<bool> composite(n, 0);
	for (int64_t i = 2; i < n; ++i) {
		if (composite[i] == 0) {
			out.push_back(i);
			composite[i] = 1;
			for (int64_t j = i * i; j < n; j += i) {
				composite[j] = 1;
			}
		}
	}
}

/// Used by the sieve of Eratosthenes, maxPrimes[i] is the largest number
/// with i digits. The sieve will look for primes up to maxPrimes[i]
const int maxPrimes[] = {0, 9, 99, 999, 9999, 99999, 999999, 9999999};

/// We need to count only unique primes. Keep a book of which primes were
/// already counted
std::vector<bool> used;

/// @brief Perform binary search in an array of prime numbers to check if n is prime
/// @param primes Sorted array of prime numbers
/// @param n The number to check
/// @return 1 if n is prime 0 otherwise
bool isPrime(const std::vector<int>& primes, int n) {
	return std::binary_search(primes.begin(), primes.end(), n);
}

/// @brief Check if n is prime by iterating over it's divisors
/// @param n The number to check
/// @return 1 if n is prime 0 otherwise
bool isPrime(int n) {
	for (int i = std::sqrt(n); i > 1; --i) {
		if (n % i == 0) return false;
	}
	return true;
}

/// @brief Count all primes in the matrix, as per task statement
/// @param matrix The matrix with all numbers to check
/// @param number The current number (when it is of length m we will perform a check)
/// @param row The current row
/// @param m Max rows
/// @param n Max cols
/// @param primes Sorted list of all primes up to 10^m - 1
/// @return The count of all unique primes in the matrix (as per task statement)
int countPrimes(int matrix[7][7], int number, int row, int m, int n, std::vector<int>& primes) {
	if (row == m) {
		if (used[number]) {
			return 0;
		} else {
			if (isPrime(primes, number)) {
				used[number] = 1;
				return 1;
			}
			return 0;
		}
	}
	int primeCount = 0;
	for (int i = 0; i < n; ++i) {
		primeCount += countPrimes(matrix, number * 10 + matrix[row][i], row + 1, m, n, primes);
	}
	return primeCount;
}

int matrix[7][7];

int main() {
	std::vector<int> primes;
	int m, n;
	cin >> m >> n;
	for (int i = 0; i < m; ++i) {
		int row;
		cin >> row;
		int div = 10;
		for (int j = n - 1; j >= 0; --j) {
			int last = row % 10;
			row /= 10;
			matrix[i][j] = last;
		}
	}
	int maxPrime = maxPrimes[m];
	primes.reserve(maxPrime);
	used.resize(maxPrime, 0);
	genPrimes(primes, maxPrime);

	cout << countPrimes(matrix, 0, 0, m, n, primes) << '\n';

	return 0;
}