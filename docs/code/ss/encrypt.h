// Copyright (c) 2012 dndx (idndx.com)

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef ENCRYPT_H_
#define ENCRYPT_H_
#include <stdint.h>
#include "rc4.h"

#define TABLE_SIZE 256
#define METHOD_SHADOWCRYPT 0x1
#define METHOD_RC4 0x2
#define MD5_LEN 16

struct encryptor {
	uint8_t *key;
	uint8_t *encrypt_table;
	uint8_t *decrypt_table;
	struct rc4_state *rc4_en;
	struct rc4_state *rc4_de;
};

void md5(const uint8_t *text, uint8_t *message);
void make_tables(const uint8_t *key, uint8_t *encrypt_table, uint8_t *decrypt_table);
void shadow_encrypt(uint8_t *data, struct encryptor *enc, register unsigned int length);
void shadow_decrypt(uint8_t *data, struct encryptor *enc, register unsigned int length);
void make_encryptor(struct encryptor *tpl, struct encryptor *enc, uint8_t method, uint8_t *key);
void destroy_encryptor(struct encryptor *enc);

#endif /* !ENCRYPT_H_ */
