#pragma once

#include <string>
#include <vector>
#include <stdint.h>

using namespace std;

/* TYPEDEFS
   ======== */

typedef uint32_t word;
typedef uint64_t longword;


/* CLASS DEFINITIONS
   ================= */

   // Start of class SHA256 


class SHA256
{
protected:
	// Protected class variables
	static const unsigned short EXTENDED_WORD_AMOUNT = 64,
		DIGEST_CHARS = 64,
		SIZE_WITHOUT_LENGTH = 448,
		WORD_BYTES = 4,
		LONGWORD_BYTES = 8,
		CHUNK_BYTES = 64,
		BYTE_HEX_DIGITS = 2, // 2 hex digits are needed to display a uint8_t.
		HEX_DIGIT_BITS = 4, // A single hex digit corresponds to 4 bits.
		BYTE_SIZE = 8, // 8 Bits.
		WORD_SIZE = WORD_BYTES * BYTE_SIZE, // 32 bits.
		LONGWORD_SIZE = LONGWORD_BYTES * BYTE_SIZE, // 64 bits.
		CHUNK_SIZE = CHUNK_BYTES * BYTE_SIZE; // 512 bits.
	static const uint8_t FULL_BYTE = 0xFF, // A uint8_t with all bits set to 1.
		HALF_BYTE = 0x0F; // A uint8_t with the 4 left bits set to 0 and the 4 right bits set to 1.
	static const string HEX_VALUES, SHA_VERSION;
	static const string ERRORS[]; // Contains at each index an error message corresponding to that error number.
	static const word ROUND_CONSTANTS[EXTENDED_WORD_AMOUNT],
		INITIAL_VALUES[];
	static const short OUT_OF_BOUNDS_E = 1,
		INVALID_HASH_LENGTH_E = 2,
		INVALID_CHUNK_SIZE_E = 3;

	word h0, h1, h2, h3, h4, h5, h6, h7;

	// Protected class methods
	void setDefaults();
	vector<uint8_t> preProcess(vector<uint8_t> input);
	void processChunk(vector<uint8_t> chunkContainer);
	word bytesToWord(vector<uint8_t> input, unsigned int start);
	string wVectorToHexString(vector<word> input);
	word rightRotate(word w, unsigned int n);
	void handleException(short errorId);
	vector<word> hash(vector<uint8_t> input);
public:
	// Public class methods
	SHA256();
	~SHA256();
	string hash(string input);
	string hash(char* input, longword size);
	string shaVersion();
};
// End of class SHA256