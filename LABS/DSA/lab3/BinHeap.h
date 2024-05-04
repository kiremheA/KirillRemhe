#include <stdio.h>

typedef struct heapnode {
  int key;   
  int value; 
} heapnode;

typedef struct Heap {
  int *index;
  int maxsize;     
  int nnodes;      
  heapnode *nodes; 

} heap;

heap *heap_create(int maxsize);
void heap_free(heap *h);
int heap_is_empty(heap *h);
void heap_swap(heapnode *a, heapnode *b, heap *h);
int heap_insert(heap *h, int key, int value);
int heap_decrease_key(heap *h, int index, int newkey);
heapnode heap_extract_min(heap *h);
void heapify_up(heap *h, int index);
void heapify_down(heap *h, int index);
heap *build_min_heap(int *arr, int n);