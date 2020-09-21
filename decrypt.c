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

typedef struct KEY {
    uint8_t k1;
    uint8_t k2;
    uint8_t k3;
} KEY_t;

int FILE_SIZE;

#define MAXWORDLENGTH 24
#define DICTLENGTH 102305
#define DICTIONARYPATH "/usr/share/dict/american-english"
#define _GNU_SOURCE

char DICTIONARY[DICTLENGTH][MAXWORDLENGTH];
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
	idx++;
	strncpy(DICTIONARY[idx], line, strlen(line)-1);
    }
    fclose(fp);
    if (line)
	free(line);
}

char* readFile(char* filename){
    //https://stackoverflow.com/a/174552/5763413
    char * buffer = 0;
    FILE * f = fopen (filename, "rb");

    if (f)
    {
	fseek (f, 0, SEEK_END);
	FILE_SIZE = ftell (f);
	fseek (f, 0, SEEK_SET);
	buffer = malloc (FILE_SIZE);
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
    char* decBuff = malloc(FILE_SIZE);    
    memset(decBuff, '\0', FILE_SIZE);

    for (int i=0;i<FILE_SIZE; i++){
	//printf("Char %c shifted with %c to %c\n", buffer[i], key[i%keyLen], shiftLetter(buffer[i], key[i%keyLen]));
	decBuff[i] = shiftLetter(buffer[i], key[i%keyLen]);
    }
    return decBuff;
}

int checkWordInDictionary(char* word){
    if (!word){
	return -2;
    }
    //char c1=word[0];
    for(int i=0;i<DICTLENGTH;i++){
	if (!strcmp(word,DICTIONARY[i])){
	    return i;
	}	

    }
    return -1;
}

float getPercentInDict(char* DATA, char* key){
    int totalCt = 0;
    int dictCt = 0;
    char* decrypted = (char*) malloc(FILE_SIZE * sizeof(char));
    //decrypted  = decryptStr(DATA, key);
    decrypted = DATA;

    char delim[] = {' ','\0'};
    char* ptr = strtok(decrypted, delim);

    while(ptr != NULL)
    {
	totalCt++;
	//ptr holds current word
	printf("Checking word >%s<\n", ptr);
	if(checkWordInDictionary(ptr) != -1){
	    printf("Word %s found in dictionary\n", ptr);
	    dictCt++;

	}
	ptr = strtok(NULL, delim);
    }



    return (float)dictCt/(float)totalCt;
}

void dumbBruteForce(char* DATA){

    float maxPercent = 0;
    float percent;

    char key[4] = {' ',' ',' '};
    for (int i=MIN_KEY; i<=MAX_KEY; i++){
	for (int j=MIN_KEY; j<=MAX_KEY; j++){
	    for (int k=MIN_KEY; k<=MAX_KEY; k++){
		printf("\nUsing key [%s]\n", key);
		sprintf(key,"%c%c%c", i,j,k);
		percent = getPercentInDict(DATA, key);
		if(percent>maxPercent){
		    printf("KEY: %s\tPercent: %f\n", key, percent);
		}


	    }
	}
    }
}

int main(int argc, char* argv[]){
    initDictionary();
    char* DATA = readFile("text.txt");
	getPercentInDict(DATA,"abb");
    //dumbBruteForce(DATA);
}
