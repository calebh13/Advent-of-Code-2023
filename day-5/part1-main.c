#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#define NUM_SEEDS 20
#define NEW_LINE 1
#define SAME_LINE 0
#define NUM_MAPS 7

// note: for this to work properly you must insert two newlines at the very end of input.txt due to how the file is parsed

typedef struct
{
	unsigned int destination;
	unsigned int source;
	unsigned int range;
} Map;

unsigned int getNextNumber(char* line, int isNewLine);
void fillSeeds(FILE* fp, unsigned int seeds[NUM_SEEDS]);
void fillMaps(FILE* fp, Map maps[NUM_MAPS][50]);
unsigned int findLocation(unsigned int seed, Map maps[NUM_MAPS][50]);

int main(void)
{
	FILE* fp = fopen("input.txt", "r");
	Map maps[NUM_MAPS][50] = { { 0 } }; // nothing has more than 50 maps, and I didn't want to deal with dynamic memory, so this works
	int seeds[NUM_SEEDS] = { 0 };
	fillSeeds(fp, seeds);
	for (int i = 0; i < NUM_SEEDS; i++)
	{
		printf("Seed %u: %u\n", i, seeds[i]);
	}
	fillMaps(fp, maps);
	unsigned int curLocation = UINT_MAX, minLocation = UINT_MAX;
	for (int i = 0; i < NUM_SEEDS; i++)
	{
		curLocation = findLocation(seeds[i], maps);
		printf("Seed %u mapped to %u.\n", seeds[i], curLocation);
		if (curLocation < minLocation)
		{
			minLocation = curLocation;
		}
	}
	printf("\nLowest location value: %u\n", minLocation);

	return 0;
}

unsigned int getNextNumber(char* line, int isNewLine)
{
	static int index = 0;
	if (isNewLine || line[index] == '\0') index = 0; // reset to 0 once we get to the end of the line
	char buf[11] = { '\0' }; // There are no numbers with more than 10 digits, because that is the 32-bit integer limit and input.txt only has integers

	//printf("index: %d\n", index);
	//printf("Starting char: %c.\n", line[index]);
	for (; !isdigit(line[index]); index++); // get to next digit
	int start = index;
	for (; isdigit(line[index]); index++)
	{
		buf[index - start] = line[index];
	}
	buf[index - start] = '\0';
	//printf("getNextNumber buf: %s.\n", buf);
	return (unsigned int)strtoul(buf, NULL, 10);
}

void fillSeeds(FILE* fp, unsigned int seeds[NUM_SEEDS])
{
	char buf[BUFSIZ] = { '\0' };
	fgets(buf, BUFSIZ, fp); // all seeds are on one line
	for (int i = 0; i < NUM_SEEDS; i++)
	{
		seeds[i] = getNextNumber(buf, SAME_LINE);
	}
}

void fillMaps(FILE* fp, Map maps[NUM_MAPS][50])
{
	char buf[BUFSIZ] = {'\0'};
	fgets(buf, BUFSIZ, fp);
	for (int i = 0; i < NUM_MAPS; i++)
	{
		printf("\n\nMap #%d: \n", i);
		// Clear thru extra lines and text
		fgets(buf, BUFSIZ, fp);
		fgets(buf, BUFSIZ, fp);
		int entry = 0;
		int temp = 0;
		while(buf[0] != '\n')
		{
			printf("buf: %s.\n", buf);
			maps[i][entry].destination = getNextNumber(buf, 1);
			maps[i][entry].source = getNextNumber(buf, 0);
			maps[i][entry].range = getNextNumber(buf, 0);
			printf("Entry #%d: Destination %u, Source %u, Range %u\n", entry, maps[i][entry].destination, maps[i][entry].source, maps[i][entry].range);
			entry++;
			//system("pause");
			fgets(buf, BUFSIZ, fp);
		}
	}
}

unsigned int findLocation(unsigned int seed, Map maps[NUM_MAPS][50])
{
	// note: all values of maps that are unset == 0
	unsigned int destination = 0, source = 0, range = 0, prevSeed = 0;
	for (int i = 0; i < NUM_MAPS; i++)
	{
		prevSeed = seed;
		for (int j = 0; maps[i][j].range != 0; j++) // range is never 0, so once we reach a 0 range, we know we are past all good values
		{
			destination = maps[i][j].destination, source = maps[i][j].source, range = maps[i][j].range;
			if (seed >= source && seed < source + range)
			{
				// seed is within the range of source, so we set it to the corresponding destination
				printf("Map %d: %u mapped to ", i + 1, seed);
				seed = destination + (seed - source);
				printf("%u (source %u)\n", seed, source);
				break;
			}
		}
		// If seed is not within any ranges, it is equal to itself, and it is unchanged so this follows the requirements
		if (seed == prevSeed) printf("Map %d: %u mapped to %u\n", i + 1, prevSeed, seed);
	}

	return seed; // this is now a location val
}
