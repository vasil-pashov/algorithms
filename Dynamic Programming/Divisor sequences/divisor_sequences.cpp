// Link to problem: https://action.informatika.bg/problems/33
#include <cstdio>
#include <cstring> // memset
#include <vector>

using namespace std;

const int mod = 1000000007;
const int maxN = 1001;
const int maxK = 1001;

int countMemoization(int n, int k, int lastDigit);

/// Count all sequences recursively. This will create all possible
/// sequences.
/// @param[in] n The remaining length of the current sequence
/// @param[in] k The largest number which can be used in the sequence
/// @param[in] lastDigit The last selected digit in the sequence. The next number
/// is selected based on lastDigit (if i % lastDigit || lastDigit % i -> nextNumber = i) 
int countBruteForce(int n, int k, int lastDigit) {
    if (n == 0) {
        return 1;
    }
    int count = 0;
    for (int i = 1; i <= k; ++i) {
        if (i % lastDigit == 0 || lastDigit % i == 0) {
            count += countBruteForce(n - 1, k, i) % mod;
        }
    }
    return count;
}

/// Table caching the divisors of each number.
/// At position divisors[i] we will store a vector
/// off all numbers which divide i
vector<int> divisors[maxK];

/// Global memoization table:
/// memoizationTable[i][j] is the count of sequences of length i with last digit j
/// which match the condition of the task
int memoizationTable[maxN][maxK];

/// Check if the memoization table has entry for sequence of length n
/// which ends with nextDigit. If there is such entry return it otherwise
/// compute it, store it in the table and return it
int fetchFromMemoizationTable(int n, int k, int nextDigit) {
    int& memo = memoizationTable[n][nextDigit];
    if (memo == 0) {
        memo = countMemoization(n - 1, k, nextDigit);
    }
    return memo;
}

/// Count all sequences using the memoization table. This won't compute all sequences.
/// @param[in] n The remaining length of the current sequence
/// @param[in] k The largest number which can be used in the sequence
/// @param[in] lastDigit The last selected digit in the sequence. The next number
/// is selected based on lastDigit (if i % lastDigit || lastDigit % i -> nextNumber = i) 
int countMemoization(int n, int k, int lastDigit) {
    if (n == 0) {
        return 1;
    }
    int count = 0;

    // The if/else below will compute the counts for all sequences which match the first
    // condition lastDigit % nextDigit == 0
    if (divisors[lastDigit].empty()) {
        // In case we don't know which numbers divide lastDigit we will compute them
        // and store them in the divisors table
        for (int nextDigit = 1; nextDigit <= lastDigit; ++nextDigit) {
            if (lastDigit % nextDigit == 0) {
                divisors[lastDigit].push_back(nextDigit);
                const int memo = fetchFromMemoizationTable(n, k, nextDigit);
                count = (memo + count) % mod;
            }
        }
    } else {
        // In this case we have a list of all numbers which divide lastDigit so instead
        // of trying each number in range [1; lastDigit] we will use the list and iterate
        // only over numbers which we know for sure divide lastDigit
        for (const int divisor : divisors[lastDigit]) {
            const int nextDigit = divisor;
            const int memo = fetchFromMemoizationTable(n, k, nextDigit);
            count = (memo + count) % mod;
        }
    }

    // This while loop will continue the sequence with the second condition
    // nextDigit % lastDigit == 0. We know that the numbers which can be divided
    // by lastDigit are: lastDigit * 2, lastDigit * 3, ... so we will loop only
    // over those values. There is no need to iterate the whole sequence [lastDigit;k].
    int nextDigit = lastDigit * 2;
    while (nextDigit <= k) {
        const int memo = fetchFromMemoizationTable(n, k, nextDigit);
        count = (memo + count) % mod;
        nextDigit += lastDigit;
    }

    return count;
}

/// Test to see if brute force solution and memoization match
void test() {
    const int testMaxN = 10;
    const int testMaxK = 20;
    for (int i = 1; i <= testMaxK; ++i) {
        divisors[i].reserve(testMaxK);
    }
    for (int n = 1; n < testMaxN; ++n) {
        for (int k = 1; k < testMaxK; ++k) {
            memset(memoizationTable, 0, sizeof(memoizationTable[0][0]) * maxN * maxK);
            const int bruteForceRez = countBruteForce(n, k, 1);
            const int memoRez = countMemoization(n, k, 1);
            if (bruteForceRez != memoRez) {
                printf("Error: n = %d, k = %d, bruteForce = %d, memo = %d\n", n, k, bruteForceRez, memoRez);
            }
        }
    }
    printf("Tests done");
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    // Reserve memory for divisors cache in advance because it's faster
    for (int i = 1; i <= k; ++i) {
        divisors[i].reserve(k);
    }
    const int rezult = countMemoization(n, k, 1);
    printf("%d\n", rezult);

    // test();
}