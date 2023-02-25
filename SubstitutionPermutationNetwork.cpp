#include "SubstitutionPermutationNetwork.h"
#include "math.h"

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
	int rounds = (key.count() - blockSize) / 4;

	// Perform the iterations over the network.
	for(int round = 0; round < rounds - 1; round++)
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

	for(int i = 0; i < blockSize; i++)
	{
		roundKey[i] = key[(i + round * 4) % keySize];
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
void SubstitutionPermutationNetwork<blockSize, keySize>::whiten(bitset<blockSize> &state, bitset<blockSize> &roundkey)
{
	state ^= roundkey;
}
