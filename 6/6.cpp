#include "stdafx.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <cstdio>

int hash_string(char* str) {
	int result = 0;

	for (int i = 0; i < strlen(str); i++) {
		result = _rotr(result, 13);
		result += str[i];
	}

	return result;
}

// example key:
// b00!4oMB@i^
// ----???????
//     x                GetLocalTime - [0x2D + HIBYTE(WORD(YEAR))] -> ASCII number    | 10  values ('0' -> '9')
//      x               GetLocalTime - [0x5E + day]                                   | 31  values
//       x              GetLocalTime - [0x42 + hour]                                  | 24  values
//        x             GetVersion - [0x3C + MajorVersion]                            |  3  values (5, 6, 10)
//         x            GetVersion - [0x3F + MinorVersion]                            |  4  values (0, 1, 2, 3)
//          x           PEB->BeingDebugged - [0x69 + PEB->BeingDebugged]              |  2  values (0x69, 0x6A)
//           x          GetUserDefaultUILanguage (AH; AL must be 0; 255 values)       | 255 values

// TL;DR 45 532 800 values to brute force

void decodeKey(unsigned char* decodedKey, unsigned char* key, int length) {
	for (int i = 0; i < 256; i++) {
		decodedKey[i] = i;
	}

	unsigned char index = 0;
	for (int j = 0; j < 256; j++) {
		unsigned char c = decodedKey[j];
		index += c + key[j % length];
		if (index < 0 || index > 255) {
			// fail
			throw 'up';
		}
		decodedKey[j] = decodedKey[index];
		decodedKey[index] = c;
	}
}

void decryptData(unsigned char* key, unsigned char* data, int length) {
	int i = 0;
	unsigned char index = 0, remoteKey = 0;

	do {
		i++;

		unsigned char c = key[i];
		index += c;
		remoteKey = key[index];
		key[i] = remoteKey;
		key[index] = c;
		remoteKey += c;
		*data++ ^= key[remoteKey];

		length--;
	} while (length);
}

int main() {
	unsigned char encryptedData[] = {
		0xBA, 0xAF, 0x4D, 0x55, 0x3C, 0xE3, 0x03, 0x22, 0xB0, 0xDF, 0xF3, 0xD3, 0x57, 0xD0, 0xE1, 0x40,
		0xF9, 0x13, 0x1F, 0xBA, 0x8D, 0x12, 0xF1, 0xFF, 0x48, 0xC2, 0x8E, 0x00, 0xFD, 0x54, 0x97, 0x9D,
		0x75, 0x71, 0x30, 0x8F, 0x43, 0x28, 0xFE, 0x69, 0x36, 0x47, 0x8F, 0xA2, 0xEF, 0x49, 0x74, 0x7C,
		0xE1, 0x4C, 0x6F, 0x4F, 0xD4, 0x82
	};

	unsigned char backupData[sizeof(encryptedData)];
	memcpy(backupData, encryptedData, sizeof(backupData));

	unsigned char decodedKey[256];
	// unsigned char key[] = "b00!4oMB@i^";
	unsigned char key[] = "b00!4oMB@jb"; // PE->BeingDebugged has to be 1, so 'j' instead of 'i'

	unsigned int counter = 0;

	for (int i = 255; i > 0; i--) {
		// MajorVersion
		int MajorVersionValues[] = { 0, 5, 6, 10 };
		for (int j = 0; j < 4; j++) {
			// MinorVersion
			int MinorVersionValues[] = { 0, 1, 2, 3 };
			for (int k = 0; k < 4; k++) {
				// Hour
				for (int l = 0; l < 25; l++) {
					// Day
					for (int m = 0; m < 32; m++) {
						// Some other fucking shit
						for (int n = 0; n < 256; n++) {
							// A message to my CPU, who will never read this because the compiler will strip it away:
							// (but anyway)
							// I'm sorry.


							// Show some progress
							counter++;
							if ((counter % 1000000) == 0)
								printf("> [i: %d, j: %d, k: %d, l: %d, m: %d, n: %d]\n", i, j, k, l, m, n);

							// year
							key[4] = i;

							// [0x5E + day]
							key[5] = char(0x5E + m);

							// [0x42 + hour]
							key[6] = char(0x42 + l);

							// [0x3C + major version]
							key[7] = char(0x3C + MajorVersionValues[j]);

							// [0x3F + minor version]
							key[8] = char(0x3F + MinorVersionValues[k]);

							// [0x69 + PEB->BeingDebugged] (0 / 1)
							// key[9] = char(0x69 + i);

							// 0-255 language index
							key[10] = char(n);

							// And finally decrypt.
							decodeKey(decodedKey, key, 11);
							decryptData(decodedKey, encryptedData, sizeof(encryptedData));

							if (encryptedData[0] == 'P' && encryptedData[1] == 'A' && encryptedData[2] == 'N' && encryptedData[3] == '{') {
								printf("Found it!\nKey is: %s\nData is:%s\n\nHex:\n", key, encryptedData);
								for (int w = 0; w < 11; w++) printf("%02X ", key[w]);
								puts("");
								for (int w = 0; w < sizeof(encryptedData); w++) printf("%02X ", encryptedData[w]);
								puts("\n");
								system("pause");
								//system("cls");
							} else {
								memcpy(encryptedData, backupData, sizeof(encryptedData));
							}
						}
					}
				}
			}
		}
	}

	printf("Nothing found... tried %d keys.\n", counter);
	system("pause > nul");

	return 0;
}