#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXFILESIZE 1024



char* readFile(char* filename){

    //https://stackoverflow.com/a/174552/5763413
    char * buffer = 0;
    long length;
    FILE * f = fopen (filename, "rb");

    if (f)
    {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
        {
            fread (buffer, 1, length, f);
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
    char* decBuff = malloc(strlen(buffer));    
    memset(decBuff, '\0', strlen(buffer));

    for (int i=0;i<strlen(buffer); i++){
      printf("Char %c shifted with %c to %c\n", buffer[i], key[i%keyLen], shiftLetter(buffer[i], key[i%keyLen]));
        decBuff[i] = shiftLetter(buffer[i], key[i%keyLen]);
    }


    return decBuff;

}

int main(int argc, char* argv[]){



    if (argc != 2){
        printf("No argument supplied\n");
        return 1;
    }

    char key[1024];
    printf("Enter key: ");
    fgets(key,1024, stdin);
    char *pos;
    if ((pos=strchr(key, '\n')) != NULL)
            *pos = '\0';

    char inFile[256];
    strcpy(inFile, argv[1]);

    char* data = readFile(inFile);

    char* decrypted = decryptStr(data,key);

    char outFile[256] = {'\0'};
    strcpy(outFile, inFile);
    strcat(outFile, ".clear");

    writeFile(outFile, decrypted);
}
