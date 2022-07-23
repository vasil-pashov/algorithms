// Link to task: https://action.informatika.bg/problems/128

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cctype>

using namespace std;

int64_t merge(
	std::vector<int>& source,
	std::vector<int>& dest,
	int start,
	int end,
	int mid
) {
	int64_t inversions = 0;
	int left = start;
	int right = mid + 1;
	int k = start;
	while (left <= mid && right <= end) {
		if (source[left] <= source[right]) {
			dest[k++] = source[left++];
		} else {
			dest[k++] = source[right++];
			// Every time something from the right part has to be taken in the main
			// array count how many elements did it "skip over" (i.e. the number of
			// unprocessed elements from the left side)
			inversions += (mid + 1 - left);
		}
	}
	while (left <= mid) dest[k++] = source[left++];
	while (right <= end) dest[k++] = source[right++];
	return inversions;
}

int64_t mergeSortHelper(
	std::vector<int>& source,
	std::vector<int>& dest,
	int start,
	int end
) {
	if (start == end) {
		return 0;
	}
	const int mid = (start + end) / 2;
	int64_t inversions = mergeSortHelper(dest, source, start, mid);
	inversions += mergeSortHelper(dest, source, mid + 1, end);
	inversions += merge(source, dest, start, end, mid);
	return inversions;
}

int64_t mergeSort(std::vector<int>& seatings) {
	std::vector<int> helper = seatings;
	return mergeSortHelper(helper, seatings, 0, seatings.size() - 1);
}

int main() {
	int n;
	cin >> n;
	std::vector<int> seatings(n);
	for (int i = 0; i < n; ++i) {
		int el;
		scanf("%d", &el);
		seatings[i] = el;
	}
	int64_t swapCount = mergeSort(seatings);
	printf("%lld\n", swapCount);
	return 0;
}