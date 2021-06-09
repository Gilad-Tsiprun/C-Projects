#ifndef __TRIE__H__
#define __TRIE__H__


#include <stddef.h> /* size_t */

typedef struct trie trie_t;

trie_t *TrieCreate(size_t height);

void TrieDestroy(trie_t *trie);

int TrieInsert(trie_t *trie, unsigned int key);

void TrieRemove(trie_t *trie, unsigned int key);

unsigned int TrieAvailableKeyFrom(const trie_t *trie, unsigned int key);

size_t TrieCountFullKeys(const trie_t *trie);


#endif /* __TRIE__H__ */