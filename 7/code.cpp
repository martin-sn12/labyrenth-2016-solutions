#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>

typedef unsigned long DWORD;

static char encoding_table[] = "qtgJYKa8y5L4flzMQ/BsGpSkHIjhVrm3NCAi9cbeXvuwDx+R6dO7ZPEno21T0UFW";
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

void build_decoding_table() {
    decoding_table = (char*) malloc(256);
    for (int i = 0; i < 64; i++) decoding_table[(unsigned char) encoding_table[i]] = i;
}

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {

    if (decoding_table == NULL) build_decoding_table();

    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    auto decoded_data = (unsigned char*) malloc(*output_length);
    if (decoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}

void base64_cleanup() { free(decoding_table); }

// Algorithm is XTEA

void encryptChunk(int iterations, DWORD* data, DWORD* key){
	int counter = 0;
	
	for (int i = 0; i < iterations; i++) {
		data[0] += (key[counter & 3] + counter) ^ (data[1] + ((data[1] >> 5) ^ 16 * data[1]));
		counter += 0x9E3779B9;
		data[1] += (key[(counter >> 11) & 3] + counter) ^ (data[0] + ((data[0] >> 5) ^ 16 * data[0]));
	}
	
}

void decryptChunk(int iterations, DWORD* data, DWORD* key){
	int counter = 0x9E3779B9 * iterations;
	
	for (int i = 0; i < iterations; i++) {
		data[1] -= (((data[0] << 4) ^ (data[0] >> 5)) + data[0]) ^ (counter + key[(counter >> 11) & 3]);
		counter -= 0x9E3779B9;
		data[0] -= (((data[1] << 4) ^ (data[1] >> 5)) + data[1]) ^ (counter + key[counter & 3]);
	}
}

int main(){
	char* key = "AWildKeyAppears!";
	char data[] =
		"B6XGLACYYUdwodupUtF0geaE5NKnf5gTiKxgwfWCJdi8Iq/"
		"b36ShdY/gs18m2VwpkTJPmg03FDpavvJF3EcAX8SUkrbpI1"
		"T61ZGKnrbD9gkf79eqi4giA4uKYEv9O/Iw3Godkhd0tB9e1"
		"ojQgW4307/OSTtWIzyEVhHbqkV694+fSZLD7FYMa80QYJQ5"
		"JRV/B6XGLACYYUdwodupUtF0geaE5NKnf5gT/Ycz/Ptt/q="
		"=B6XGLACYYUdwodupUtF0geaE5NKnf5gTiKxgwfWCJdi8Iq"
		"/b36ShdY/gs18m2VwpkTJPmg03FDpavvJF3EcAX8SUkrbpI"
		"1T61ZGKnrbD9gkf79eqi4giA4uKYEv9O/Iw3Godkhd0tB9e"
		"1ojQgW4307/OSTtWIzyEVhHbqkV694+fSZLD7FYMa80QYJQ"
		"5JRV/B6XGLACYYUdwodupUtF0geaE5NKnf5gT/Ycz/Ptt/q"
		"==";
	
	size_t length = 0;
	unsigned char* decodedData = base64_decode(data, sizeof(data) - 1, &length);
	
	for (int i = 0; i < length / 8; i++) {
		decryptChunk(32, (DWORD*) (&decodedData[8 * i]), (DWORD*) key);
	}
	
	printf("%s", decodedData);

	base64_cleanup();
	
	return 0;
}