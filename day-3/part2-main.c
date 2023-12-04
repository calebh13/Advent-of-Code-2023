#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
// Following 2 constants are based off of input.txt
#define ROWS 140 // real version: 140 lines of input.txt
#define COLS 141 // real version: 140 chars for each line PLUS null char = 141

int isValidRow(int row);
int isValidCol(int col);
int encodeDirection(int rowIncrement, int colIncrement);
void decodeDirection(int dir, int* rowIncrement, int* colIncrement);
void readFileData(char text[ROWS][COLS]);
int couldBeGear(char text[ROWS][COLS], int* row, int* col);
int findNextGear(char text[ROWS][COLS], int row, int col, int prevDir);
int findNextNumber(char text[ROWS][COLS], int line, int* colIndex);
int getNumberValue(char text[ROWS][COLS], int line, int* colIndex);

int isValidRow(int row)
{
	return (row >= 0 && row < ROWS);
}
int isValidCol(int col)
{
	return (col >= 0 && col < COLS);
}

int encodeDirection(int rowIncrement, int colIncrement)
{
	return (rowIncrement * 10) + colIncrement;
}

void decodeDirection(int dir, int* rowIncrement, int* colIncrement)
{
	// This is a disgusting hacky function and I hate it
	// Please do not read this code
	printf("dir %% 10: %d.\n", (dir % 10));
	int posVal = abs(dir);
	int sign = 0;
	if (dir > 0) sign = 1;
	if (dir < 0) sign = -1;

	if (sign == 1)
	{
		*rowIncrement = sign;
		if (dir % 10 == 9)
		{
			*colIncrement = -1;
		}
		else if (dir % 10 == 1)
		{
			*colIncrement = 1;
		}
		else
		{
			*colIncrement = 0;
		}
	}
	else if (sign == -1)
	{
		if (posVal % 10 == 9)
		{
			*colIncrement = 1;
		}
		else if (posVal % 10 == 1)
		{
			*colIncrement = -1;
		}
		else
		{
			*colIncrement = 0;
		}
	}
	else
	{
		*rowIncrement = 0;
		*colIncrement = 0;
	}
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
// Changes row and col to location of '*' if an adjacent one exists, and returns direction that we took to get to the '*' (if there is no '*', returns 0).
int couldBeGear(char text[ROWS][COLS], int* row, int* col)
{
	char c = '\0';
	// Technically this does check for symbols with row and col increments of 0, but it doesn't cause any problems

	for (int rowIncrement = -1; rowIncrement <= 1; rowIncrement++)
	{
		for (int colIncrement = -1; colIncrement <= 1; colIncrement++)
		{
			if (isValidRow(*row + rowIncrement) && isValidCol(*col + colIncrement))
			{
				c = text[*row + rowIncrement][*col + colIncrement];
				//printf("c = text[%d][%d] = %c\n", *row + rowIncrement, *col + colIncrement, c);
				if (c == '*')
				{
					*row = *row + rowIncrement;
					*col = *col + colIncrement;
					return encodeDirection(rowIncrement, colIncrement);
				}
			}
		}
	}
	return 0;
}

// Returns value of next gear, 0 if there is none
// Row and col are position of '*', dir is direction we took to get to the '*'
int findNextGear(char text[ROWS][COLS], int row, int col, int prevDir)
{
	char c = '\0';
	int prevRowIncrement = 0, prevColIncrement = 0;
	int curCol = 0; // only necessary because getNumberValue requires a pointer for 3rd parameter

	for (int rowIncrement = -1; rowIncrement <= 1; rowIncrement++)
	{
		for (int colIncrement = -1; colIncrement <= 1; colIncrement++)
		{
			// Check to make sure that we are within array bounds 
			if (isValidRow(row + rowIncrement) && isValidCol(col + colIncrement))
			{
				c = text[row + rowIncrement][col + colIncrement];
				decodeDirection(prevDir, &prevRowIncrement, &prevColIncrement);
				printf("c = text[%d][%d] = %c\n", row + rowIncrement, col + colIncrement, c);
				printf("Prev row inc: %d. Cur row inc (rev): %d. Prev col inc: %d. Cur col inc (rev): %d.\n", prevRowIncrement, -1 * rowIncrement, prevColIncrement, -1 * colIncrement);
				if (isdigit(c) && !(-1 * rowIncrement == prevRowIncrement && -1 * colIncrement == prevColIncrement)) // makes sure that we don't get duplicates
				{
					printf("Was valid.\n");
					// Note that calling this will not mess with our current position in text[] because 
					// the parameters of THIS function are not pointers and thus no changes occur in main() scope
					//printf("c is a digit: %c.\n", c);
					curCol = col + colIncrement;
					return getNumberValue(text, row + rowIncrement, &curCol);
				}
			}
		}
	}
	printf("\n");
	return 0;
}

// Moves colIndex to the next index of a number
// Returns 0 if no number left in the line
int findNextNumber(char text[ROWS][COLS], int line, int* colIndex)
{
	if (!isValidCol(*colIndex) || !isValidRow(line)) return 0;
	for (; isValidCol(*colIndex) && !isdigit(text[line][*colIndex]); (*colIndex)++)
	{
		if (!isValidCol(*colIndex)) return 0;
	}
	return 1;
}

// Finds the value of the number that colIndex is in, returns that, and then moves colIndex to the next non-digit character
int getNumberValue(char text[ROWS][COLS], int line, int* colIndex)
{
	for (; isValidCol(*colIndex - 1) && isdigit(text[line][*colIndex - 1]); (*colIndex)--); // Set colIndex to the first digit of the number
	printf("First digit of number to find value of: %c\n", text[line][*colIndex]);
	char buf[20] = { '\0' };
	int i = 0;
	for (; isValidCol(*colIndex + i) && isdigit(text[line][*colIndex + i]); i++)
	{
		buf[i] = text[line][*colIndex + i];
		text[line][*colIndex + i] = '.';
		printf("buf[%d]: %c\n", i, buf[i]);
	}
	*colIndex = *colIndex + i;
	//text[line][*colIndex - 1] = '.';
	text[line][COLS - 1] = '\0';
	printf("Line after finding number value: %s\n", text[line]);
	return atoi(buf);
}

int main(void)
{
	char text[ROWS][COLS] = { {'\0'} };
	readFileData(text);
	printf("File data read into array successfully.\n");
	int colIndex = 0, curNum = 0, sum = 0, nextNum = 0;
	int prevGearRatio = 0, curGearRatio = 0;
	int curRow, curCol = 0; // used as location for '*'s
	int curDir = 0;
	int isValidNum = 0;

	
	for (int line = 0; line < ROWS; line++)
	{
		while (findNextNumber(text, line, &colIndex)) // If there is a number left in the line
		{
			printf("Checking number at column %d\n", colIndex);
			printf("LINE: %d\n", line);
			for (int i = 0; isValidCol(colIndex + i) && isdigit(text[line][colIndex + i]); i++) // Check each of its digits for adjacent '*'s
			{
				curRow = line, curCol = colIndex + i;
				curDir = couldBeGear(text, &curRow, &curCol);
				if (curDir != 0) // If there is a '*' somewhere adjacent to this digit
				{
					isValidNum = 1;
					printf("Valid number spotted!\n");
					break; // It's valid, so we can quit the loop
				}
			}
			curNum = getNumberValue(text, line, &colIndex); // Increments colIndex to next non-digit AND gets value of number
			//printf("Past for loop: validity = %d\n", isValidNum);
			printf("curNum: %d\n", curNum);
			if (isValidNum)
			{
				printf("Number is valid, let's find next gear.\n");
				nextNum = findNextGear(text, curRow, curCol, curDir); // remember curRow and curCol are now set to the position of the previously found '*'
				// Because it returns 0, there's no harm in multiplying them together and adding them to the sum, because it will just add 0 and not affect anything
				printf("Gear ratio: %d * %d.\n", curNum, nextNum);
				curGearRatio = curNum * nextNum;
				if (curGearRatio != prevGearRatio) sum += curGearRatio; // this is a hacky way to prevent duplicates ... but it should work for this input data.
				// the main issue is things like "464*47": without this line it would do 464*47 and 47*464. this is just the easiest way to fix that
				prevGearRatio = curGearRatio;
			}
			//printf("Column moved to %d\n", colIndex);
			printf("Sum: %d.\n", sum);
			isValidNum = 0;
		}
		colIndex = 0;
		printf("\n\n\n\n");
		if(line > 139) system("pause");
	}

	return 0;
}
