#include <stdio.h>
#include <stdlib.h>
#include "xorcrypt.h"

#define NUM_PLAYERS 4
#define HAND_SIZE 13

int main()
{
    Card deck[DECK_SIZE];
    FILE *ef = fopen(ENCRYPTED_FILE, "rb");
    if (ef)
    {
        fclose(ef);
        decryptDeck(deck);
    }
    else
    {
        FILE *f = fopen("deck.txt", "r");
        if (!f)
        {
            printf("Failed to open the file.\n");
            return 1;
        }
        for (int i = 0; i < DECK_SIZE; i++)
        {
            if (fscanf(f, "%d %c", &deck[i].rank, &deck[i].suit) != 2)
            {
                printf("Error reading cards from the file.\n");
                fclose(f);
                return 1;
            }
        }
        fclose(f);
        encryptDeck(deck);
    }

    Card hands[NUM_PLAYERS][HAND_SIZE];
    int handCount[NUM_PLAYERS] = {0};
    int rankCount[NUM_PLAYERS][14] = {{0}};

    int winner = -1;
    int idx = 0;

    while (winner == -1 && idx < DECK_SIZE)
    {
        for (int p = 0; p < NUM_PLAYERS && winner == -1; p++)
        {
            if (idx >= DECK_SIZE)
                break;

            Card c = deck[idx++];
            hands[p][handCount[p]++] = c;
            rankCount[p][c.rank]++;

            if (rankCount[p][c.rank] == 4)
            {
                winner = p;
            }
        }
    }

    if (winner == -1)
    {
        printf("No player received 4 identical cards (the game ended).\n");
        return 0;
    }

    printf("Winning player: Player %d\n", winner + 1);
    printf("Cards of the winning player:\n");
    for (int i = 0; i < handCount[winner]; i++)
    {
        printf("%d %c\n", hands[winner][i].rank, hands[winner][i].suit);
    }

    return 0;
}