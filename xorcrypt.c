#include "xorcrypt.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Card { int rank; char suit; };

void encryptDeck(Card* cardArray)
{
    char buffer[DECK_SIZE * LINE_SIZE];
    int pos = 0;
    int i;
    for (i = 0; i < DECK_SIZE; i++)
    {
        int written = snprintf(buffer + pos, LINE_SIZE, "%d %c\n",
                               cardArray[i].rank, cardArray[i].suit);
        if (written < 0)
            written = 0;
        if (written >= LINE_SIZE)
            written = LINE_SIZE - 1;
        pos += written;
    }
    {
        const char *key = XOR_KEY;
        size_t keylen = strlen(key);
        size_t j;
        if (keylen != 0)
        {
            for (j = 0; j < (size_t)pos; j++)
            {
                buffer[j] = (char)(buffer[j] ^ key[j % keylen]);
            }
        }
    }
    {
        FILE *f = fopen(ENCRYPTED_FILE, "wb");
        if (!f)
        {
            fprintf(stderr, "Failed to open %s for writing.\n", ENCRYPTED_FILE);
            return;
        }
        if (pos > 0)
            fwrite(buffer, 1, (size_t)pos, f);
        fclose(f);
    }
}

void decryptDeck(Card* cardArray)
{
    char buffer[DECK_SIZE * LINE_SIZE + 1];
    size_t filesize = 0;
    FILE *f = fopen(ENCRYPTED_FILE, "rb");
    if (!f)
    {
        fprintf(stderr, "Failed to open %s for reading.\n", ENCRYPTED_FILE);
        return;
    }
    if (fseek(f, 0, SEEK_END) != 0)
    {
        fprintf(stderr, "Failed to seek in %s.\n", ENCRYPTED_FILE);
        fclose(f);
        return;
    }
    {
        long s = ftell(f);
        if (s < 0)
        {
            fprintf(stderr, "Failed to tell size of %s.\n", ENCRYPTED_FILE);
            fclose(f);
            return;
        }
        filesize = (size_t)s;
    }
    if (fseek(f, 0, SEEK_SET) != 0)
    {
        fprintf(stderr, "Failed to rewind %s.\n", ENCRYPTED_FILE);
        fclose(f);
        return;
    }
    if (filesize > (DECK_SIZE * LINE_SIZE))
    {
        fprintf(stderr, "%s is too large to decrypt (max %d bytes).\n",
                ENCRYPTED_FILE, DECK_SIZE * LINE_SIZE);
        fclose(f);
        return;
    }
    if (filesize > 0)
    {
        size_t read = fread(buffer, 1, filesize, f);
        if (read != filesize)
        {
            fprintf(stderr, "Failed to read %s fully.\n", ENCRYPTED_FILE);
            fclose(f);
            return;
        }
        buffer[filesize] = '\0';
    }
    else
    {
        buffer[0] = '\0';
    }
    fclose(f);
    {
        const char *key = XOR_KEY;
        size_t keylen = strlen(key);
        size_t j;
        if (keylen != 0)
        {
            for (j = 0; j < filesize; j++)
            {
                buffer[j] = (char)(buffer[j] ^ key[j % keylen]);
            }
            buffer[filesize] = '\0';
        }
    }
    {
        int idx = 0;
        char *line = strtok(buffer, "\n");
        while (line != NULL && idx < DECK_SIZE)
        {
            int rank = 0;
            char suit = '\0';
            if (sscanf(line, "%d %c", &rank, &suit) == 2)
            {
                cardArray[idx].rank = rank;
                cardArray[idx].suit = suit;
                idx++;
            }
            line = strtok(NULL, "\n");
        }
    }
}


