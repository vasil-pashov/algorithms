// Link to task: https://action.informatika.bg/problems/67
#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>

using namespace std;

bool differentDigits(int64_t number) {
	bool used[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	while (number > 0) {
		const int last = number % 10;
		if (used[last]) {
			return false;
		} else {
			used[last] = 1;
		}
		number /= 10;
	}
	return true;
}

int64_t findNextDummy(int64_t number) {
	while (number <= 9876543210) {
		number++;
		if (differentDigits(number)) {
			return number;
		}
	}
	return -1;
}

const int t[] = {-1, 10, 102, 1023, 10234, 102345, 1023456, 10234567, 102345678, 1023456789};

bool largerExists(int64_t n, int64_t current, bool used[10], int numDigits, int usedDigits) {
	bool usedTmp[10];
	for (int i = 0; i < 10; ++i) usedTmp[i] = used[i];
	while (usedDigits < numDigits) {
		for (int i = 9; i >= 0; --i) {
			if (usedTmp[i] == false) {
				current *= 10;
				current += i;
				usedTmp[i] = true;
				break;
			}
		}
		usedDigits++;
	}
	return current > n;
}

/// To find the next number we start from the right most digit and go left
/// We mark taken digits with *, assume some random number ***
/// Add new digit(%) ***%
/// - % is already used, we need to change % with the largest unused digit
///   this is done to assure that the new number will be larger than the initial one.
///   after the first digit is used we continue adding digits in increasing order
/// - % is not used but we cannot create a number larger than the initial with prefix ***%
///   this means that we need to remove % and replace it with the highest unused digit then
///   proceed with adding digits in increasing order
int64_t findNext(int64_t number) {
	const int64_t n = number;
	bool used[10] = {0, 0, 0, 0 ,0, 0, 0, 0, 0 ,0};
	int arr[10] = {0, 0, 0, 0 ,0, 0, 0, 0, 0 ,0};
	// count digits
	int numDigits = 0;
	while (number > 0) {
		const int last = number % 10;
		arr[numDigits++] = last;
		number /= 10;
	}
	if (n == 0) numDigits = 1;

	int64_t current = 0;

	if (!largerExists(n, current, used, numDigits, 0)) {
		return t[numDigits];
	}

	for (int i = numDigits - 1, usedDigits = 1; i >= 0; --i, usedDigits++) {
		const int currentDigit = arr[i];
		bool needsChange = false;

		if (used[currentDigit]) {
			needsChange = true;
		}

		bool old = used[currentDigit];
		used[currentDigit] = true;
		if (!largerExists(n, current * 10 + currentDigit, used, numDigits, usedDigits)) {
			needsChange = true;
		}
		used[currentDigit] = old;

		if (needsChange) {
			for (int j = currentDigit + 1; j <= 9; ++j) {
				if (used[j] == false) {
					current *= 10;
					current += j;
					used[j] = true;
					break;
				}
			}
			--i;

			while (i >= 0) {
				for (int j = 0; j <= 9; ++j) {
					if (used[j] == false) {
						used[j] = true;
						current *= 10;
						current += j;
						break;
					}
				}
				--i;
			}
			return current;
		} else {
			used[currentDigit] = true;
			current *= 10;
			current += currentDigit;
		}
	}
	return -1;
}

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		int64_t number;
		cin >> number;
		if (number >= 9876543210) {
			cout << -1 << '\n';
		} else {
			// const int dummy = findNextDummy(number);
			const int64_t next = findNext(number);
			cout << next << '\n';
		}
	}
}