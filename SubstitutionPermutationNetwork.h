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
	// I don't fully understand what this line does; stackoverflow suggested it as a fix: https://stackoverflow.com/questions/37301160/invalid-operands-to-binary-expression-const-and-const
	struct comp { bool operator()(const bitset<4>& a, const bitset<4>& b) const { return a.to_ulong() < b.to_ulong(); }};
	map<bitset<4>, bitset<4>, comp> mappings;
};

template <int blockSize> class PBox
{
public:
	PBox(const int mappings[blockSize]);
	void permute(bitset<blockSize> &state);
private:
	int mappings[blockSize];
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
