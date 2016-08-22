#include <cstdio>

int main(){
	// total has to be 68
	// first numbers should be 3121
	char keyPool[] = {
		0x27, 0x34, 0x38, 0x0E, 
		0x36, 0x47, 0x19, 0x11, 
		0x07, 0x43, 0x40, 0x03, 
		0x1A, 0x14, 0x23, 0x47, 
		0x1A, 0x19, 0x04, 0x29, 
		0x17, 0x02, 0x13, 0x12, 
		0x0F, 0x2A, 0x0E, 0x46, 
		0x20, 0x01, 0x44, 0x29, 
		0x04, 0x1A, 0x1A, 0x03, 
		0x10, 0x13, 0x28, 0x02, 
		0x1D, 0x12, 0x28, 0x04, 
		0x13, 0x41, 0x1B, 0x29, 
		0x2A, 0x07, 0x05, 0x39, 
		0x17, 0x3B, 0x44, 0x3B, 
		0x0B,
	};
	
	for (int j = 1; j <= 3; j++) {
		for (int i = 0; i < sizeof(keyPool); i++) {
			printf("%c", keyPool[i] ^ 68 ^ ('0' + j));
		}
		
		puts("");
	}
	
	// This will output three lines with possible characters. It's a matter of seeing the pattern to get the key,
	// which is PAN{C0ngr47ulaT1ons_buddy_y0Uv3_solved_the_re4l__prObL3M}
	
	return 0;
}