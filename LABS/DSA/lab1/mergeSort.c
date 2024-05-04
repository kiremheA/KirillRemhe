#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

#define MAX 50000
#define RANGE 100001

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + (double)t.tv_usec / 1000000;
}

void merge(uint32_t arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    uint32_t L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++; k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++; k++;
    }
}

void mergeSort(uint32_t arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main() {
    FILE *outputFile;
    uint32_t arr[MAX];
    double time_s = 0.0;
    double sum_time = 0.0;

    outputFile = fopen("outputMerge.txt", "a");
    if (outputFile == NULL) {
        printf("Не удалось открыть файл для записи.\n");
        return 1;
    }
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < MAX; j++) {
            arr[j] = rand() % RANGE;
        }

        double start_time = wtime();

        mergeSort(arr, 0, MAX - 1);

        double end_time = wtime();

        time_s = end_time - start_time;

        sum_time += time_s;

        printf("Время выполнения сортировки: %.6lf \n", time_s);
    }

    printf("Среднее время выполнения сортировки: %.6lf секунд\n", sum_time/10);   
    fprintf(outputFile, "%d %.6lf\n", MAX, sum_time/10);
    fclose(outputFile);

    return 0;
}
