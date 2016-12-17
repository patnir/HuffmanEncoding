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

long long int* readFile(char *filename, int *uniqueChars, int *totalChars)
{
	FILE *iptr = fopen(filename, "r");

	if (iptr == NULL) {
		fprintf(stderr, "Invalid file\n");
		return NULL;
	}

	fseek(iptr, 0, SEEK_END);
	
	long int location = ftell(iptr);

	fprintf(stdout, "location = %ld\n", location);

	if (location == 0) {
		fprintf(stderr, "Empty file\n");
		fclose(iptr);
		return NULL;
	}

	long long int *charOccurrences = (long long int *)calloc(sizeof(long long int), 256);

	fseek(iptr, 0, SEEK_SET);

	location = ftell(iptr);

	fprintf(stdout, "location = %ld\n", location);

	int i;

	//unsigned long long int a;

	fread(charOccurrences, sizeof(long long int), 256, iptr);

	for (i = 0; i < 256; i++) {
		/*fscanf(iptr, "%llu", &(a));
		charOccurrences[i] = (long long int) a;
		*totalChars += charOccurrences[i];*/
		if (charOccurrences[i] != 0) {
			*uniqueChars += 1;
			*totalChars += charOccurrences[i];
		}

		/*location = ftell(iptr);

		fprintf(stdout, "location = %ld\n", location);*/
	}

	fprintf(stdout, "\ndone printing a\n");

	fclose(iptr);

	return charOccurrences;
}

void printArray(long long int* array, int length) 
{
	int i;
	for (i = 0; i < length; i++) {
		fprintf(stdout, "%c%lld ", i, array[i]);
	}
	fprintf(stdout, "\n");
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
			fprintf(stdout, "%c->%lld  ", platform[i]->c, platform[i]->occurrences);
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

huff *declareHuff(int uniqueChars) 
{
	huff *array = (huff *)malloc(sizeof(huff) * uniqueChars);

	return array;
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

void decompress(char *filename, node *tree, int totalChars) 
{
	FILE *fptr = fopen(filename, "r");

	char huffFilename[(int)strlen(filename) + 3];

	int j;

	for (j = 0; j < strlen(filename); j++) {
		huffFilename[j] = filename[j];
	}

	huffFilename[(int)strlen(filename) - 5] = '.';
	huffFilename[(int)strlen(filename) - 4] = 'u';
	huffFilename[(int)strlen(filename) - 3] = 'n';
	huffFilename[(int)strlen(filename) - 2] = 'h';
	huffFilename[(int)strlen(filename) - 1] = 'u';
	huffFilename[(int)strlen(filename)] = 'f';
	huffFilename[(int)strlen(filename) + 1] = 'f';
	huffFilename[(int)strlen(filename) + 2] = '\0';

	FILE *optr = fopen(huffFilename, "w+");

	fseek(fptr, 0, SEEK_END);
	
	long int location = ftell(fptr);

	fprintf(stdout, "location = %ld\n", location);

	fseek(fptr, 256 * sizeof(long long int), SEEK_SET);

	int c;

	int count = 0;

	node *root = tree;

	while ((c = fgetc(fptr)) != EOF && totalChars > 0) {
		fprintf(stdout, "%c count = %d\n", c, ++count);
		unsigned char mask = 1 << 7;
		while (mask > 0 && totalChars > 0) {
			if (root->left == NULL && root->right == NULL) {
				fprintf(optr, "%c", root->c);
				totalChars -= 1;
				root = tree;
			}
			else if ((mask & c) == mask) {
				root = root->right;			
				fprintf(stdout, "moved right\n");
				mask = mask >> 1;
			}
			else {
				root = root->left;
				fprintf(stdout, "moved left\n");
				mask = mask >> 1;
			}
		}		
	}

	fclose(fptr);
	fclose(optr);
}

int main(int argc, char **argv) 
{
	fprintf(stdout, "Invalid input\n");


	if (argc != 2) {
		fprintf(stderr, "Invalid input\n");
		return EXIT_FAILURE;
	}

	int uniqueChars = 0;
	int totalChars = 0;

	long long int *charOccurrences = readFile(argv[1], &uniqueChars, &totalChars);

	if (charOccurrences == NULL) {
		fprintf(stdout, "Error creating occurrences\n");
		return EXIT_FAILURE;
	}

	fprintf(stdout, "number of uniqueChars = %d\n", uniqueChars);
	fprintf(stdout, "total number of chars = %d\n", totalChars);

	printArray(charOccurrences, 256);

	
	node *tree = buildTree(charOccurrences, uniqueChars);

	fprintf(stdout, "\n");

	huff *array = declareHuff(uniqueChars);

	printPreOrder(tree, array, 0, 0, 0);

	fprintf(stdout, "\n");

	decompress(argv[1], tree, totalChars);

	destroyTree(tree);
	free(charOccurrences);
	free(array);

	return EXIT_SUCCESS;
}
