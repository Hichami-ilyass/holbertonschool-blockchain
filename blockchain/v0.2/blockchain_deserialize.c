#include "blockchain.h"
/**
* convert_endianness - converts endianness of a block
* @block: a blockchain block
*/
void swap_endianness(block_t *block)
{
	_swap_endian(&block->info.index, sizeof(block->info.index));
	_swap_endian(&block->info.difficulty, sizeof(block->info.difficulty));
	_swap_endian(&block->info.timestamp, sizeof(block->info.timestamp));
	_swap_endian(&block->info.nonce, sizeof(block->info.nonce));
}
/**
* read_blocks - read blocks from a file
* @fp: file pointer
* @swap_endian: a flag to swap endianness
* @blockchain: blockchain
* @size: number of blocks to read
* Return: 0 if successful, -1 if failed
*/
int read_blocks(FILE *fp, uint8_t swap_endian, blockchain_t *blockchain, int size)
{
	int i;
	block_t *block;
	uint32_t data_len;
	long int genesis_size;
	
	genesis_size = sizeof(block->info) + sizeof(block->hash) + 20L;
	fseek(fp, genesis_size, SEEK_CUR);
	for(i = 0; i < size - 1; ++i)
	{
		block = malloc(sizeof(*block));
		if(!block)
			return (-1);
		fread(&block->info, sizeof(block->info), 1, fp);
      		fread(&data_len, sizeof(data_len), 1, fp);
		if (swap_endian)
			_swap_endian(&data_len, sizeof(data_len));
		fread(&block->data.buffer, data_len, 1, fp);
      		fread(&block->hash, SHA256_DIGEST_LENGTH, 1, fp);
		if (swap_endian)
			swap_endianness(block);
		block->data.len = data_len;
		*(block->data.buffer + data_len) = '\0';
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);	
	}
	return (0);
}
/**
 * blockchain_deserialize - deserialize a blockchain from a file
 *
 * @path: path to a file from which to load the blockchain
 *
 * Return: a pointer to the deserialized blockchain, NULL upon failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *fp;
	uint8_t hblk_magic[4];
  	uint8_t hblk_version[3];
  	uint8_t hblk_endian;
  	int32_t hblk_blocks;
  	blockchain_t *blockchain;
  	int size;
  	uint8_t swap_endian;

	if (!path)
		return (NULL);
	fp = fopen(path,"r");
	if (!fp)
		return (NULL);
	fread(&hblk_magic, sizeof(hblk_magic), 1, fp);
  	fread(&hblk_version, sizeof(hblk_version), 1, fp);
  	fread(&hblk_endian, sizeof(hblk_endian), 1, fp);
  	fread(&hblk_blocks, sizeof(hblk_blocks), 1, fp);
	if (memcmp(hblk_magic, "HBLK", 4) ||
		memcmp(hblk_version, "0.1", 3))
		return(fclose(fp), NULL);
	blockchain = blockchain_create();
	if (!blockchain)
		return(fclose(fp), NULL);
	swap_endian = _get_endianness() != hblk_endian;
	size = hblk_blocks;
	if (swap_endian)
		_swap_endian(&size, sizeof(size));
	if (read_blocks(fp, swap_endian, blockchain, size) == -1)
		return (blockchain_destroy(blockchain), fclose(fp), NULL);
	fclose(fp);
	return (blockchain);
}

