#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define NUM_RACES 1

long long calcWaysToWin(long long raceTime, long long distance);
long long calcDistance(long long raceTime, long long buttonTime);
void readFile(FILE* fp, long long times[NUM_RACES], long long distances[NUM_RACES]);
long long getNextNumber(char* line, int* index);

int main(void)
{
	long long times[NUM_RACES] = { 0 };
	long long distances[NUM_RACES] = { 0 };
	FILE* fp = fopen("input.txt", "r");
	readFile(fp, times, distances);
	int waysToWin = 0, product = 1; // multiplicative identity is 1, not 0 :)
	for (int i = 0; i < NUM_RACES; i++)
	{
		printf("times[%d]: %lld, distances[%d]: %lld\n", i, times[i], i, distances[i]);
		waysToWin = calcWaysToWin(times[i], distances[i]);
		printf("Ways to win: %lld\n", waysToWin);
		product *= waysToWin;
	}
	printf("Prod: %lld\n", product);

	return 0;
}

long long calcWaysToWin(long long raceTime, long long distance)
{
	// Quadratic formula is below. Our quadratic is -x^2 + tx - d, where t = raceTime and d = distance
	printf("raceTime^2: %lld\n", raceTime * raceTime);
	long double root1 = (( - 1 * raceTime) + sqrt((raceTime * raceTime) - (4 * distance))) / (-2);
	printf("a: %.2lf\n", root1);
	long double root2 = (( - 1 * raceTime) - sqrt((raceTime * raceTime) - (4 * distance))) / (-2);
	printf("b: %.2lf\n", root2);
	// root2 > root1 in this case
	// Determine number of integers inbetween the two roots:
	return (long long)ceil(root2 - 1) - (long long)floor(root1 + 1) + 1;
}

long long calcDistance(long long raceTime, long long buttonTime)
{
	// Note: velocity never decreases or increases. It is just set to however long you pressed the button
	return buttonTime * (raceTime - buttonTime);
}

void readFile(FILE* fp, long long times[NUM_RACES], long long distances[NUM_RACES])
{
	char buf1[BUFSIZ] = { '\0' }, buf2[BUFSIZ] = { '\0' };
	int index1 = 0, index2 = 0;
	fgets(buf1, BUFSIZ, fp);
	fgets(buf2, BUFSIZ, fp);
	for (int i = 0; i < NUM_RACES; i++)
	{
		times[i] = getNextNumber(buf1, &index1);
		distances[i] = getNextNumber(buf2, &index2);
	}
}

long long getNextNumber(char* line, int* index)
{
	if (line[*index] == '\0') *index = 0; // reset to 0 once we get to the end of the line
	char buf[BUFSIZ] = { '\0' };

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
	return strtoll(buf, NULL, 10);
}
