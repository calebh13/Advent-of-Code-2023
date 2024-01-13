#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NETWORK_SIZE 714 // num of nodes: 714 in input.txt, 8 for test
#define INPUT_FILE "input.txt" // input.txt
#define NUM_INSTRUCTIONS 269 // 269 in input.txt. 2 for test
#define LEFT 0
#define RIGHT 1

typedef struct
{
	char name[4]; // 3 chars + 1 for null char
	char branches[2][4];
} Node;

long long gcd(long long a, long long b);
long long lcm(long long a, long long b);
int filledIn(int pathCycles[6]);
long long traverseNodes(Node network[NETWORK_SIZE], int instructions[NUM_INSTRUCTIONS]);
void getInstructions(FILE* fp, int instructions[NUM_INSTRUCTIONS]);
void getNodes(FILE* fp, Node network[NETWORK_SIZE]);
void advanceToNextWord(char buf[BUFSIZ], int* index);

int main(void)
{
	Node network[NETWORK_SIZE] = { 0 };
	int instructions[NUM_INSTRUCTIONS] = { '\0' };
	FILE* fp = fopen(INPUT_FILE, "r");
	long long stepCounter = 0;

	getInstructions(fp, instructions);
	getNodes(fp, network);
	stepCounter = traverseNodes(network, instructions);
	
	printf("Total steps to get to ZZZ: %lld\n", stepCounter);

	return 0;
}

long long gcd(long long a, long long b)
{
	while (1) {
		if (a >= b) {
			a -= b;
		}
		else if (a < b) {
			b -= a;
		}
		if (a == 0)
			return b;
	}
}

long long lcm(long long a, long long b)
{
	return (a * b) / gcd(a, b);
}

int filledIn(int pathCycles[6])
{
	for (int i = 0; i < 6; i++)
	{
		if (pathCycles[i] == 0)
		{
			//printf("not there yet ...\n");
			return 0;
		}
	}
	return 1;
}

long long traverseNodes(Node network[NETWORK_SIZE], int instructions[NUM_INSTRUCTIONS])
{
	long long stepCounter = 1;
	char nodeNames[6][4] = { {"AAA"}, {"BFA"}, {"VGA"}, {"DXA"}, {"VJA"}, {"BPA"} }; // yes, it's hardcoded
	//char nodeNames[2][4] = { {"AAA"}, {"BBA"} };
	int nodeNums[6] = { 210, 297, 539, 575, 623, 674 };
	long long pathCycles[6] = { 0,0,0,0,0,0 };
	//int nodeNums[2] = { 0, 3 };
	//printf("Step counter: %d\n", stepCounter);
	for (int pathNum = 0; pathNum < 6; pathNum++)
	{
		for (stepCounter = 1; !pathCycles[pathNum]; stepCounter++)
		{
			//printf("pathNum: %d\n", pathNum);
			for (nodeNums[pathNum] = 0; strcmp(network[nodeNums[pathNum]].name, nodeNames[pathNum]) != 0; (nodeNums[pathNum])++); // search for nodeName
			//printf("nodenum: %d\n", nodeNums[pathNum]);
			printf("nextNode: %s.\n", network[nodeNums[pathNum]].branches[instructions[(stepCounter - 1) % NUM_INSTRUCTIONS]]);
			strcpy(nodeNames[pathNum], network[nodeNums[pathNum]].branches[instructions[(stepCounter - 1) % NUM_INSTRUCTIONS]]); // set next node to whatever instructions say
			//printf("copied successfully!\n");
			//stepCounter++;
			if (nodeNames[pathNum][2] == 'Z')
			{
				pathCycles[pathNum] = stepCounter;
				printf("steps: %lld\n", stepCounter);
				stepCounter = 1;
				system("pause");
			}
		}
		//printf("Step counter: %d\n\n", stepCounter);
	}
	long long lcm12 = lcm(pathCycles[0], pathCycles[1]), lcm34 = lcm(pathCycles[2], pathCycles[3]), lcm56 = lcm(pathCycles[4], pathCycles[5]);
	printf("TOTAL STEPS: %lld\n", lcm(lcm(lcm12, lcm34), lcm56));
	
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
