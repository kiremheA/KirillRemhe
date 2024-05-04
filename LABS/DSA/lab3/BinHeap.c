#include "BinHeap.h"
#include "Graph.h"

heap *heap_create(int maxsize) {
  heap *h;
  h = malloc(sizeof(*h));
  if (h != NULL) {
    h->maxsize = maxsize;
    h->nnodes = 0;
    h->index = (int *)malloc(sizeof(int) * (maxsize + 1));
    h->nodes = malloc(sizeof(*h->nodes) *
                      (maxsize + 1)); 
    if (h->nodes == NULL) {
      free(h);
      return NULL;
    }
  }
  return h;
}

void heap_free(heap *h) {
  free(h->nodes);
  free(h);
}

void heap_swap(heapnode *a, heapnode *b, heap *h) {
  heapnode temp = *a;

  temp = *a;
  *a = *b;
  *b = temp;

  int tmp;

  tmp = h->index[a->value];
  h->index[a->value] = h->index[b->value];
  h->index[b->value] = tmp;
}

int heap_is_empty(heap *h) {
  int len = sizeof(*(h->nodes));
  if (!len) {
    return 1;
  }
  return 0;
}

int heap_insert(heap *h, int key, int value) {
  if (h->nnodes >= h->maxsize) {
    return -1;
  }

  h->nnodes++;
  h->nodes[h->nnodes].key = key;
  h->nodes[h->nnodes].value = value;
  h->index[value] = h->nnodes;

  int i;

  heapify_up(h, h->nnodes);

  return 0;
}

heapnode heap_extract_min(heap *h) { 
  if (h->nnodes == 0)
    return (heapnode){0, 0};
  heapnode minnode = h->nodes[1];
  h->nodes[1] = h->nodes[h->nnodes--];
  heapify_down(h, 1);
  return minnode;
}

void heapify_up(heap *h, int index) {
  while (index > 1 && h->nodes[index].key < h->nodes[index / 2].key) {
    heap_swap(&h->nodes[index], &h->nodes[index / 2], h);
    index = index / 2;
  }
}

int heap_decrease_key(heap *h, int index, int newkey) {
  index = h->index[index];
  if (h->nodes[index].key < newkey)
    return -1;

  h->nodes[index].key = newkey;
  while (index > 1 && h->nodes[index].key < h->nodes[index / 2].key) {
    heap_swap(&h->nodes[index], &h->nodes[index / 2], h);
    index /= 2;
  }
  return index;
}

void heapify_down(heap *h, int index) {
  while (1) {
    int left = 2 * index;
    int right = 2 * index + 1;
    int least = index;
    if (left <= h->nnodes && h->nodes[left].key < h->nodes[least].key)
      least = left;
    if (right <= h->nnodes && h->nodes[right].key < h->nodes[least].key)
      least = right;
    if (least == index)
      break;
    heap_swap(&h->nodes[index], &h->nodes[least], h);
    index = least;
  }
}

heap *build_min_heap(int *arr, int n) {
  heap *h = heap_create(n);
  for (int i = 0; i < n; i++) {
    h->nodes[i].key = arr[i];
  }
  int index = n / 2;
  for (index; index >= 0; index--) {
    heapify_down(h, index);
  }
  return h;
}