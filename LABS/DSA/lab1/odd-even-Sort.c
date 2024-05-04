#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

#define MAX 950000
#define RANGE 100001

double wtime() {
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec + (double)t.tv_usec / 1000000;
}

void swap(uint32_t *a, uint32_t *b) {
	uint32_t temp = *a;
	*a = *b;
	*b = temp;
}

void oddEvenSort(uint32_t arr[], int n) {
	bool isSorted = false;

	while (!isSorted) {
		isSorted = true;

		for (int i = 1; i <= n - 2; i += 2) {
			if (arr[i] > arr[i + 1]) {
				swap(&arr[i], &arr[i + 1]);
				isSorted = false;
			}
		}

		for (int i = 0; i <= n - 2; i += 2) {
			if (arr[i] > arr[i + 1]) {
				swap(&arr[i], &arr[i + 1]);
				isSorted = false;
			}
		}
	}
}

int main() {
	FILE *outputFile;
	uint32_t arr[MAX];
	double time_s = 0.0;

	outputFile = fopen("outputOddEvenSort.txt", "a");
	if (outputFile == NULL) {
		printf("Не удалось открыть файл для записи.\n");
		return 1;
	}
	
	for (int j = 0; j < MAX; j++) {
		arr[j] = rand() % RANGE;
	}

	double start_time = wtime();

	oddEvenSort(arr, MAX);

	double end_time = wtime();

	time_s = end_time - start_time;

	printf("Время выполнения сортировки: %.6lf секунд\n", time_s);
	fprintf(outputFile, "%d %.6lf\n", MAX, time_s);
	fclose(outputFile); 
	
	return 0;
}
