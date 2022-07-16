// Link to task: https://action.informatika.bg/problems/45
#include <iostream>
#include <unordered_set>
#include <cstring>

using namespace std;


/// @brief Count all shorthands.
/// @param word A word to create shorthands for
/// @return The number of shorthands
int printAllShorthands(const string& word) {
	string shorthand;
	shorthand.resize(word.size() + 1);
	unordered_set<string> s;
	// Outer loop selects the start of the 'cut interval'. This is the
	// interval of letters which will be omitted. It is [cutStart; cutEnd]
	// (inclusive on both sides).
	for (int cutStart = 0; cutStart < word.size(); ++cutStart) {
		// Copy the first half of the word
		memcpy(&shorthand[0], &word[0], cutStart);
		for (int cutEnd = cutStart; cutEnd < word.size(); ++cutEnd) {
			const int toCopy = word.size() - cutEnd - 1;
			// Skip the cut interval and copy from cutEnd until the last char
			memcpy(&shorthand[cutStart], &word[cutEnd + 1], toCopy);
			// Put terminating 0 so that string comparison used by the set knows
			// when the word ends.
			shorthand[cutStart + toCopy] = '\0';
			s.insert(shorthand);
		}
	}
	// Subtract one in order to account for the empty string which will be
	// generated with cutStart = 0 and cutEnd = word.size() - 1
	return s.size() - 1;
}

int main() {
	int n;
	string word;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> word;
		cout << printAllShorthands(word) << '\n';
	}
}