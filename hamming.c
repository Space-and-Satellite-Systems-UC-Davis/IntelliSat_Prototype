#include <stdio.h>

// Hamming(11,7) Encoder
void hamming_encoder(const int* data, int* encoded) {
	// Copy the 7 bits into the encoded array, leaving spaces for parity bits.
	encoded[2] = data[0];
    encoded[4] = data[1];
    encoded[5] = data[2];
    encoded[6] = data[3];
    encoded[8] = data[4];
    encoded[9] = data[5];
    encoded[10] = data[6];

    // Calculate parity bits.
    encoded[0] = encoded[2] ^ encoded[4] ^ encoded[6] ^ encoded[8] ^ encoded[10];
    encoded[1] = encoded[2] ^ encoded[5] ^ encoded[6] ^ encoded[9] ^ encoded[10];
    encoded[3] = encoded[4] ^ encoded[5] ^ encoded[6];
    encoded[7] = encoded[8] ^ encoded[9] ^ encoded[10];
}

// Hamming(11,7) Decoder
void hamming_decoder(int* encoded, int* decoded) {
	
	// Calculate parity check bits.
    int p1 = encoded[0] ^ encoded[2] ^ encoded[4] ^ encoded[6] ^ encoded[8] ^ encoded[10];
    int p2 = encoded[1] ^ encoded[2] ^ encoded[5] ^ encoded[6] ^ encoded[9] ^ encoded[10];
    int p4 = encoded[3] ^ encoded[4] ^ encoded[5] ^ encoded[6];
    int p8 = encoded[7] ^ encoded[8] ^ encoded[9] ^ encoded[10];

	// Check for errors and correct if necessary.
    int error_position = p1 + p2 * 2 + p4 * 4 + p8 * 8;
	// throw an exception if error_position is > 8 -- if more than one bit got flipped 
		// double check if this is needed at SSS 
	
	// Error detected, correct the error.
	if (error_position != 0)
	{
		printf("Error detected at position %d.\n", error_position); // position, not index!
		encoded[error_position - 1] = 1 - encoded[error_position - 1];
	}
	
	// Copy the 7 bits from encoded to decoded.
	decoded[0] = encoded[2];
	decoded[1] = encoded[4];
	decoded[2] = encoded[5];
	decoded[3] = encoded[6];
	decoded[4] = encoded[8];
	decoded[5] = encoded[9];
	decoded[6] = encoded[10];
}

int main() {
    int bits[7] = {1, 0, 0, 1, 1, 0, 1};
	int encodedBits[11]; // 01110010101
	int decodedBits[7];

	int encodedBitsFlip[11] = {0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1}; // one bit flipped
	int decodedFlip[7];

	int encodedBitsFlipBreak[11] = {1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1}; // multiple bits flipped (possibly break system?)
	int decodedFlipBreak[7];
    
	hamming_encoder(bits, encodedBits);
	hamming_decoder(encodedBits, decodedBits);

    printf("Original Bits:             ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", bits[i]);
    }
    printf("\n");

    printf("Encoded Bits:              ");
    for (int i = 0; i < 11; i++) {
        printf("%d ", encodedBits[i]);
    }
    printf("\n");

	printf("Decoded Bits:              ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", decodedBits[i]);
    }
    printf("\n");

	printf("Encoded Flipped Bit:       ");
    for (int i = 0; i < 11; i++) {
        printf("%d ", encodedBitsFlip[i]);
    }
    printf("\n");

	hamming_decoder(encodedBitsFlip, decodedFlip); // down here to show in console properly

	printf("Decoded Flipped Bit:       ");
    for (int i = 0; i < 7; i++) {
        printf("%d ", decodedFlip[i]);
    }
    printf("\n");

	hamming_decoder(encodedBitsFlipBreak, decodedFlipBreak); // won't work

	printf("Decoded Flipped Bit BREAK: "); //
    for (int i = 0; i < 7; i++) {
        printf("%d ", decodedFlipBreak[i]);
    }
    printf("\n");

    return 0;
}
