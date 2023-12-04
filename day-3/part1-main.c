#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
// Following 2 constants are based off of input.txt
#define ROWS 140
#define COLS 141 // 140 chars for each line PLUS null char

int isValidIndex(int index);
void readFileData(char text[ROWS][COLS]);
int isValidNumber(char text[ROWS][COLS], int row, int col);
int findNextNumber(char text[ROWS][COLS], int line, int* colIndex);
int getNumberValue(char text[ROWS][COLS], int line, int* colIndex);

int isValidIndex(int index)
{
	return (index >= 0 && index < ROWS);
}

void readFileData(char text[ROWS][COLS])
{
	char buf[COLS]; 
	FILE* fp = fopen("input.txt", "r");
	for (int i = 0; i < ROWS; i++)
	{
		fgets(buf, COLS, fp);
		for (int j = 0; j < COLS; j++)
		{
			text[i][j] = buf[j];
		}
		// no idea why, but there's some problem with the textfile that causes me to need to read each line twice, as every other line was blank
		// if your input.txt behaves differently, you may need to remove this line.
		fgets(buf, COLS, fp);
	}
}

// Works for 1 specific row and col: needs to be called one time for each DIGIT of any number
int isValidNumber(char text[ROWS][COLS], int row, int col)
{
	char c = '\0';
	// Technically this does check for symbols with row and col increments of 0, but it doesn't cause any problems due to the !isdigit(c) check

	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			if (isValidIndex(row + i) && isValidIndex(col + j))
			{
				c = text[row + i][col + j];
				//printf("c = text[%d][%d] = %c\n", row + i, col + j, c);
				if (c != '.' && !isdigit(c))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

// Moves colIndex to the next index of a number
// Returns 0 if no number left in the line
int findNextNumber(char text[ROWS][COLS], int line, int* colIndex)
{
	if (!isValidIndex(*colIndex) || !isValidIndex(line)) return 0;
	for (; !isdigit(text[line][*colIndex]); (*colIndex)++)
	{
		if (!isValidIndex(*colIndex)) return 0;
	}
	return 1;
}

// Finds the value of the number colIndex is currently at the start of, returns that, and then moves colIndex to the next non-digit character
int getNumberValue(char text[ROWS][COLS], int line, int* colIndex)
{
	printf("First digit of number to find value of: %c\n", text[line][*colIndex]);
	char buf[20] = { '\0' };
	int i = 0;
	for (; isValidIndex(*colIndex + i) && isdigit(text[line][*colIndex + i]); i++)
	{
		buf[i] = text[line][*colIndex + i];
		printf("buf[%d]: %c\n", i, buf[i]);
	}
	*colIndex = *colIndex + i;
	return atoi(buf);
}

int main(void)
{
	char text[ROWS][COLS] = { {'\0'} };
	readFileData(text);
	printf("File data read into array successfully.\n");
	int colIndex = 0, curNum = 0, sum = 0;
	int isValidNum = 0, i = 0;

	
	for (int line = 0; line < ROWS; line++)
	{
		while (findNextNumber(text, line, &colIndex)) // If there is a number left in the line
		{
			printf("Checking number at column %d\n", colIndex);
			printf("LINE: %d\n", line);
			for (i = 0; isdigit(text[line][colIndex + i]); i++) // Check each of its digits for adjacent symbols
			{
				if (isValidNumber(text, line, colIndex + i)) // If there is a symbol adjacent to this digit
				{
					isValidNum = 1; 
					printf("Valid number spotted!\n");
					break; // It's valid, so we can quit
				}
			}
			//printf("Past for loop: validity = %d\n", isValidNum);
			curNum = getNumberValue(text, line, &colIndex);
			if (isValidNum)
			{
				sum += curNum;
			}
			//printf("Column moved to %d\n", colIndex);
			printf("Sum: %d.\n", sum);
			isValidNum = 0;
			//system("pause");
		}
		colIndex = 0;
	}
	

	return 0;
}
