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

typedef struct KEY {
    char k1;
    char k2;
    char k3;
    char k4;
} KEY_t;

KEY_t* generateKeys(){

    const unsigned long NUMKEYS = 84934656; //(128-32)^4
    KEY_t* KEY_LIST = (KEY_t*)calloc(NUMKEYS, sizeof(KEY_LIST[0]));
    if (!(KEY_LIST)) return NULL;

#define MIN_KEY 0x20
#define MAX_KEY 0x7E
#include <limits.h>
    KEY_t tempKey;
    
    unsigned long int pos = 0;

    for (unsigned int i=0;i<=0xFFFF;i++){
                    tempKey.k1 = (i&0xF000)>>12;
                    tempKey.k2 = (i&0x0F00)>> 8;
                    tempKey.k3 = (i&0x00F0)>> 4;
                    tempKey.k4 = (i&0x000F)>> 0;
                    printf("%d: %d,%d,%d,%d\n", i, tempKey.k1, tempKey.k2, tempKey.k3, tempKey.k4);
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
    printf("Got here\n");


    //printf("%c,%c,%c,%c\n", tempKey.k1, tempKey.k2, tempKey.k3, tempKey.k4);
}

int main(int argc, char* argv[]){


    dumbBruteForce();

}
