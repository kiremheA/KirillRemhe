#include "hashtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern unsigned int collisions; 

int main() {
    srand(time(NULL));

    const char* filename = "words.txt";
    const char* fileout = "../ex3res/HashCollisionsKP.txt";

    FILE *fileres = fopen(fileout, "w");
    FILE *file = fopen(filename, "r");
    
    if (!file) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    char word[256];
    int count = 0;
    char* words[200000];
    struct listnode *hashtab[HASHTAB_SIZE];
    hashtab_init(hashtab);

    while (fscanf(file, "%s", word) != EOF && count < 200000) {
        words[count] = strdup(word);
        hashtab_add(hashtab, words[count], count);
        count++;

        if (count % 10000 == 0) {
            fprintf(fileres, "%d %u\n", count, collisions);
            collisions = 0;
        }
    }

    for (int i = 0; i < count; i++) {
        free(words[i]);
    }

    fclose(file);
    fclose(fileres);
    return 0;
}
