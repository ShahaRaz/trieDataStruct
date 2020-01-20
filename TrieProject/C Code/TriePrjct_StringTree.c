#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 
// A=65 , a=97 , z = 90 , z=122 ->  Capital 65-90 ,, LowerCase 97-122
// TODO -  insert deletion function.
// inorder to insert the deletion fun we need to insert to each node the number of times the letter was used.
// TODO  - maybe insert a TOLOWER definition that will check by ascii if the letter is upper case, if it is, make it lower case, if it isnt a letter -> ?

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0]) 
// Alphabet size (# of symbols) 
#define ALPHABET_SIZE (26) 

// Converts key current character into index 
// use only 'a' through 'z' and lower case 
#define CHAR_TO_INDEX(c) ((int)c - (int)'a') 


typedef int DATA; // adds possiblity to scale down or up, to save space/create more nodes.

// Modify strings to lower case (when external source adds string)
void stringtoLwrCase(char* s)
{
	int i = 0;
	while (s[i] != '\0')
	{
		if (s[i] >= 'A' && s[i] <= 'Z') {
			s[i] = s[i] + 32;
		}
		++i;
	}
}

// trie node 
struct TrieNode 
{
	// pointer to array of TrieNodes* (pointers to trienode)
	struct TrieNode* children[ALPHABET_SIZE]; 
	bool isEndOfWord;
	DATA numOfWordsUsing; 
};

// Returns new trie node (initialized to NULLs) 
struct TrieNode* getNode(void)
{
	struct TrieNode* pNode = NULL; // creates pointer to TrieNode

	pNode = (struct TrieNode*)malloc(sizeof(struct TrieNode)); // why malloc? in the TrieNode decleration there is array in the correct size...?

	if (pNode) // if we are pointing at a pnode
	{
		int i;

		pNode->isEndOfWord = false;
		pNode->numOfWordsUsing = 0;

		for (i = 0; i < ALPHABET_SIZE; i++)
			pNode->children[i] = NULL;
	}

	return pNode;
}

// If not present, inserts key into trie 
// If the key is prefix of trie node, just marks leaf node 
void insert(struct TrieNode* root, const char* key)
{
	int level; // the level we are inside the trie
	int length = strlen(key); //level will tell us over which character we are lookin at the moment
	int index;

	struct TrieNode* pTN = root; // pointer Trie Node

	for (level = 0; level < length; level++)
	{
		index = CHAR_TO_INDEX(key[level]); // index inside the Child's array, based on the char we look for.
		if (!pTN->children[index]) // if the letter is not already in the trie
			pTN->children[index] = getNode(); // create new node

		pTN->numOfWordsUsing++;
		pTN = pTN->children[index]; // point to next char

	}
	// mark last node as leaf 
	pTN->isEndOfWord = true;
}

// Returns true if key presents in trie, else false 
bool search(struct TrieNode* root, const char* key)
{
	int level;
	int length = strlen(key);
	int index;
	struct TrieNode* pTN = root;

	for (level = 0; level < length; level++)
	{
		index = CHAR_TO_INDEX(key[level]); // index inside correnct array 
		if (pTN->children[index] == NULL) return false;
		if (!pTN->children[index]) // if the next char doesn't exists
			return false;

		pTN = pTN->children[index];
	}

	return (pTN != NULL && pTN->isEndOfWord);
}


void freeSubTree(struct TrieNode* root)
{ 
	if (root->isEndOfWord == true) // run until the end of the word
	{
		free(root);
		//FREE MYSELF
		return;
	}
	for (int i = 0; i < ARRAY_SIZE(root->children); i++)
	{
		if (root->children != NULL) // only one letter will fit
		{
			freeSubTree(root->children[i]); // recursive call , will reach all descendants
					//break; // uncomment to save time, ONLY if Node has ONE child.
		}
	}
	free(root); // free itself 
}


bool delWord(struct TrieNode* root, const char* key) {
	bool isWordExists = search(root, key); // O(wordLen)
	if (!isWordExists) return false;

	int level;
	int length = strlen(key);
	int index;
	struct TrieNode* pTN = root;

	for (level = 0; level < length; level++)
	{
		pTN->numOfWordsUsing--; // Decrement the corrent's node wordsUsing Count.
		index = CHAR_TO_INDEX(key[level]); // index of next char inside correnct array 
		if (pTN->children[index]->numOfWordsUsing == 1) 
			//does the next node is used only for the word we now deleting?
		{
			freeSubTree(pTN->children[index]); // free next node and all nodes under it
			pTN->children[index] = NULL; // cut the pointer to next node.
			break; // leave the for loop
		}
		pTN = pTN->children[index];
	}
	// if the node still exists, after finishing all of the word lenght, mark it as "notEndOfWord" 
	//for example, (if deleted "the" and Trie contains "these")
	if (pTN)	pTN->isEndOfWord = false;
	return true;
}

bool findSubString(struct TrieNode* root, const char* key) {
	int level;
	int length = strlen(key);
	int index;
	struct TrieNode* pTN = root;

	for (level = 0; level < length; level++)
	{
		index = CHAR_TO_INDEX(key[level]); // index inside correnct array 
		if (pTN->children[index] == NULL) return false;
		if (!pTN->children[index]) // if the next char doesn't exists
			return false;

		pTN = pTN->children[index];
	}

	return (pTN != NULL && !pTN->isEndOfWord); // pTN is not End of word SO IT IS A SUB STRING


}


// Driver 
int main()
{
	// Input keys (use only 'a' through 'z' and lower case) 
	char keys[][20] = { "the", "a", "there", "answer", "any",
							"by", "bye", "their", "stop" , "hey" , "they" , "to" ,"this" ,"that" , "these", "throw"
							"hype" , "tango" , "troops" , "tea" , "theres" };

	char output[][32] = { "Not present in trie", "Present in trie" };

	struct TrieNode* root = getNode();

	// Construct trie 
	int i;
	for (i = 0; i < ARRAY_SIZE(keys); i++)
		insert(root, keys[i]);

	printf("%s --- %s\n", "answer", output[search(root, "answer")]);
	printf("%s --- %s\n", "any", output[search(root, "any")]);
	// Search for different keys 
	printf("%s --- %s\n", "the", output[search(root, "the")]);
	printf("%s --- %s\n", "these", output[search(root, "these")]);
	printf("%s --- %s\n", "their", output[search(root, "their")]);
	printf("%s --- %s\n", "thaw", output[search(root, "thaw")]);
	printf("%s --- %s\n\n\n", "there", output[search(root, "there")]);
	///printf("deleting there : %b", delNode(root, "there"));
	delWord(root, "there");
	printf("removed there__\n");

	printf("%s --- %s\n\n\n", "there", output[search(root, "there")]);

	printf("%s --- %s\n", "bye", output[search(root, "bye")]);
	printf("%s --- %s\n", "a", output[search(root, "a")]);
	printf("%s --- %s\n", "the", output[search(root, "the")]);
	printf("%s --- %s\n", "f", output[search(root, "f")]);
	printf("%s --- %s\n", "thei", output[search(root, "thei")]);
	printf("%s --- %s\n", "their", output[search(root, "their")]);



	printf("%s --- %s\n", "theres", output[search(root, "theres")]);
	printf("%s --- %s\n", "hey", output[search(root, "hey")]);
	printf("%s --- %s\n", "that", output[search(root, "that")]);
	printf("%s --- %s\n", "this", output[search(root, "this")]);


	return 0;
}