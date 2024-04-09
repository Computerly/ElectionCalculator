#include "Combinations.h"
#include <iostream>

using namespace std;

Combinations::Combinations() {}
Combinations::~Combinations() {}

int Combinations::indexOf(int ranking, int combinations){
	return binarySearch(COMBINATIONS[combinations - 1], 0, sizes[combinations - 1], ranking);
}

int Combinations::sizeOf(unsigned short combinations){
	return sizes[combinations - 1];
}
int Combinations::getRankingNumber(unsigned int index, unsigned int combinations){
	return COMBINATIONS[combinations - 1][index];
}

int Combinations::binarySearch(const int arr[], int l, int r, int &ranking)
{
    if (r >= l) {
        int idx = l + (r - l) / 2;

        if (arr[idx] == ranking) { return idx; }

        if (arr[idx] > ranking)  { return binarySearch(arr, l, idx - 1, ranking); }

        return binarySearch(arr, idx + 1, r, ranking);
    }
    return -1;
}
	// ****************************************************************************
	// *                               1 Combination                              *
	// ****************************************************************************
	constexpr int Combinations::COMBINATIONS_1[];

	// ****************************************************************************
	// *                              4 Combinations                              *
	// ****************************************************************************
	constexpr int Combinations::COMBINATIONS_2[];
	
	// ****************************************************************************
	// *                              15 Combinations                             *
	// ****************************************************************************
	constexpr int Combinations::COMBINATIONS_3[];

	// ****************************************************************************
	// *                              64 Combinations                             *
	// ****************************************************************************
	constexpr int Combinations::COMBINATIONS_4[];

	// ****************************************************************************
	// *                             325 Combinations                             *
	// ****************************************************************************
	constexpr int Combinations::COMBINATIONS_5[];

	// ****************************************************************************
	// *                             1956 Combinations                            *
	// ****************************************************************************
	constexpr int Combinations::COMBINATIONS_6[];

	// ****************************************************************************
	// *                           Array of Combinations                          *
	// ****************************************************************************
	constexpr const int *Combinations::COMBINATIONS[];

	// ****************************************************************************
	// *                  Array of lengths of combination arrays                  *
	// ****************************************************************************
	constexpr int Combinations::sizes[];
