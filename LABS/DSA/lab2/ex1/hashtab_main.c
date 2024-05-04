#include "hashtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_SEARCHES 100

#include <time.h>

double wtime() {
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1E9;
}

int getRand(int min, int max) {
    return min + rand() % (max - min + 1);
}

int main() {
    srand(time(NULL));

    const char* filename = "small-big.txt";
    const char* fileout = "../ex1res/HashTimeS.txt";

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
            int idx = getRand(0, count - 1);
            double t_start = wtime();
            hashtab_lookup(hashtab, words[idx]);
            double t_end = wtime();
            fprintf(fileres, "%d %.12lf\n", count, (t_end - t_start) / NUM_SEARCHES);
        }
    }
    
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }

    fclose(file);
    return 0;
}
