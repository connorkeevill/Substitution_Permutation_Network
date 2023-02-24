#ifndef SUBSTITUTION_PERMUTATION_NETWORK_SUBSTITUTIONPERMUTATIONNETWORK_H
#define SUBSTITUTION_PERMUTATION_NETWORK_SUBSTITUTIONPERMUTATIONNETWORK_H

#include "bitset"
#include "map"

using namespace std;

class SBox
{
public:
	SBox(map<char, char> mappings);
	void substitute(bitset<4> &state);
private:
	map<bitset<4>, bitset<4>> mappings;
};

template <int blockSize> class PBox
{
public:
	PBox(map<bitset<blockSize>, bitset<blockSize>> mappings);
	void permute(bitset<blockSize> &state);
private:
	map<bitset<blockSize>, bitset<blockSize>> mappings;
};


template <int blockSize, int keySize> class SubstitutionPermutationNetwork
{
public:
	SubstitutionPermutationNetwork(bitset<keySize> key, SBox, PBox<blockSize>);
	bitset<blockSize> encrypt(bitset<blockSize> plaintext);
	bitset<blockSize> decrypt(bitset<blockSize> ciphertext);
	void setKey(bitset<blockSize> newKey);
private:
	bitset<keySize> key;
	SBox sBox;
	PBox<blockSize> pBox;

	bitset<blockSize> getRoundKey(int round);
	void whiten(bitset<blockSize> &state, bitset<blockSize> &key);
	void substitute(bitset<blockSize> &);
	void permute(bitset<blockSize> &);
};


#endif //SUBSTITUTION_PERMUTATION_NETWORK_SUBSTITUTIONPERMUTATIONNETWORK_H
