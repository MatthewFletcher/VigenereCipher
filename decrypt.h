#ifndef DECRYPT_H
#define DECRYPT_H
int FILE_SIZE;
#define MAXWORDLENGTH 24
#define DICTLENGTH 102305
char DICTIONARY[DICTLENGTH][MAXWORDLENGTH];
typedef struct KeyResult
{
    char key[4];
    double percent;
    char* decrypted;
} KeyResult_t;
void initDictionary();
char* readFile(const char* filename);
void writeFile(char* filename, char* buff);
char shiftLetter(char ct, char shift);
char* decryptStr(char* buffer, char* key);
int checkWordInDictionary(char* word, int start, int end);
double getPercentInDict(char* data, char* key);
void printNode(KeyResult_t* node);
KeyResult_t** dumbBruteForce(char* DATA);
void masterDecrypt(const char* filename);
#endif
