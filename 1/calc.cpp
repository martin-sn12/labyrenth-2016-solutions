#include <cstdio>

int main(){
	char v9[40];
	v9[0] = 0x8Cu; // P (80)
	v9[1] = 0xF1u;
	v9[2] = 0x53;
	v9[3] = 0xA3u;
	v9[4] = 8;
	v9[5] = 0xD7u;
	v9[6] = 0xDCu;
	v9[7] = 0x48;
	v9[8] = 0xDBu;
	v9[9] = 0xC;
	v9[10] = 0x3A;
	v9[11] = 0xEEu;
	v9[12] = 0x15;
	v9[13] = 0x22;
	v9[14] = 0xC4u;
	v9[15] = 0xE5u;
	v9[16] = 0xC9u;
	v9[17] = 0xA0u;
	v9[18] = 0xA5u;
	v9[19] = 0xC;
	v9[20] = 0xD3u;
	v9[21] = 0xDCu;
	v9[22] = 0x51;
	v9[23] = 0xC7u;
	v9[24] = 0x39;
	v9[25] = 0xFDu;
	v9[26] = 0xD0u;
	v9[27] = 0xF8u;
	v9[28] = 0x3B;
	v9[29] = 0xE8u;
	v9[30] = 0xCCu;
	v9[31] = 3;
	v9[32] = 6;
	v9[33] = 0x43;
	v9[34] = 0xF7u;
	v9[35] = 0xDAu;
	v9[36] = 0x7E;
	v9[37] = 0x65;
	v9[38] = 0xAEu;
	v9[39] = 0x80u;
	
	char key[sizeof(v9) + 1] = { 0 };
	char counter = 0;

	for (int i = 0; i < sizeof(v9); i++) {
		// c == v9[i] to match

		char c_ = 0;
		bool found = false;
		for (int j = 0; j < 255; j++) {
			char c = ((((j ^ 0x33) + 0x44) ^ 0x55) - 0x66) ^ counter;
			if (c == v9[i]) {
				key[i] = j;
				c_ = c;
				found = true;
				break;
			}
		}

		if (!found) {
			puts("Oops!");
			break;
		}

		counter += c_;
	}
	
	printf("Key is %s\n", key);

	return 0;
}