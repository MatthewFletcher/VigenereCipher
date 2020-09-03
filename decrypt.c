#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXFILESIZE 1024
#define MAXKEYSIZE 5 //Size key takes in memory
#define MAXKEYLEN MAXKEYSIZE-1 //Number of useable characters in key


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
        }
        fclose (f);
    }
    //end copy from SO

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

#include <stdint.h>
typedef struct KEY {
    uint8_t k1;
    uint8_t k2;
    uint8_t k3;
    uint8_t k4;
} KEY_t;

#include <omp.h>
#define MIN_KEY 0x20
#define MAX_KEY 0x7E
#include <limits.h>
const unsigned long NUMKEYS = 84934656; //(128-32)^4
KEY_t* generateKeys(){

    KEY_t* KEY_LIST = (KEY_t*)malloc(NUMKEYS*sizeof(KEY_t*));
    if (!(KEY_LIST)) return NULL;

    KEY_t tempKey;

    unsigned long int pos = 0;

#pragma omp simd simdlen(8)
    for (uint64_t i=0x20202020;i<=0xFFFFFFFF;i++){
        tempKey.k1 = ((i&0x5F000000)>> 24) + MIN_KEY; 
        tempKey.k2 = ((i&0x005F0000)>> 16 ) + MIN_KEY;
        tempKey.k3 = ((i&0x00005F00)>>  8 ) + MIN_KEY;
        tempKey.k4 = ((i&0x0000005F)>>  0 ) + MIN_KEY;
        if(!(i%200000000))  printf("%lu: %d,%d,%d,%d\n", i, tempKey.k1, tempKey.k2, tempKey.k3, tempKey.k4);
        //getchar();
        KEY_LIST[pos] = tempKey;
    }


    return KEY_LIST;

}

void dumbBruteForce(){

    KEY_t* keyList = generateKeys();
    if (!(keyList)){
        printf("ERROR: malloc returned NULL\n");
        return;
    }

    KEY_t tempKey = keyList[420000];

        printf("%d,%d,%d,%d\n",  tempKey.k1, tempKey.k2, tempKey.k3, tempKey.k4);


    //printf("%c,%c,%c,%c\n", tempKey.k1, tempKey.k2, tempKey.k3, tempKey.k4);
}

int main(int argc, char* argv[]){


    dumbBruteForce();

}
