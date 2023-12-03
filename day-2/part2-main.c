#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define RED_MAX 12
#define GREEN_MAX 13
#define BLUE_MAX 14

int getGameNum(char* buf);
int determineColorIndex(char* buf);
int gameWouldBePossible(int colorMaxes[4]);


int getGameNum(char* buf)
{
	buf[strlen(buf) - 1] = '\0'; // removes colon
	return atoi(buf);
}

int determineColorIndex(char* buf)
{
	static const char colors[3][6] = { "red", "green", "blue" };
	//printf("determineColorIndex buf: %s.\n", buf);
	int retVal = 1;
	int len = strlen(buf);
	//printf("buf[len - 1]: %c.\n", buf[len - 1]);
	switch (buf[len - 1])
	{
	case ';': // End of a group so we flip it to be negative while preserving the index (so that we can do something with this info in main)
		retVal = -1;
		buf[len - 1] = '\0';
		break;
	case ',': // Middle of a group
		buf[len - 1] = '\0';
		break;
	default: // Only reached when it is the end of a line
		retVal = 10;
		break;
	}
		
	for (int i = 0; i < 3; i++)
	{
		if (strcmp(buf, colors[i]) == 0) // color matches
		{
			return retVal * (i + 1);
		}
	}
	return 0;
}

int main(void)
{
	FILE* fp = fopen("input.txt", "r");
	int sumOfPowers = 0;
	char buf[BUFSIZ] = { '\0' };
	int colorMaxes[4] = { 0, 0, 0, 0 }; // (nothing for better indexing), red, green, blue
	int colorIndex = 0, curCount = 0;
	int endOfLine = 0, tempIndex = 0;
	int curGameNum = 0;

	for (int line = 0; !feof(fp); line++)
	{
		// we will just use fscanf to process word by word ... not very efficient but it gets the job done
		fscanf(fp, "%s", buf); // gets past "Game"
		fscanf(fp, "%s", buf); // gets number
		curGameNum = getGameNum(buf); // converts number to integer

		while (!endOfLine) // Loop through game
		{
			while (colorIndex > -1) // Loop through round
			{
				fscanf(fp, "%s", buf); // gets number of first color
				curCount = atoi(buf); // convert to int so we now have the # of occurrences of that color
				fscanf(fp, "%s", buf); // gets the name of the color
				colorIndex = determineColorIndex(buf);

				if (colorIndex < 0)
				{
					tempIndex = colorIndex * -1;
				}
				else if (colorIndex >= 10)
				{
					tempIndex = colorIndex / 10;
					colorIndex = -1; // to break out of the while loop
					endOfLine = 1;
				}
				else
				{
					tempIndex = colorIndex;
				}

				//printf("colorIndex: %d\n", colorIndex);
				if (curCount > colorMaxes[tempIndex]) colorMaxes[tempIndex] = curCount;
			}
			colorIndex = 0;
		}
		// Calc power
		printf("line %d: %dr, %dg, %db\n", line, colorMaxes[1], colorMaxes[2], colorMaxes[3]);
		printf("power: %d\n", colorMaxes[1] * colorMaxes[2] * colorMaxes[3]);
		sumOfPowers += (colorMaxes[1] * colorMaxes[2] * colorMaxes[3]);
		colorIndex = 0;
		colorMaxes[1] = 0, colorMaxes[2] = 0, colorMaxes[3] = 0;
		endOfLine = 0;
	}
	printf("sumOfPowers: %d\n", sumOfPowers);


	return 0;
}
