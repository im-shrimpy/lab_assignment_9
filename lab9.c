#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RecordType
struct RecordType
{
	int	id;
	char name;
	int	order; 
};

// Fill out this structure
typedef struct HashType
{
	struct RecordType *data;
	struct HashType *next;
}thing;

// Compute the hash function
int hash(int x,int tableSize)
{
	return x%tableSize;

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
void displayRecordsInHash(thing *array,int i)
{
	thing *cur=array;
	printf("index %d -> ",i);
	while(cur!=NULL)
	{
		printf("%d, %c, %d -> ",cur->data->id,cur->data->name,cur->data->order);
		cur=cur->next;
		// if index is occupied with any records, print all
	}
	printf("NULL\n");
}



void insertTable(thing **hashTable, struct RecordType *record,int tableSize)
{
    int key = hash(record->id, tableSize);
    thing *new = malloc(sizeof(thing));
    if (new == NULL) {
        printf("Cannot allocate memory\n");
        exit(-1);
    }
    new->data = record;
    new->next = NULL;
    if (hashTable[key] == NULL) {
        hashTable[key] = new;
    } else {
        thing *cur = hashTable[key];
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = new;
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
    // Your hash implementation
    int TABLE_SIZE = 11;
    thing **table = calloc(TABLE_SIZE, sizeof(thing *));
    for (int i = 0; i < recordSz; i++) {
        insertTable(table, pRecords + i,TABLE_SIZE);
    }
    for (int i = 0; i < TABLE_SIZE; i++) {
        displayRecordsInHash(table[i], i);
    }
    // Free dynamically allocated memory
    for (int i = 0; i < TABLE_SIZE; i++) {
        thing *cur = table[i];
        while (cur != NULL) {
            thing *next = cur->next;
            free(cur->data);
            free(cur);
            cur = next;
        }
    }
    free(table);
    free(pRecords);
    return 0;
}
