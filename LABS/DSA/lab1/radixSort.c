#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

#define MAX 1000000
#define RANGE 100001 

double wtime() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + (double)t.tv_usec / 1000000;
}

uint32_t getMax(uint32_t arr[], int n) {
    uint32_t mx = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

void countSort(uint32_t arr[], int n, int exp) {
    uint32_t output[MAX];
    int i, count[10] = {0};

    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

void radixsort(uint32_t arr[], int n) {
    uint32_t m = getMax(arr, n);

    for (uint32_t exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

int main() {
    FILE *outputFile;
    uint32_t arr[MAX];
    double time_s = 0.0;
    double sum_time = 0.0;


    outputFile = fopen("outputRadixSort.txt", "a");
    if (outputFile == NULL) {
	printf("Не удалось открыть файл для записи.\n");
	return 1;
    }

    for (int i = 0; i < 10; i++){

        for(int j = 0; j < MAX; j++)
            arr[j] = rand() % RANGE;

        double startTime = wtime();

        radixsort(arr, MAX);

        double endTime = wtime();

        time_s = endTime - startTime;

        sum_time += time_s;

        printf("Время выполнения сортировки: %.6lf секунд\n", time_s);
    }

    printf("Суммарное время сортировки: %.6lf секунд\n", sum_time/10);
    printf("Среднее время сортировки: %.6lf секунд\n", sum_time/10);
    
    fprintf(outputFile, "%d %.6lf\n", MAX, sum_time/10);
    
    fclose(outputFile);
    return 0;
}
