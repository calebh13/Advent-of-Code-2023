#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NETWORK_SIZE 714 // num of nodes: 714 in input.txt, 9 for test
#define INPUT_FILE "input.txt" // input.txt
#define NUM_INSTRUCTIONS 269 // 269 in input.txt. 2 for test
#define LEFT 0
#define RIGHT 1

typedef struct
{
	char name[4]; // 3 chars + 1 for null char
	char branches[2][4];
} Node;

int traverseNodes(Node network[NETWORK_SIZE], int instructions[NUM_INSTRUCTIONS]);
void getInstructions(FILE* fp, int instructions[NUM_INSTRUCTIONS]);
void getNodes(FILE* fp, Node network[NETWORK_SIZE]);
void advanceToNextWord(char buf[BUFSIZ], int* index);

int main(void)
{
	Node network[NETWORK_SIZE] = { 0 };
	int instructions[NUM_INSTRUCTIONS] = { '\0' };
	FILE* fp = fopen(INPUT_FILE, "r");
	int stepCounter = 0;

	getInstructions(fp, instructions);
	getNodes(fp, network);
	stepCounter = traverseNodes(network, instructions);
	
	printf("Total steps to get to ZZZ: %d\n", stepCounter);

	return 0;
}

int traverseNodes(Node network[NETWORK_SIZE], int instructions[NUM_INSTRUCTIONS])
{
	int stepCounter = 0; // so initial search of AAA makes it equal to 0
	char nodeName[4] = "AAA";
	for (; strcmp("ZZZ", nodeName) != 0; stepCounter++)
	{
		int nodeNum = 0;
		for (; strcmp(network[nodeNum].name, nodeName) != 0; nodeNum++); // search for nodeName
		//printf("nodeNum: %d. nodeName: %s\n", nodeNum, nodeName);
		//printf("nextNode: %s\n", network[nodeNum].branches[instructions[stepCounter % NUM_INSTRUCTIONS]]);
		strcpy(nodeName, network[nodeNum].branches[instructions[stepCounter % NUM_INSTRUCTIONS]]); // set next node to whatever instructions say
		//system("pause");
	}
	return stepCounter;
}

void getInstructions(FILE* fp, int instructions[NUM_INSTRUCTIONS])
{
	char buf[NUM_INSTRUCTIONS + 2] = { '\0' }; // + 2 for \n and \0
	fgets(buf, NUM_INSTRUCTIONS + 1, fp);
	for (int i = 0; i < NUM_INSTRUCTIONS; i++)
	{
		// convert to integer for easier comparisons
		instructions[i] = ((buf[i] == 'L') ? (LEFT) : (RIGHT));
	}
	fgets(buf, NUM_INSTRUCTIONS + 2, fp); // get next newline
	fgets(buf, NUM_INSTRUCTIONS + 2, fp); // get next newline
}

void getNodes(FILE* fp, Node network[NETWORK_SIZE])
{
	char buf[BUFSIZ];
	for (int i = 0; !feof(fp); i++)
	{
		fgets(buf, BUFSIZ, fp);
		int index = 0;
		strncpy(network[i].name, &buf[index], 3);
		advanceToNextWord(buf, &index);
		strncpy(network[i].branches[LEFT], &buf[index], 3);
		advanceToNextWord(buf, &index);
		strncpy(network[i].branches[RIGHT], &buf[index], 3);
	}
}

void advanceToNextWord(char buf[BUFSIZ], int* index)
{
	for (; isalpha(buf[*index]); (*index)++); // advance to next non-letter
	for (; !isalpha(buf[*index]); (*index)++); // advance to next letter
}
