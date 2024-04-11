#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 10

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType data;
	struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
	const float A = 0.6180339887; // A constant recommended for multiplication method
	float frac = x * A - (int)(x * A);
	return (int)(frac * HASH_SIZE);
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		printf("Index %d -> ", i);
		struct HashType* current = (pHashArray + i)->next;
		while (current != NULL)
		{
			printf("%d, %c, %d -> ", current->data.id, current->data.name, current->data.order);
			if (current->next != NULL) {
				current = current->next;
			} else {
				break; // Exit the loop if there are no more elements in the linked list
			}
		}
		printf("NULL\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	// Create hash table
	struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType) * HASH_SIZE);
	if (hashTable == NULL)
	{
		printf("Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}

	// Initialize hash table
	for (int i = 0; i < HASH_SIZE; ++i) 
	{
		hashTable[i].next = NULL; // Initialize each slot to NULL
	}

	// Insert records into the hash table
	for (int i = 0; i < recordSz; ++i)
	{
		int index = hash(pRecords[i].id); // Get hash index
		struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType));
		if (newNode == NULL)
		{
			printf("Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
		newNode->data = pRecords[i];
		newNode->next = hashTable[index].next;
		hashTable[index].next = newNode;
	}

	// Display records in the hash table
	displayRecordsInHash(hashTable, HASH_SIZE);

	// Free memory
	free(pRecords);
	for (int i = 0; i < HASH_SIZE; ++i)
	{
		struct HashType* current = hashTable[i].next;
		while (current != NULL)
		{
			struct HashType* temp = current;
			current = current->next;
			free(temp);
		}
	}
	free(hashTable);

	return 0;
}