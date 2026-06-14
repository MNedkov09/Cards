 #ifndef XORCRYPT_H
 #define XORCRYPT_H

#define DECK_SIZE 52
#define XOR_KEY "your_secret_key"
#define ENCRYPTED_FILE "encryptedDeck.txt"
#define LINE_SIZE 16

typedef struct
{
    int rank;
    char suit;
} Card;

void encryptDeck(Card* cardArray);
void decryptDeck(Card* cardArray);

#endif
