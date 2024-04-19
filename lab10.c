#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100 

struct Trie
{   
    struct Trie *children[26]; 
    int isEndOfWord; 
    int count; 
};

void insert(struct Trie *pTrie, char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!pTrie->children[index]) {
            pTrie->children[index] = (struct Trie *)malloc(sizeof(struct Trie));
            memset(pTrie->children[index], 0, sizeof(struct Trie)); 
        }
        pTrie = pTrie->children[index];
    }
    pTrie->isEndOfWord = 1; 
    pTrie->count += 1; 
}

int numberOfOccurances(struct Trie *pTrie, char *word) {
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!pTrie->children[index])
            return 0;
        pTrie = pTrie->children[index];
    }
    return pTrie->isEndOfWord ? pTrie->count : 0; 
}

struct Trie *deallocateTrie(struct Trie *pTrie) {
    if (!pTrie) return NULL;
    for (int i = 0; i < 26; i++) {
        if (pTrie->children[i]) {
            deallocateTrie(pTrie->children[i]);
        }
    }
    free(pTrie);
    return NULL;
}

struct Trie *createTrie() {
    struct Trie *root = (struct Trie *)malloc(sizeof(struct Trie));
    memset(root, 0, sizeof(struct Trie)); 
    return root;
}

int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open the file.\n");
        return 0;
    }

    char buffer[MAX_WORD_LENGTH];
    int wordCount = 0;

    while (fgets(buffer, MAX_WORD_LENGTH, file)) {
        size_t len = strlen(buffer);
        if (buffer[len - 1] == '\n') buffer[len - 1] = '\0';

        pInWords[wordCount] = (char*) malloc(strlen(buffer) + 1);
        strcpy(pInWords[wordCount], buffer);
        wordCount++;

        if(wordCount >= 256) break;
    }

    fclose(file);
    return wordCount;
}

int main(void) {
    char *inWords[256];

    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");

    for (int i = 0; i < numWords; i++) {
        free(inWords[i]);
    }

    return 0;
}
