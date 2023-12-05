#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define ROWS 218
#define COLS 119 // should be 117, but is +2 for CRLF (or maybe \n\0 ?)
#define STARTING_COL 10 // this is where numbers begin on each line
#define WINNING_NUMBERS 10 // should be 10
#define NUMBERS_DRAWN 25 // should be 25

int getNextNumber(char line[COLS]);
void initializeCardCounts(int cardCounts[ROWS]);
int countCards(int cardCounts[ROWS]);

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

void initializeCardCounts(int cardCounts[ROWS])
{
	for (int i = 0; i < ROWS; i++)
	{
		cardCounts[i] = 1;
	}
}

int countCards(int cardCounts[ROWS])
{
	int sum = 0;
	for (int i = 0; i < ROWS; i++)
	{
		printf("cardCounts[%d]: %d\n", i + 1, cardCounts[i]);
		sum += cardCounts[i];
	}
	return sum;
}

int main(void)
{
	char buf[COLS] = { '\0' };
	int winningNumbers[WINNING_NUMBERS] = { 0 };
	int drawnNumbers[NUMBERS_DRAWN] = { 0 };
	int cardCounts[ROWS] = { 0 };
	initializeCardCounts(cardCounts);
	int numMatches = 0, points = 0, i = 0;
	FILE* fp = fopen("input.txt", "r");
	for (int card = 0; card < ROWS; card++)
	{
		fgets(buf, COLS, fp);
		for (i = 0; i < WINNING_NUMBERS; i++)
		{
			winningNumbers[i] = getNextNumber(buf);
			printf("winningNum[%d]: %d\n", i, winningNumbers[i]);
		}
		printf("\n");
		for (i = 0; i < NUMBERS_DRAWN; i++)
		{
			drawnNumbers[i] = getNextNumber(buf);
			printf("drawnNumber[%d]: %d\n", i, drawnNumbers[i]);
			for (int j = 0; j < WINNING_NUMBERS; j++) // Look for matches and increment numMatches if they exist
			{
				if (drawnNumbers[i] == winningNumbers[j]) numMatches++;
			}
		}
		printf("numMatches for card %d: %d\n", card + 1, numMatches);
		for (i = 1; i <= numMatches && card + i < ROWS; i++)
		{
			cardCounts[card + i] += cardCounts[card];
			printf("cardCounts[%d]: %d\n", card + i + 1, cardCounts[card]);
			// The number of cards we currently have equals the number of cards that get added to each count.
			// Say we're on card 2. We have 3 copies of card 2 total. Card 2 gave us 4 matches. As a result of this,
			// cards 3, 4, 5, and 6 will now have 3 copies added to their counts, and so will all have a total of 4 cards.
		}
		numMatches = 0;
		printf("\n\n");
	}
	printf("Total cards: %d.\n", countCards(cardCounts));
	return 0;
}
