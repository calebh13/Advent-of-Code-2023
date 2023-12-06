#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int calcWaysToWin(int raceTime, int distance);
int calcDistance(int raceTime, int buttonTime);
void readFile(FILE* fp, int times[4], int distances[4]);
int getNextNumber(char* line, int* index);

int main(void)
{
	int times[4] = { 0 };
	int distances[4] = { 0 };
	FILE* fp = fopen("input.txt", "r");
	readFile(fp, times, distances);
	int waysToWin = 0, product = 1; // multiplicative identity is 1, not 0 :)
	for (int i = 0; i < 4; i++)
	{
		printf("times[%d]: %d, distances[%d]: %d\n", i, times[i], i, distances[i]);
		waysToWin = calcWaysToWin(times[i], distances[i]);
		printf("Ways to win: %d\n", waysToWin);
		product *= waysToWin;
	}
	printf("Prod: %d\n", product);

	return 0;
}

int calcWaysToWin(int raceTime, int distance)
{
	// Quadratic formula is below. Our quadratic is -x^2 + tx - d, where t = raceTime and d = distance
	double root1 = (-1 * raceTime + sqrt(raceTime * raceTime - 4 * distance)) / (-2);
	double root2 = (-1 * raceTime - sqrt(raceTime * raceTime - 4 * distance)) / (-2);
	// root2 > root1 in this case
	// Determine number of integers inbetween the two roots:
	return (int)ceil(root2 - 1) - (int)floor(root1 + 1) + 1;
}

int calcDistance(int raceTime, int buttonTime)
{
	// Note: velocity never decreases or increases. It is just set to however long you pressed the button
	return buttonTime * (raceTime - buttonTime);
}

void readFile(FILE* fp, int times[4], int distances[4])
{
	char buf1[BUFSIZ] = { '\0' }, buf2[BUFSIZ] = { '\0' };
	int index1 = 0, index2 = 0;
	fgets(buf1, BUFSIZ, fp);
	fgets(buf2, BUFSIZ, fp);
	for (int i = 0; i < 4; i++)
	{
		times[i] = getNextNumber(buf1, &index1);
		distances[i] = getNextNumber(buf2, &index2);
	}
}

int getNextNumber(char* line, int* index)
{
	if (line[*index] == '\0') *index = 0; // reset to 0 once we get to the end of the line
	char buf[11] = { '\0' }; // There are no numbers with more than 10 digits, because that is the 32-bit integer limit and input.txt only has integers

	//printf("*index: %d\n", *index);
	//printf("Starting char: %c.\n", line[*index]);
	for (; !isdigit(line[*index]); (*index)++); // get to next digit
	//printf("index: %d\n", *index);
	int start = *index;
	for (; isdigit(line[*index]); (*index)++)
	{
		//printf("char: %c\n", line[*index]);
		buf[*index - start] = line[*index];
	}
	buf[*index - start] = '\0';
	//printf("getNextNumber buf: %s.\n", buf);
	return atoi(buf);
}
