#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define ROWS 218
#define COLS 119 // should be 117, but is +2 for CRLF (or maybe \n\0 ?)
#define STARTING_COL 10 // this is where numbers begin on each line

int getNextNumber(char line[COLS]);

int getNextNumber(char line[COLS])
{
	static int index = STARTING_COL;
	if (index >= (COLS - 3)) index = STARTING_COL; // reset to STARTING_COL once we get to the end of the line
	char buf[3] = { '\0' }; // There are no numbers with more than 2 digits, so this works fine

	//printf("index: %d\n", index);
	//printf("Starting char: %c.\n", line[index]);
	for (; !isdigit(line[index]); index++); // get to next digit
	int start = index;
	for (; isdigit(line[index]); index++)
	{
		buf[index - start] = line[index];
	}
	buf[2] = '\0';
	return atoi(buf);
}

int main(void)
{
	char buf[COLS] = { '\0' };
	int winningNumbers[10] = { 0 };
	int drawnNumbers[25] = { 0 };
	int numMatches = 0, points = 0;
	FILE* fp = fopen("input.txt", "r");
	for (int line = 0; line < ROWS; line++)
	{
		fgets(buf, COLS, fp);
		for (int i = 0; i < 10; i++)
		{
			winningNumbers[i] = getNextNumber(buf);
			printf("winningNum[%d]: %d\n", i, winningNumbers[i]);
		}
		printf("\n");
		for (int i = 0; i < 25; i++)
		{
			drawnNumbers[i] = getNextNumber(buf);
			printf("drawnNumber[%d]: %d\n", i, drawnNumbers[i]);
			for (int j = 0; j < 10; j++) // Look for matches and increment numMatches if they exist
			{
				if (drawnNumbers[i] == winningNumbers[j]) numMatches++; 
			}
		}
		points += (1 << (numMatches - 1)); // Points are just powers of 2 so this is a faster/cleaner way to do it
		numMatches = 0;
		printf("\n\n");
	}
	printf("points: %d\n", points);
  return 0;
}
