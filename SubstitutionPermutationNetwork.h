#ifndef SUBSTITUTION_PERMUTATION_NETWORK_SUBSTITUTIONPERMUTATIONNETWORK_H
#define SUBSTITUTION_PERMUTATION_NETWORK_SUBSTITUTIONPERMUTATIONNETWORK_H

#include "bitset"

using namespace std;

class SBox
{

};

class PBox
{

};


template <int blockSize> class SubstitutionPermutationNetwork
{
public:
	SubstitutionPermutationNetwork(bitset<blockSize> key);
	bitset<blockSize> encrypt(bitset<blockSize> plaintext);
	bitset<blockSize> decrypt(bitset<blockSize> ciphertext);
	void setKey(bitset<blockSize> newKey);

private:
	bitset<blockSize> key;
	SBox sBox;
	PBox pBox;

	void permute(bitset<blockSize> *);
	void substitute(bitset<blockSize> *);
};


#endif //SUBSTITUTION_PERMUTATION_NETWORK_SUBSTITUTIONPERMUTATIONNETWORK_H
