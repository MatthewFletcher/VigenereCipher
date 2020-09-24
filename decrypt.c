#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXFILESIZE 1024
#define MAXKEYSIZE 4 //Size key takes in memory
#define MAXKEYLEN MAXKEYSIZE-1 //Number of useable characters in key

#include <stdint.h>

#include <omp.h>
#define MIN_KEY 0x41
#define MAX_KEY 0x7A
#include <limits.h>



#define MAXWORDLENGTH 24
#define DICTLENGTH 102305
#define DICTIONARYPATH "/usr/share/dict/american-english"
#define _GNU_SOURCE


#include "decrypt.h"

int main(int argc, char* argv[]){
    masterDecrypt("text.txt.vig");
    return 0;
}

void initDictionary(){

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(DICTIONARYPATH, "r");
    if (fp == NULL){
        printf("ERROR opening dictionary\n");
        exit(EXIT_FAILURE);
    }

    int idx = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        strncpy(DICTIONARY[idx], line, strlen(line)-1);
        idx++;
    }
    fclose(fp);
    if (line)
        free(line);
}

char* readFile(const char* filename){
    //https://stackoverflow.com/a/174552/5763413
    char * buffer = 0;
    FILE * f = fopen (filename, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        FILE_SIZE = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = calloc (FILE_SIZE, sizeof(char));
        if (buffer)
        {
            int r = fread (buffer, 1, FILE_SIZE, f);
            if (!(r==FILE_SIZE)){
                printf("ERROR: File read incorrectly.\n");
                return NULL;
            }
            fclose (f);
        }
        //end copy from SO

    }

    else {
        printf("ERROR: File not found or file empty\n");
        return NULL;
    }

    return buffer;
}

void writeFile(char* filename, char* buff){
    FILE * fp = fopen(filename, "wb");
    if (fp){
        fputs(buff, fp);
        fclose(fp);
    }
    else{
        printf("ERROR writing to file %s\n", filename);
    }
}


char shiftLetter(char ct, char shift){
    return (char)(((int)ct-(int)shift+127)%127);
}

char* decryptStr(char* buffer, char* key){
    int keyLen = strlen(key);
    char* decBuff = calloc(FILE_SIZE, sizeof(char));    
    //memset(decBuff, '\0', FILE_SIZE);

    for (int i=0;i<FILE_SIZE; i++){
        //printf("Char %c shifted with %c to %c\n", buffer[i], key[i%keyLen], shiftLetter(buffer[i], key[i%keyLen]));
        decBuff[i] = shiftLetter(buffer[i], key[i%keyLen]);
    }
    //printf("Encrypted: |%s|\n", buffer);
    //printf("Decrypted: |%s|\n", decBuff);

    return decBuff;
}

int checkWordInDictionary(char* word, int start, int end){
    //
    //If word is null
    if (!word){
        return -2;
    }

    if (strlen(word) > MAXWORDLENGTH) return -1;
    int mid = (start+end)/2;
    //printf("Comparing from %s to %s\n", DICTIONARY[start], DICTIONARY[mid]);
    //If word is not null, check in dictionary
    //printf("Comparing %s to %s\n", word, DICTIONARY[mid]);
    int r = strcmp(word, DICTIONARY[mid]);
    if (r==0){
        //printf("Comparing %s to %s\n", word, DICTIONARY[mid]);
        return mid;
    }
    //If word not found, abort
    if (end-start<2) return -1;

    if (r<0) return checkWordInDictionary(word, start,mid);
    else return checkWordInDictionary(word, mid,end);
    //if word not found in dictionary, return -1
}

double getPercentInDict(char* data, char* key){
    int totalCt = 0;
    int dictCt = 0;
    char* decrypted = (char*) calloc(FILE_SIZE, sizeof(char));
    decrypted  = decryptStr(data, key);
#ifdef DEBUG
    decrypted = data;
#endif
    //printf("Decrypted text: |%s|\n", decrypted);
    //printf("Decrypted text length: |%lu|\n", strlen(decrypted));

    char delim[] = {' ','\0'};
    char* ptr = strtok(decrypted, delim);

    int idx = 0;

    while((ptr != NULL) || (idx++ < FILE_SIZE))
    {

        if (!ptr) break;
        totalCt++;
        //ptr holds current word
        //printf("Checking word >%s<\n", ptr);
        switch (checkWordInDictionary(ptr, 0,DICTLENGTH-1)){
            case -2:
                //printf("NULL string passed to function\n");
                goto ENDLOOP;
                break;
            case -1:
                //printf("Word %s not found in dictionary\n", ptr);
                break;
            default:
                //printf("Word %s found in dictionary\n", ptr);
                dictCt++;
                break;
        }
        ptr = strtok(NULL, delim);
    }
    //Please forgive me for using a goto, but it was the cleanest way
ENDLOOP: ; 
         //SEMICOLON MUST STAY IN HERE
         //https://stackoverflow.com/questions/18496282
         //

         if (abs(strlen(data) - FILE_SIZE)>3) return 0;

         double percent =  (double)dictCt/(double)totalCt;
         //if (percent) printf("%f%% of words found in dictionary\n",100*percent);

         return percent;
}


void printNode(KeyResult_t* node){
        printf("Key %s\t Score %3.2f\tMessage:|%s|\n", node->key, node->percent, node->decrypted);
}

KeyResult_t** dumbBruteForce(char* DATA){

    int listSize = 10;
    KeyResult_t** bestList = malloc(listSize * sizeof(KeyResult_t*));
    int idx = 0;

    double percent;
    char key[4] = {' ',' ',' '};



    for (int i=MIN_KEY; i<=MAX_KEY; i++){
        for (int j=MIN_KEY; j<=MAX_KEY; j++){
            for (int k=MIN_KEY; k<=MAX_KEY; k++){
                // printf("\nUsing key [%s]\n", key);
                sprintf(key,"%c%c%c", i,j,k);
                percent = getPercentInDict(DATA, key);
                    //printf("KEY: %s\tPercent: %f\n", key, percent);
                const double THRESHOLD = 0.6;
                if(percent > THRESHOLD){
                    KeyResult_t* toAdd = (KeyResult_t*) malloc(sizeof(KeyResult_t));
                    if (toAdd == NULL) printf("ERROR allocating for toAdd\n");


                    //Add decrypted value
                    toAdd->decrypted = (char*) calloc(strlen(DATA), sizeof(char));
                    if (toAdd->decrypted== NULL) printf("ERROR allocating for toAdd->decrypted\n");
                    char* tmp = strcpy(toAdd->decrypted, decryptStr(DATA,key));

                    //printf("length diff for %s: %d\n", key, abs(strlen(tmp)-FILE_SIZE));
                    if(abs(strlen(tmp) - FILE_SIZE)>3){
                        continue;
                    }


                    //Set percentage
                    toAdd->percent = percent;
                    //Set key
                    strcpy(toAdd->key, key);
                    printNode(toAdd);
                    bestList[idx++] = toAdd;
                }
            }
        }
    }


    //Returning a value in case I want to play around with this more in the future
    return bestList;
}

void masterDecrypt(const char* filename){
    initDictionary();
    char* DATA = readFile(filename);
    KeyResult_t** results = dumbBruteForce(DATA);
    if (results[0] == NULL){
        printf("No decryption keys found\n");
    }

}
