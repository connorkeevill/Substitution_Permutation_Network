#include "SubstitutionPermutationNetwork.h"

template<int blockSize>
SubstitutionPermutationNetwork<blockSize>::SubstitutionPermutationNetwork(bitset<blockSize> key,
																		  SBox sBox,
																		  PBox<blockSize> pBox)
{
	this->key = key;
	this->sBox = sBox;
	this->pBox = pBox;
}
