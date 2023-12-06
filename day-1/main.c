#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int modifiedStrcmp(const char* str1, const char* str2, int length);
int searchSubstrings(char line[BUFSIZ], int startIndex);
void searchForNums(char buf[BUFSIZ], int* firstDigit, int* lastDigit);

// Only compares the first [length] characters of str1.
int modifiedStrcmp(const char* str1, const char* str2, int length)
{
	char copy[BUFSIZ] = { '\0' };
	strncpy(copy, str1, length); // makes it so str1 is the same length as str2
	copy[length] = '\0'; // need to null-terminate
	return strcmp(copy, str2);
}


int searchSubstrings(char line[BUFSIZ], int startIndex)
{
	// Longest length of a spelled out number is 5
	// There are no uppercase letters in the input file
	// First is left blank for nicer indexing
	char names[10][6] = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
	for (int i = 1; i <= 9; i++)
	{
		if (modifiedStrcmp(line + startIndex * sizeof(char), names[i], strlen(names[i])) == 0) // equal strings
		{
			return i; // returns the number that is in word form
		}
	}
	return 0; // nothing
}

void searchForNums(char buf[BUFSIZ], int* firstDigit, int* lastDigit)
{
	char curChar = -1, curDigit = 0;
	for (int i = 0; curChar != '\0' && curChar != '\n'; i++)
	{
		curChar = buf[i];
		// Find the value of the current character. If it is a digit, convert to int, otherwise, check if it is the beginning of a word.
		if (isdigit(curChar))
		{
			curDigit = curChar - '0';
		}
		else
		{
			curDigit = searchSubstrings(buf, i);
		}

		// Now assign the value of this to either firstDigit, if there have been no digits yet (i.e. firstDigit == -1),
		// or lastDigit if there have been digits. We can only assign it to either of these as long as it is not 0, as this is what curDigit
		// will equal if it is not a digit (there are no zeros in the data) and not the start of a word (searchSubstrings returns 0 if no matches).
		if (*firstDigit == -1 && curDigit != 0)
		{
			*firstDigit = curDigit;
		}
		else if (curDigit != 0)
		{
			*lastDigit = curDigit;
		}
	}

	if (*lastDigit == -1) // there was only 1 number, so lastDigit has not been changed yet and we must assign it to the value of firstDigit
	{
		*lastDigit = *firstDigit;
	}
}

int main(void)
{
	int sum = 0;
	FILE* fp = fopen("input.txt", "r");
	char buf[BUFSIZ];
	char curChar = -1;
	int firstDigit = -1, lastDigit = -1;

	for(int line = 0; !feof(fp); line++)
	{
		fgets(buf, BUFSIZ, fp);
		//printf("line %d: %s\n", line, buf);
		searchForNums(buf, &firstDigit, &lastDigit);
		//printf("first: %d, last: %d\n", firstDigit, lastDigit);
		sum += (firstDigit * 10) + lastDigit;

		firstDigit = -1, lastDigit = -1;
		//system("pause");
	}

	printf("Sum: %d\n", sum);
	return 0;
}
