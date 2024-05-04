// main.c
#include "bstree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define MAX_WORD_LENGTH 100
#define SEARCH_ITERATIONS 300

int getRand(int min, int max) {
    return min + rand() % (max - min + 1);
}

double wtime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec / 1E6;
}

double measure_average_lookup_time(struct bstree *tree, char *words[], int count, int iterations) {
    double total_time = 0.0;
    for (int i = 0; i < iterations; ++i) {
        char *w = words[getRand(0, count - 1)];
        double t = wtime();
        bstree_lookup(tree, w);
        total_time += wtime() - t;
    }
    return total_time / (double)iterations;
}

int main() {
    srand((unsigned int)time(NULL));
    const int N_WORDS = 200000;
    char *words[N_WORDS];
    char word_buffer[MAX_WORD_LENGTH];
    
    const char *filename = "small-big.txt";
    const char *fileout = "../ex1res/BStreeTimeS2.txt";
    
    FILE *file = fopen(filename, "r");
    FILE *outputfile = fopen(fileout, "w");
    
    if (!file) {
        perror("Unable to open file");
        return 1;
    }
    
    int index = 0;
    while (index < N_WORDS && fscanf(file, "%99s", word_buffer) == 1) {
        words[index] = strdup(word_buffer);
        
        if (!words[index]) {
            fprintf(stderr, "Memory allocation error\n");
            
            for (int i = 0; i < index; ++i) {
                free(words[i]);
            }
            fclose(file);
            return 1;
        }
        index++;
    }
    fclose(file);
    
    if (index < N_WORDS) {
        fprintf(stderr, "Not enough words in file.\n");
        
        for (int i = 0; i < index; ++i) {
            free(words[i]);
        }
        return 1;
    }
    
    struct bstree *tree = bstree_create(words[0], 0);
    
    if (!tree) {
        fprintf(stderr, "Memory allocation error for root\n");
        
        for (int i = 0; i < N_WORDS; ++i) {
            free(words[i]);
        }
        return 1;
    }
    
    for (int i = 2; i <= index; ++i) {
        bstree_add(tree, words[i - 1], i - 1);
        
        if (i % 10000 == 0) {
            double avg_time = measure_average_lookup_time(tree, words, i - 1, SEARCH_ITERATIONS);
            printf("%d %.9f\n", i, avg_time);
            fprintf(outputfile, "%d %.9f\n", i, avg_time);
        }
    }
    
    for (int i = 0; i < N_WORDS; ++i) {
        free(words[i]);
    }
    
    
    fclose(outputfile);
    
    return 0;
}
