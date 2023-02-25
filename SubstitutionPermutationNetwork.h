#ifndef SUBSTITUTION_PERMUTATION_NETWORK_SUBSTITUTIONPERMUTATIONNETWORK_H
#define SUBSTITUTION_PERMUTATION_NETWORK_SUBSTITUTIONPERMUTATIONNETWORK_H

#include <vector>
#include "bitset"
#include "map"

using namespace std;

class SBox
{
public:
	SBox() {};
	SBox(map<int, int> mappings);
	void substitute(bitset<4> &state);
private:
	// I don't fully understand what this line does; stackoverflow suggested it as a fix: https://stackoverflow.com/questions/37301160/invalid-operands-to-binary-expression-const-and-const
	struct comp { bool operator()(const bitset<4>& a, const bitset<4>& b) const { return a.to_ulong() < b.to_ulong(); }};
	map<bitset<4>, bitset<4>, comp> mappings;
};

template <int blockSize> class PBox
{
public:
	PBox() {};
	PBox(vector<int> mappings);
	void permute(bitset<blockSize> &state);
private:
	vector<int> mappings;
};


template <int blockSize, int keySize> class SubstitutionPermutationNetwork
{
public:
	SubstitutionPermutationNetwork() {};
	SubstitutionPermutationNetwork(bitset<keySize> key, SBox, PBox<blockSize>);

	bitset<blockSize> encrypt(bitset<blockSize> plaintext);
	bitset<blockSize> decrypt(bitset<blockSize> ciphertext);
	void setKey(bitset<blockSize> newKey);
private:
	bitset<keySize> key;
	SBox sBox;
	PBox<blockSize> pBox;

	bitset<blockSize> getRoundKey(int round);
	bitset<blockSize> whiten(bitset<blockSize> state, bitset<blockSize> key);
	bitset<blockSize> substitute(bitset<blockSize>);
	bitset<blockSize> permute(bitset<blockSize>);
};



/**
 * Constructor.
 *
 * @tparam blockSize the network's block size.
 * @tparam keySize the networks key size.
 * @param key the network's key.
 * @param sBox the s-box to use for substitutions.
 * @param pBox the permutation table to use for permuting.
 */
 template<int blockSize, int keySize>
SubstitutionPermutationNetwork<blockSize, keySize>::SubstitutionPermutationNetwork(bitset<keySize> key,
																				   SBox sBox,
																				   PBox<blockSize> pBox)
{

	this->key = key;
	this->sBox = sBox;
	this->pBox = pBox;
}

/**
 * Returns the ciphertext which is a result of encrypting the given plaintext ith this network's key.
 *
 * @tparam blockSize the block size for this network.
 * @tparam keySize the key size for this network.
 * @param plaintext the plaintext to encrypt.
 * @return the ciphertext.
 */
template<int blockSize, int keySize>
bitset<blockSize> SubstitutionPermutationNetwork<blockSize, keySize>::encrypt(bitset<blockSize> plaintext)
{
	// Each round key will move through the key by four bits for each round. This expression gets the number of rounds
	// which will happen under this key schedule.
	int rounds = (key.size() - blockSize) / 4;

	cout << rounds << " rounds" << endl;

	bitset<blockSize> state = plaintext;

	// Perform the iterations over the network.
	for(int round = 0; round < rounds - 1; round++)
	{
		cout << "State at the start of round: " << state.to_string() << endl;
		cout << "Round " << round << endl;
		bitset<blockSize> roundKey = getRoundKey(round);
		cout << "Round Key: " << roundKey.to_string() << endl;
		state = whiten(state, roundKey);
		cout << "State after whitening: " << state.to_string() << endl;
		state = substitute(state);
		cout << "State after s: " << state.to_string() << endl;
		state = permute(state);
		cout << "State after p: " << state.to_string() << endl;
	}

	state = whiten(state, getRoundKey(rounds));
	state = substitute(state);

	return state;
}

/**
 * Returns the round key for the given round.
 * Each round key shifts through the encryption key by 4 bits.
 *
 * @param round the round of encrytption
 * @return
 */
template<int blockSize, int keySize>
bitset<blockSize> SubstitutionPermutationNetwork<blockSize, keySize>::getRoundKey(int round)
{
	bitset<blockSize> roundKey;

	for(int i = blockSize; i >= 0; i--)
	{
		roundKey[i] = key[(blockSize + (i - (round * 4))) % keySize];
	}

	return roundKey;
}

/**
 * Perform key whitening by xor-ing.
 *
 * @param state the state to whiten.
 * @param roundkey the round key to use for whitening.
 */
template<int blockSize, int keySize>
bitset<blockSize> SubstitutionPermutationNetwork<blockSize, keySize>::whiten(bitset<blockSize> state, bitset<blockSize> roundkey)
{
	state ^= roundkey;
	return state;
}

/**
 * Breaks the state up into (blockSize / 4) nibbles and runs them all through the s-box.
 *
 * @param state the state to substitute.
 */
template<int blockSize, int keySize>
bitset<blockSize> SubstitutionPermutationNetwork<blockSize, keySize>::substitute(bitset<blockSize> state)
{
	bitset<4> nibble;

	for(int i = 0; i < blockSize; i += 4)
	{
		for(int j = 0; j < 4; j++) nibble[j] = state[i + j];

		sBox.substitute(nibble);

		for(int j = 0; j < 4; j++) state[i + j] = state[j];
	}

	return state;
}

/**
 * Puts the state through the p-box
 *
 * @param state the state to permute.
 */
template<int blockSize, int keySize>
bitset<blockSize> SubstitutionPermutationNetwork<blockSize, keySize>::permute(bitset<blockSize> state)
{
	pBox.permute(state);
	return state;
}

/**
 * Constructor which converts given int-to-int mapping to a bitset to bitset mapping.
 *
 * @param mappings an int-to-int map which describes the s-box
 */
SBox::SBox(map<int, int> mappings)
{
	for (auto [key, val] : mappings) {
		this->mappings[bitset<4>(key)] = bitset<4>(val);
	}
}

/**
 * Performs a substitution on the given 4 bits of state.
 *
 * @param state 4 bits of state to be substituted.
 */
void SBox::substitute(bitset<4> &state)
{
	state = mappings[state];
}

/**
 * Constructor which assigns to the mappings for the permutations.
 *
 * @tparam blockSize the block size of the SPN that the permutation is in.
 * @param mappings the permutation mappings.
 */
template<int blockSize>
PBox<blockSize>::PBox(vector<int> mappings)
{
	this->mappings = mappings;
}

/**
 * Permutes the bits in the given state.
 *
 * @param state the state to permute.
 */
template<int blockSize>
void PBox<blockSize>::permute(bitset<blockSize> &state)
{
	bitset<blockSize> old = state;

	for(int bit = 0; bit < blockSize; bit++)
	{
		state[bit] = old[this->mappings[bit]];
	}
}

#endif //SUBSTITUTION_PERMUTATION_NETWORK_SUBSTITUTIONPERMUTATIONNETWORK_H
