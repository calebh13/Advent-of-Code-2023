#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define NUM_HANDS 1000 // 1000 for input.txt
#define INPUT_FILE "input.txt"

// note: this is represented lowest to highest, so that a higher index in this array means a higher card value
// slight change from part 1, J is now lowest card
const char* global_CardTypes = "J23456789TQKA";

typedef struct
{
	int bid;
	int handValue;
	// Explanation for handValue: we need to represent a hand fully numerically so that way we can easily sort the cards and thus determine rank
	// Because there are 13 hand types, we will just use hexadecimal to make things more understandable
	// So we need to represent 6 total pieces of information: the hand type (0-6, where 0 is high card and 6 is five of a kind),
	// and then the 5 cards in ORDER. so, moving right to left in handValue, the 5th card will be multiplied by 16^0 (1), the 4th card will be multiplied by 16^1,
	// the 3rd card will be multiplied by 16^2, 2nd by 16^3, 1st by 16^4, and hand type by 16^5. Therefore, we can simply sort 2 handTypes as integers.
	char cards[5];
} Hand;

void readHands(Hand hands[NUM_HANDS]);
void calculateHandValues(Hand hands[NUM_HANDS]);
int calculateHandType(char cards[5]);
int calculateCardNumber(char card);
int numOfAKind(int cardCounts[13], int ofAKind, int* forbiddenNum, int* jokerUsed);
void calculateRanks(Hand hands[NUM_HANDS]);
int getTotalWinnings(Hand hands[NUM_HANDS]);

int main(void)
{
	Hand hands[NUM_HANDS] = {0};
	readHands(hands);
	calculateHandValues(hands);
	calculateRanks(hands);
	printf("Total winnings: %d\n", getTotalWinnings(hands));

	return 0;
}

void readHands(Hand hands[NUM_HANDS])
{
	FILE* fp = fopen(INPUT_FILE, "r");
	char buf[BUFSIZ] = { '\0' };
	for (int i = 0; !feof(fp); i++)
	{
		fgets(buf, BUFSIZ, fp);
		int colIndex = 0;
		for (; colIndex < 5; colIndex++)
		{
			hands[i].cards[colIndex] = buf[colIndex];
		}
		for (; isdigit(buf[colIndex]); colIndex++); // advance to next number
		hands[i].bid = atoi(buf + colIndex * sizeof(char));
	}
}

void calculateHandValues(Hand hands[NUM_HANDS])
{
	// first must calculate hand type (e.g. full house), then we add cards
	// make the array sorted as we calculate the values 
	int handValue = 0, cardValue = 0, j = 0;
	for (int i = 0; i < NUM_HANDS; i++)
	{
		handValue = calculateHandType(hands[i].cards); // hand rank (full house, 4 of a kind, etc.)
		printf("Hand %d type: %d\n", i, handValue);
		for (int card = 0; card < 5; card++)
		{
			handValue <<= 4; // bitshift by 4 to effectively multiply by 16, and make it so each hex digit is 'concatenated'
			handValue |= calculateCardNumber((hands[i].cards[card])); // add the next card
		}
		printf("Hand value: %x\n", handValue);
		hands[i].handValue = handValue;
	}
}


int calculateHandType(char cards[5])
{
	int cardCounts[13] = { 0 };
	int cardIndex = 0;
	// Count cards:
	for (int i = 0; i < 5; i++)
	{
		cardIndex = calculateCardNumber(cards[i]);
		cardCounts[cardIndex]++;
	}
	// Now, we can use the cardCounts array to much more efficiently determine the hand type through the usage of numOfAKind function, which makes
	// it so we do not have to search the array each time to count

	int forbiddenNum = -1, jokerUsed = 0;
	if (numOfAKind(cardCounts, 5, &forbiddenNum, &jokerUsed)) // If 5 of a kind (handType 6)
	{
		// 5 of a kind
		return 6;
	}
	else if (numOfAKind(cardCounts, 4, &forbiddenNum, &jokerUsed)) // If 4 of a kind (handType 5)
	{
		// 4 of a kind
		return 5;
	}
	else if (numOfAKind(cardCounts, 3, &forbiddenNum, &jokerUsed) && numOfAKind(cardCounts, 2, &forbiddenNum, &jokerUsed)) // If full house (handType 4)
	{
		// Full house
		return 4;
	}
	else if (forbiddenNum != -1) // The only way this can happen is if the first part of the previous statement was true, so there were 3 of a kind but not an additional 2 of a kind
	{
		// 3 of a kind
		return 3;
	}
	else if (numOfAKind(cardCounts, 2, &forbiddenNum, &jokerUsed) && numOfAKind(cardCounts, 2, &forbiddenNum, &jokerUsed)) // Two pairs
	{
		// 2 pairs
		return 2;
	}
	else if (forbiddenNum != -1) // The only way this can happen is if there was at least 1 pair
	{
		// 1 pair
		return 1;
	}
	else
	{
		// High card
		return 0; // It's just a high card, nothing special in it
	}
}


int calculateCardNumber(char card)
{
	// strchr returns a pointer to where 'card' is found in global_CardTypes, and so we just need to subtract the pointer to the start of the array (global_CardTypes)
	// from the strchr result and this will give us the index. it's like a manual indexOf in other languages
	return (int)((strchr(global_CardTypes, card)) - (global_CardTypes)); 
	// note: int casting is just so the compiler doesn't complain about int64 vs int. because global_CardTypes is only a 14 byte array,
	// the difference in indices will obviously never be greater, or even closer to, the int limit
}


// Returns 1 if there at least ofAKind counts of any card, 0 otherwise. Card cannot == *forbiddenNum
// If successful, sets *forbiddenNum to the card 
int numOfAKind(int cardCounts[13], int ofAKind, int* forbiddenNum, int* jokerUsed)
{
	// i starts at 1 now, because 0 would be joker which wouldn't really make sense
	for (int i = 1; i < 13; i++)
	{
		// cardCounts[0] == number of jokers
		if ((cardCounts[i] + (cardCounts[0] * !(*jokerUsed)) >= ofAKind) && i != *forbiddenNum) // We can add joker to calculations if it has not yet been used
		{
			*forbiddenNum = i;
			*jokerUsed = (cardCounts[i] < ofAKind); // If cardCounts would not reach ofAKind without the joker, we must set *jokerUsed to true
			return 1;
		}
	}
	return 0;
}

void calculateRanks(Hand hands[NUM_HANDS])
{
	Hand curHand = { 0 };
	// Insertion sort
	for (int i = 0; i < NUM_HANDS; i++) // For each hand
	{
		/* If the current hand is less than the hand to the left, we need to shift everything to the right and then "insert" the hand at the first
		 * spot where it's greater than the hand to the left. */
		for (int j = i; j > 0 && hands[j].handValue < hands[j - 1].handValue; j--)
		{
			// Swap the two elements
			curHand = hands[j];
			hands[j] = hands[j - 1];
			hands[j - 1] = curHand;
		}
	}
}

int getTotalWinnings(Hand hands[NUM_HANDS])
{
	// Array is now sorted, so rank = index + 1
	int winnings = 0, temp = 0;
	for (int i = 0; i < NUM_HANDS; i++)
	{
		temp = (hands[i].bid * (i + 1));
		printf("Card %d winnings: %d\n", i, temp);
		winnings += temp;
	}
	return winnings;
}
