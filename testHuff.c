#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node{
	char c;
	long long int occurrences;
	int level;
	int bin;
	struct _node *left;
	struct _node *right;
}node;


typedef struct _huff{
	char c;
	int level;
	int bin;
}huff;

void printArray(long long int* array, int length) 
{
	int i;
	for (i = 0; i < length; i++) {
		fprintf(stdout, "%c%I64d ", i, array[i]);
	}
	fprintf(stdout, "\n");

	return;
}

long long int* readFile(char *filename, int *uniqueChars)
{
	FILE *iptr = fopen(filename, "r");

	if (iptr == NULL) {
		fprintf(stderr, "Invalid file\n");
		return NULL;
	}

	fseek(iptr, 0, SEEK_END);
	
	long int location = ftell(iptr);

	if (location == 0) {
		fprintf(stderr, "Empty file\n");
		fclose(iptr);
		return NULL;
	}

	long long int *charOccurrences = (long long int *)calloc(sizeof(long long int), 256);

	fseek(iptr, 0, SEEK_SET);

	int ch;

	while ((ch = fgetc(iptr)) != EOF) {
		if (charOccurrences[ch] == 0) {
			*uniqueChars += 1;
		}
		charOccurrences[ch] += 1;
	}

	fclose(iptr);

	return charOccurrences;
}

void sortTree(node **platform, int start, int end) 
{
	int i;
	int j;

	node *tempNode = NULL;

	for (i = start + 1; i <= end; i++) {

		tempNode = platform[i];

		j = i - 1;

		while (j > start - 1 && platform[j]->occurrences >= tempNode->occurrences) {
			if (platform[j]->occurrences == tempNode->occurrences && platform[j]->left == NULL) {
				break;
			}
			/*if (platform[j]->occurrences == tempNode->occurrences && platform[j]->c > platform[j+1]->c) {
				break;
			}*/
			platform[j + 1] = platform[j];
			j = j - 1;
		}
		platform[j + 1] = tempNode;

	}

	return;
}

void printTree(node **platform, int size)
{
	int i;
	for (i = 0; i < size; i++) {
		if (platform[i] != NULL) {
			fprintf(stdout, "%c->%I64d  ", platform[i]->c, platform[i]->occurrences);
		}
	}
	fprintf(stdout, "\n");
}

node* buildTree(long long int* charOccurrences, int uniqueChars) 
{
	node **platform = (node **)malloc(sizeof(node *) * uniqueChars);
	if (platform == NULL) {
		fprintf(stderr, "Mis-allocation of platform\n");
		return NULL;
	}

	int i = 0;

	int curr = 0;

	for (i = 0; i < 256; i++) {
		if (charOccurrences[i] != 0) {
			node *n = (node *) malloc(sizeof(node));
			n->c = i;
			n->occurrences = charOccurrences[i];
			n->left = NULL;
			n->right = NULL;
			platform[curr] = n;

			curr += 1;
		}
	}

	sortTree(platform, 0, uniqueChars - 1);

	fprintf(stdout, "\n\n\n");

	printTree(platform, uniqueChars);

	int platformSize = uniqueChars;

	int beg = 0;
	int end = uniqueChars - 1;
	
	while (platformSize > 1) {
		node *combo = (node *)malloc(sizeof(node));
		combo->left = platform[beg];
		combo->right = platform[beg + 1];
		combo->c = '1';
		combo->occurrences = platform[beg]->occurrences + platform[beg  + 1]->occurrences;

		beg += 1;

		platform[beg] = combo;
		platform[beg - 1] = NULL;

		sortTree(platform, beg, end);

		platformSize -= 1;

		printTree(platform, uniqueChars);
	}

	node *copy = platform[uniqueChars - 1];

	free(platform);

	return copy;
}

int printPreOrder(node *root, huff *array, int level, int toAdd, int count) 
{
	if (root == NULL) {
		return count;
	}
	if (root->left == NULL && root -> right == NULL) {
		fprintf(stdout, "count = %d %cL%dB%d\n", count, root->c, level, toAdd);

		root->level = level;
		root->bin = toAdd;

		array[count].c = root->c;
		array[count].level = level;
		array[count].bin = toAdd;
		return count + 1;
	}

	toAdd = toAdd << 1;

	int newCount = printPreOrder(root->left, array, level	+ 1, toAdd, count);

	toAdd = toAdd | 1;

	newCount = printPreOrder(root->right, array, level + 1, toAdd, newCount);

	return newCount;
}

void destroyTree(node *root) 
{
	if (root == NULL) {
		return;
	}

	destroyTree(root->left);
	destroyTree(root->right);

	free(root);

	return;
}

void printBits(unsigned char c) 
{
	unsigned char mask = 1 << 7;

	while (mask > 0) {
		if ((c & mask) == mask) {
			fprintf(stdout, "1");
		}
		else {
			fprintf(stdout, "0");
		}

		mask = mask >> 1;
	}
	fprintf(stdout, "\n");
}

huff *declareHuff(int uniqueChars) 
{
	huff *array = (huff *)malloc(sizeof(huff) * uniqueChars);

	return array;
}

/*
typedef struct _huff{
	char c;
	int level;
	int bin;
}huff;
*/

void constructHuff(huff *array, char c, int level, int bin)
{
	array->c = c;
	array->level = level;
	array->bin = bin;
}

void printHuff(huff *array, int length) 
{
	int i = 0;
	for (i = 0; i < length; i++) {
		printf("%c level = %d bin =", array[i].c, array[i].level);
		printBits(array[i].bin);
	}
	return;
}

void changeHuff(huff *array) 
{
	/*constructHuff(&(array[0]), 'g', 2, 0);
	constructHuff(&(array[1]), 'o', 2, 1);
	constructHuff(&(array[2]), 'p', 4, 14);
	constructHuff(&(array[3]), 'h', 4, 13);
	constructHuff(&(array[4]), 'e', 3, 5);
	constructHuff(&(array[5]), 'r', 4, 15);
	constructHuff(&(array[6]), 's', 4, 12);
	constructHuff(&(array[7]), ' ', 3, 4);*/

	constructHuff(&(array[0]), 'a', 3, 4);
	constructHuff(&(array[1]), 'r', 3, 5);
	constructHuff(&(array[2]), 'e', 2, 3);
	constructHuff(&(array[3]), 'n', 4, 1);
	constructHuff(&(array[4]), 't', 3, 3);
	constructHuff(&(array[5]), 's', 3, 2);
	constructHuff(&(array[6]), 'o', 4, 0);
	constructHuff(&(array[7]), ' ', 3, 1);

}

void huffPrint(char *filename, huff *array, int length, long long int *charOccurrences) 
{

	fprintf(stdout, "length %d\n", (int)strlen(filename));

	char huffFilename[(int)strlen(filename) + 6];

	int j;

	for (j = 0; j < strlen(filename); j++) {
		huffFilename[j] = filename[j];
	}

	huffFilename[(int)strlen(filename)] = '.';
	huffFilename[(int)strlen(filename) + 1] = 'h';
	huffFilename[(int)strlen(filename) + 2] = 'u';
	huffFilename[(int)strlen(filename) + 3] = 'f';
	huffFilename[(int)strlen(filename) + 4] = 'f';
	huffFilename[(int)strlen(filename) + 5] = '\0';

	FILE *optr = fopen(huffFilename, "w+");

	if (optr == NULL) {
		fprintf(stdout, "error in creating new file %s\n", huffFilename);
	}

	for (j = 0; j < 256; j++) {
		fprintf(optr, "%I64d", charOccurrences[j]);
	}

	FILE *iptr = fopen(filename, "r");

	fseek(iptr, 0, SEEK_SET);

	int ch;

	unsigned char c = 0;

	int added = 0;

	int overflow = 0;

	unsigned char overC = 0;

	while ((ch = fgetc(iptr)) != EOF) {

		//fprintf(stdout, "\n");
		int i;

		for(i = 0; i < length; i++) {
			if (array[i].c == ch) {
				added += array[i].level;
				if (added >= 8) {
					//fprintf(stdout, "before adding\n");
					//printBits(array[i].bin);
					overflow = added - 8;
					int curr = array[i].level - overflow;
					c = c << curr;
					overC = 0;
					overC = array[i].bin >> overflow;
					c = c | overC;
					//fprintf(stdout, "adding %c\n", array[i].bin);
					//printBits(array[i].bin);
					fprintf(stdout, "to file\n");
					printBits(c);
					fprintf(stdout, "\n");
					fprintf(optr, "%c", c);
					int push = 8 - overflow;
					c = array[i].bin << push;
					c = c >> push;
					//printBits(c);
					added = overflow;
				}
				else {
					//fprintf(stdout, "just chillding\n");
					c = c << array[i].level;
					c = c | array[i].bin;
					//printBits(c);
				}
				//fprintf(stdout, "char = %c added after = %d\n", array[i].c, added);
				printBits(c);
			}

		}
	}

	if (added > 0) {
		c = c << (8 - added);

		fprintf(stdout, "last %d\n", overflow);
		printBits(c);

		fprintf(optr, "%c", c);
	}


	fprintf(stdout, "\n");
	fclose(iptr);
	fclose(optr);

	return;
}

int main(int argc, char **argv) 
{
	if (argc != 2) {
		fprintf(stderr, "Invalid input\n");
		return EXIT_FAILURE;
	}

	int uniqueChars = 0;

	long long int *charOccurrences = readFile(argv[1], &uniqueChars);

	if (charOccurrences == NULL) {
		return EXIT_FAILURE;
	}

	fprintf(stdout, "number of uniqueChars = %d\n", uniqueChars);



	printArray(charOccurrences, 256);

	node *tree = buildTree(charOccurrences, uniqueChars);

	fprintf(stdout, "\n");

	huff *array = declareHuff(uniqueChars);

	printPreOrder(tree, array, 0, 0, 0);

	fprintf(stdout, "\n");

	destroyTree(tree);

	//changeHuff(array);

	printHuff(array, uniqueChars);

	huffPrint(argv[1], array, uniqueChars, charOccurrences);

	free(array);
	free(charOccurrences);
	return EXIT_SUCCESS;
}