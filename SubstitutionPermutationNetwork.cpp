#include "SubstitutionPermutationNetwork.h"
#include "math.h"

template<int blockSize, int keySize>
SubstitutionPermutationNetwork<blockSize, keySize>::SubstitutionPermutationNetwork(bitset<keySize> key,
																		  SBox sBox,
																		  PBox<blockSize> pBox)
{

	this->key = key;
	this->sBox = sBox;
	this->pBox = pBox;
}

template<int blockSize, int keySize>
bitset<blockSize> SubstitutionPermutationNetwork<blockSize, keySize>::encrypt(bitset<blockSize> plaintext)
{
	int rounds = (key.count() - blockSize) / 4;

	for(int round = 0; round < rounds; round++)
	{
		bitset<blockSize> roundKey = getRoundKey(round);
		whiten(plaintext, roundKey);
		substitute(plaintext);
		permute(plaintext);
	}

	whiten(plaintext, getRoundKey(rounds));
	substitue(plaintext);

	return plaintext;
}

template<int blockSize, int keySize>
bitset<blockSize> SubstitutionPermutationNetwork<blockSize, keySize>::getRoundKey(int round)
{
	bitset<blockSize> roundKey;

	for(int i = 0; i < blockSize; i++)
	{
		roundKey[i] = key[(i + round * 4) % keySize];
	}

	return roundKey;
}

template<int blockSize, int keySize>
void SubstitutionPermutationNetwork<blockSize, keySize>::whiten(bitset<blockSize> &state, bitset<blockSize> &roundkey)
{
	state ^= roundkey;
}
