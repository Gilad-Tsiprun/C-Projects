#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include "trie.h" 


#define ROOT_DEPTH (0)

#define LEFT (0)
#define RIGHT (1)

#define NOT_FULL (0)
#define FULL (1)

#define OTHER_DIRECTION(direction) ((direction) ^ (1))

typedef struct trie_node
{
    struct trie_node *children[2];
    int is_full;
    size_t depth;
} trie_node_t;

struct trie
{
    size_t height;
    trie_node_t *root;
};

static trie_node_t *CreateNode(trie_node_t *left, trie_node_t *right, 
                               int is_full, size_t depth)
{
    trie_node_t *node = (trie_node_t *)malloc(sizeof(trie_node_t));

    if (!node)
    {
        return NULL;
    }

    node->children[0] = left;
    node->children[1] = right;
    node->is_full = is_full;
    node->depth = depth;

    return node;
}

int HasChildren(trie_node_t *node)
{
    return (node->children[LEFT] || node->children[RIGHT]);
}

int NodeIsFull(trie_node_t *node)
{
    if (node->children[LEFT] && node->children[RIGHT])
    {
        return (node->children[LEFT]->is_full && node->children[RIGHT]->is_full);
    }

    return 0;
}


trie_t *TrieCreate(size_t height)
{
    trie_t *trie = (trie_t *)malloc(sizeof(trie_t));
    trie_node_t *root = NULL;

    if (!trie)
    {
        return NULL;
    }
    
    root = CreateNode(NULL, NULL, NOT_FULL, ROOT_DEPTH);
    
    if (root == NULL)
    {
        free(trie);
        trie = NULL;

        return NULL;
    }

    trie->root = root;
    trie->height = height;

    return trie;
}


static void DestroySubTrie(trie_node_t *node)
{
	if (node->children[LEFT])
	{
		DestroySubTrie(node->children[LEFT]);
	}
	
	if (node->children[RIGHT])
	{
		DestroySubTrie(node->children[RIGHT]);
	}
	
	free(node);
	node = NULL;
}

void TrieDestroy(trie_t *trie)
{
    assert(trie);

    DestroySubTrie(trie->root);
    
    free(trie);
    trie = NULL;
}

size_t RecCountFullKeys(trie_node_t *node)
{
    if (node == NULL)
    {
        return 0;
    }

    if (!node->children[LEFT] && !node->children[RIGHT])
    {
        return node->is_full;
    }
    
    return (RecCountFullKeys(node->children[LEFT]) + RecCountFullKeys(node->children[RIGHT]));
}

size_t TrieCountFullKeys(const trie_t *trie)
{
    assert(trie);
    
    if (!HasChildren(trie->root))
    {
        return 0;
    }
    

    return (RecCountFullKeys(trie->root));
}

int NodeInsert(trie_node_t *node, unsigned int key, size_t current_depth ,
			size_t depth_left)
{
	int child_direction = ((key >> (depth_left - 1)) & 1);
	int status = 0;
	
	if (!depth_left)
	{
		node->is_full = FULL;

		return 0;
	}

	if (node->is_full == FULL)
	{
		return 1;
	}
	
	if (!node->children[child_direction])
	{
		node->children[child_direction] = CreateNode(NULL, NULL, NOT_FULL, current_depth + 1);

		if(!node->children[child_direction])
		{
			return 1;
		}
	}

	status = NodeInsert(node->children[child_direction], key ,
						current_depth + 1 ,depth_left - 1);

	node->is_full = NodeIsFull(node);

	return status;
}

int TrieInsert(trie_t *trie, unsigned int key)
{
	return NodeInsert(trie->root, key, ROOT_DEPTH, trie->height);
}

void NodeRemove(trie_node_t *node, unsigned int key, size_t current_depth ,
			size_t depth_left)
{
	int child = ((key >> (depth_left - 1)) & 1);
	
	node->is_full = NOT_FULL;	
	
	if (depth_left)
	{		
		NodeRemove(node->children[child], key, current_depth + 1, depth_left - 1);	
	}
}

void TrieRemove(trie_t *trie, unsigned int key)
{
	NodeRemove(trie->root, key, ROOT_DEPTH, trie->height);
}

unsigned int AvailableKeyFrom(trie_node_t *node, unsigned int key,
					 size_t height, unsigned int *new_key)
{
	int direction = key >> (height) & 1;

	if (!node)
	{
		return 1;
	}

	else if ((node->children[direction])&&
			(node->children[direction]->is_full))
	{
		if (direction == LEFT)
		{
			*new_key = *new_key | (1 << (height));
			return AvailableKeyFrom(node->children[1], 0, height - 1, new_key);
		}

		return 0;
	}

	if (direction == LEFT)
	{
		int current_key = *new_key;
		int is_found =  AvailableKeyFrom(node->children[direction],
			                            key, height - 1, new_key);
		
		if (is_found)
		{		
			return 1;
		}
		else
		{
			*new_key = (current_key |(1 << (height)));
			
			return AvailableKeyFrom(node->children[OTHER_DIRECTION(direction)],
					 		                        0, height - 1 , new_key);			
		}		
	}
	else
	{
		*new_key = *new_key | (direction << (height));

		return AvailableKeyFrom(node->children[direction],
					 		key, height - 1, new_key);
	}		

}

unsigned int TrieAvailableKeyFrom(const trie_t *trie, unsigned int key)
{
	unsigned int new_key = 0;

	AvailableKeyFrom(trie->root, key, trie->height - 1, &new_key);	

	if (new_key < key)
	{
		new_key = key;
	} 

	return new_key;
}