#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXFILESIZE 1024
#define MAXKEYSIZE 5 //Size key takes in memory
#define MAXKEYLEN MAXKEYSIZE-1 //Number of useable characters in key

#include <stdint.h>

#include <omp.h>
#define MIN_KEY 0x20
#define MAX_KEY 0x7E
#include <limits.h>

typedef struct KEY {
    uint8_t k1;
    uint8_t k2;
    uint8_t k3;
} KEY_t;

int FILE_SIZE;



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
        printf("Char %c shifted with %c to %c\n", buffer[i], key[i%keyLen], shiftLetter(buffer[i], key[i%keyLen]));
        decBuff[i] = shiftLetter(buffer[i], key[i%keyLen]);
    }


    return decBuff;

}

const unsigned long NUMKEYS = 99999; //(128-32)^3
KEY_t* generateKeys(){

    printf("Size of KEY_t: %lu \n", sizeof(KEY_t));
    printf("Allocating %lu bytes\n", NUMKEYS*sizeof(KEY_t));
    //KEY_t* KEY_LIST = (KEY_t*)malloc(NUMKEYS*sizeof(KEY_t));
    KEY_t* KEY_LIST = (KEY_t*)malloc(99999999);
    if (!(KEY_LIST)) return NULL;

    KEY_t tempKey;


    unsigned long int startPos = 0x202020;

#pragma omp simd simdlen(8)
    for (uint64_t i=startPos;i<=0xFFFFFF;i++){
        tempKey.k1 = ((i&0x5F0000)>> 16 ) + MIN_KEY;
        tempKey.k2 = ((i&0x005F00)>>  8 ) + MIN_KEY;
        tempKey.k3 = ((i&0x00005F)>>  0 ) + MIN_KEY;
        if(!(i%100))  printf("%lu: %d,%d,%d\n", i-startPos, tempKey.k1, tempKey.k2, tempKey.k3);
        //getchar();
        KEY_LIST[i-startPos] = tempKey;
    }


    return KEY_LIST;

}

void dumbBruteForce(){

    KEY_t* keyList = generateKeys();
    if (!(keyList)){
        printf("ERROR: malloc returned NULL\n");
        return;
    }

    for (int i=0; i<50; i++){
        KEY_t tempKey = keyList[i];
        printf("%d: %d,%d,%d\n", i, tempKey.k1, tempKey.k2, tempKey.k3);

    }



    //printf("%c,%c,%c,%c\n", tempKey.k1, tempKey.k2, tempKey.k3, tempKey.k4);
}

int main(int argc, char* argv[]){


    dumbBruteForce();

}
