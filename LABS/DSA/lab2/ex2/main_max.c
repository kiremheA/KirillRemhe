// main.c
#include "bstree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define MAX_WORD_LENGTH 100
#define SEARCH_ITERATIONS 1000// Увеличьте число для большей точности

// Вспомогательная функция для генерации случайного числа
int getRand(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Функция для измерения процессорного времени
double wtime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec / 1E6;
}

// Функция для замера времени нахождения максимального элемента в дереве
// Функция для замера времени нахождения максимального элемента в дереве
double measure_average_max_time(struct bstree *tree, int iterations, int max_searches) {
    double total_time = 0.0;
    for (int i = 0; i < iterations; ++i) {
        double t = wtime();
        for (int j = 0; j < max_searches; ++j) {
            bstree_max(tree); // Выполняем поиск максимального элемента несколько раз
        }
        total_time += wtime() - t;
    }
    return total_time / (double)iterations;
}

int main() {
    srand((unsigned int)time(NULL));
    const int N_WORDS = 200000;
    char *words[N_WORDS];
    char word_buffer[MAX_WORD_LENGTH];
    
    const char *filename = "words.txt";
    //const char *fileout = "../BStreeTimeMAXPOS.txt";
    const char *fileout = "../ex2res/BStreeTimeMAXPOS.txt";
    
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
    
    const int MAX_SEARCHES = 1; // Указываем, сколько раз выполнять поиск в каждой итерации
    
    for (int i = 1; i < index; ++i) {
        bstree_add(tree, words[i], i);
        
        if ((i + 1) % 10000 == 0 || i == index - 1) {
            double avg_time = measure_average_max_time(tree, SEARCH_ITERATIONS, MAX_SEARCHES);
            printf("%d %.9f\n", i + 1, avg_time);
            fprintf(outputfile, "%d %.9f\n", i + 1, avg_time);
        }
    }
    
    for (int i = 0; i < N_WORDS; ++i) {
        free(words[i]);
    }
    
    // Освобождение памяти для бинарного дерева
    //bstree_free(tree);
    
    fclose(outputfile);
    
    return 0;
}
