#include "SubstitutionPermutationNetwork.h"

template<int blockSize>
SubstitutionPermutationNetwork<blockSize>::SubstitutionPermutationNetwork(bitset<32> key,
																		  SBox sBox,
																		  PBox<blockSize> pBox)
{
	this->key = key;
	this->sBox = sBox;
	this->pBox = pBox;
}
