#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <llist.h>

#include "provided/endianness.h"
#include "../../crypto/hblk_crypto.h"


#define HBLK_MAGIC "HBLK"
#define HBLK_MAGIC_LEN	4
#define HBLK_VERSION "0.1"
#define HBLK_VERSION_LEN 3

#define BLOCK_DATA_MAX_LEN 1024
#define BLOCKCHAIN_DATA_MAX 1024

#define GENESIS_INDEX 	0								
#define GENESIS_DIFFICULTY  0
#define GENESIS_NONCE  	0
#define GENESIS_HASH 								\
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d"	\
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"	
#define GENESIS_DATA_BUFFER "Holberton shcool" 
#define GENESIS_TIMESTAMP 1537578000 
#define GENESIS_DATA_LEN 16
#define GENESIS_PREV_HASH 							\
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"	\
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"

#define GENESIS_BLOCK { \
		{ /* info */	    \
			0 /* index */,				\
				0, /* difficulty */		\
				1537578000, /* timestamp */	\
				0, /* nonce */			\
				{0} /* prev_hash */		\
		},						\
		{ /* data */					\
			"Holberton School", /* buffer */	\
				16 /* len */				\
				},					\
		"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
		"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03" \
		}					

#define MIN(x, y) ((x) < (y) ? (x) : (y))

/**
 * struct block_data_s - Block data
 *
 * @buffer: Data buffer
 * @len:    Data size (in bytes)
 */
typedef struct block_data_s
{
    /*
     * @buffer must stay first, so we can directly use the structure as
     * an array of char
     */
    int8_t      buffer[BLOCKCHAIN_DATA_MAX];
    uint32_t    len;
} block_data_t;


/**
 * struct block_info_s - Block info structure
 *
 * @index:      Index of the Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
    /*
     * The order of the elements in this structure should remain the same.
     * It was designed so every element of this structure is aligned and
     * doesn't require padding from the compiler.
     * Therefore, it is possible to use the structure as an array of char,
     * on any architecture.
     */
    uint32_t    index;
    uint32_t    difficulty;
    uint64_t    timestamp;
    uint64_t    nonce;
    uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_s - Block structure
 *
 * @info: Block info
 * @data: Block data
 * @hash: 256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
    block_info_t    info; /* This must stay first */
    block_data_t    data; /* This must stay second */
    uint8_t     hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_s - Blockchain structure
 *
 * @chain: Linked list of pointers to block_t
 */
typedef struct blockchain_s
{
    llist_t     *chain;
} blockchain_t;

blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev, int8_t const *data,uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block);
#endif
