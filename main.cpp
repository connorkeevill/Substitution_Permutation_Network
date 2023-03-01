#include <iostream>
#include "SubstitutionPermutationNetwork.h"

int main()
{
	const int BLOCKSIZE = 16;
	const int KEYSIZE = 32;

	SBox sbox {
		{{0x0, 0x4},
		 {0x1, 0x1},
		 {0x2, 0xE},
		 {0x3, 0x8},
		 {0x4, 0xD},
		 {0x5, 0x6},
		 {0x6, 0x2},
		 {0x7, 0xB},
		 {0x8, 0xF},
		 {0x9, 0xC},
		 {0xA, 0x9},
		 {0xB, 0x7},
		 {0xC, 0x3},
		 {0xD, 0xA},
		 {0xE, 0x5},
		 {0xE, 0x5},
		 {0xF, 0x0}}
	};
	vector<int> permutations {0, 4, 8, 12, 1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15};
	PBox<BLOCKSIZE> permutation = PBox<BLOCKSIZE>{permutations};

	SubstitutionPermutationNetwork<BLOCKSIZE, KEYSIZE> SPN =
			SubstitutionPermutationNetwork<BLOCKSIZE, KEYSIZE>{bitset<KEYSIZE>{0b11100111011001111001000000111101}, sbox, PBox<BLOCKSIZE>{permutations}};
	bitset<BLOCKSIZE> plaintext = bitset<BLOCKSIZE>{0b0100111010100001};
	cout << SPN.encrypt(plaintext).to_string() << endl;
	return 0;
}
