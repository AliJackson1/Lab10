#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode
{
    int count;
    struct TrieNode* children[26];
};

// Trie structure
struct Trie
{
    struct TrieNode* root;
};

// Creates new TrieNode
struct TrieNode* createTrieNode()
{
    struct TrieNode* newNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    newNode->count = 0;
    for (int i = 0; i < 26; ++i)
        newNode->children[i] = NULL;
    return newNode;
}

// Inserts word to the trie structure
void insert(struct Trie* pTrie, char* word)
{
    if (pTrie == NULL || word == NULL)
        return;

    struct TrieNode* node = pTrie->root;
    int len = strlen(word);
    for (int i = 0; i < len; ++i)
    {
        int index = word[i] - 'a';
        if (node->children[index] == NULL)
            node->children[index] = createTrieNode();
        node = node->children[index];
    }
    node->count++;
}

// Computes number of occurrences of word
int numberOfOccurrences(struct Trie* pTrie, char* word)
{
    if (pTrie == NULL || word == NULL)
        return 0;

    struct TrieNode* node = pTrie->root;
    int len = strlen(word);
    for (int i = 0; i < len; ++i)
    {
        int index = word[i] - 'a';
        if (node->children[index] == NULL)
            return 0;
        node = node->children[index];
    }
    return node->count;
}

// Deallocate trie structure
struct Trie* deallocateTrie(struct Trie* pTrie)
{
    if (pTrie == NULL)
        return NULL;

    // Recursive function to free nodes
    void freeNodes(struct TrieNode* node)
    {
        if (node == NULL)
            return;
        for (int i = 0; i < 26; ++i)
        {
            freeNodes(node->children[i]);
        }
        free(node);
    }

    freeNodes(pTrie->root);
    free(pTrie);
    return NULL;
}

// Initializes trie structure
struct Trie* createTrie()
{
    struct Trie* pTrie = (struct Trie*)malloc(sizeof(struct Trie));
    pTrie->root = createTrieNode();
    return pTrie;
}

// return the number of words in the dictionary
// and read all the words in the dictionary to the structure words
int readDictionary(char* filename, char** pInWords)
{
    FILE* fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Error opening the file %s\n", filename);
        return 0;
    }

    int numWords;
    fscanf(fp, "%d", &numWords);

    for (int i = 0; i < numWords; ++i)
    {
        pInWords[i] = (char*)malloc(100 * sizeof(char)); // Assuming max word length is 100
        fscanf(fp, "%s", pInWords[i]);
    }

    fclose(fp);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Parse line by line, and insert each word to the trie data structure
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie == NULL)
        printf("Trie deallocated successfully.\n");

    // Free memory for words
    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }

    return 0;
}

