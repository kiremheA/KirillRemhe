#include "BinHeap.h"
#include "Graph.h"

void graph_clear(graph *g) {
  int i, j;
  for (i = 0; i < g->nvertices; i++) {
    g->visited[i] = 0;
    for (j = 0; j < g->nvertices; j++) {
      g->m[i][j] = 0;
    }
  }
}

graph *graph_create(int nvertices) {
  graph *g;
  g = malloc(sizeof(*g));
  g->nvertices = nvertices;
  g->m = malloc(sizeof(int *) * nvertices);
  for (int i = 0; i < g->nvertices; i++) {
    g->m[i] = malloc(sizeof(int) * nvertices);
  }
  g->visited = malloc(sizeof(int) * nvertices);
  graph_clear(g); // Опционально, O(n^2)
  return g;
}

void print_array(heap *Q, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", Q->nodes[i].key);
  }
  putchar('\n');
}

void graph_set_edge(graph *g, int i, int j, int w) {
  g->m[i][j] = w;
  g->m[j][i] = w;
}

void create_bound_graph(graph *g) {
  srand(time(NULL));
  int r;
  for (int i = 0; i < g->nvertices; i++) {
    r = 10 + rand() % 80;
    graph_set_edge(g, i, (i + 1) % g->nvertices, r);
    // graph_set_edge(g, i+1, i, r);
  }
  graph_set_edge(g, 0, g->nvertices - 1, 0);
}

void create_grid_graph(graph *g) {
  srand(time(NULL));
  int r;
  for (int i = 0; i < g->nvertices; i++) {
    for (int j = 0; j < g->nvertices / 2; j++) {
      r = 10 + rand() % 80;
      if (i % 2 == 0) {
        graph_set_edge(g, i, 2 * j + 1, r);
        // g->m[i * g->nvertices + 2 * j + 1] = r;
      } else {
        graph_set_edge(g, i, 2 * j, r);
      }
    }
  }
}

void fprint_graph(FILE *file, graph *g) {
  fprintf(file, "   ");
  for (int i = 0; i < g->nvertices; i++) {
    fprintf(file, "%2d  ", i);
  }
  fprintf(file, "\n\n");
  for (int i = 0; i < g->nvertices; i++) {
    fprintf(file, "%2d  ", i);
    for (int j = 0; j < g->nvertices; j++) {
      fprintf(file, "%2d  ", g->m[i][j]);
    }
    fprintf(file, "\n");
  }
}

void *Dijkstra_shortest_path(graph *g, int src, int *D, int *prev) {
  heap *Q = heap_create(g->nvertices);
  heapnode v;
  int u;
  for (int i = 0; i < g->nvertices; i++) {
    D[i] = INF;
    prev[i] = -1;
    heap_insert(Q, D[i], i);
    g->visited[i] = 0;
  }
  D[src] = 0;
  // prev[src] = -1;
  heap_decrease_key(Q, src, D[src]);
  for (int i = 0; i < g->nvertices; i++) {
    v = heap_extract_min(Q); 
    u = v.value;
    // printf("D[v] %d ", D[u]);
    if (g->visited[u])
      continue;
    g->visited[u] = 1;
    for (int j = 0; j < g->nvertices; j++) {
      if (D[j] > D[u] + g->m[u][j] && !g->visited[j] && g->m[u][j] != 0) {
        D[j] = D[u] + g->m[u][j];
        heap_decrease_key(Q, j, D[j]);
        prev[j] = u;
      }
      // printf("j: %d   ", j);
      // print_array(Q, Q->nnodes);
    }
  }
}

void search_path(int *prev, int src, int dst, int *pathlen, int *path) {
  int i;
  int j;

  *pathlen = 1;
  i = dst;

  while (i != src) {
    (*pathlen)++;
    i = prev[i];
  }

  j = 0;
  i = dst;

  while (i != src) {
    path[*pathlen - j] = i;
    i = prev[i];
    j++;
  }
  path[*pathlen - j] = i;
}

void print_connected(int *prev, int *d, int src, double t) {
  int path[SIZE1], pathlen;
  FILE *bound_file = fopen("BoundGraphRes.txt", "w");
  fprintf(bound_file, "Кратчайший путь от вершины <1>\n");
  for (int j = 0; j < SIZE1; j++) {
    if (j != 1) {
      search_path(prev, src, j, &pathlen, path);
      fprintf(bound_file, "\t<%d>: ", j);
      fprintf(bound_file, "[");
      for (int i = 1; i <= pathlen; i++) {
        fprintf(bound_file, "%d", path[i]);
        if (i < pathlen) {
          fprintf(bound_file, "->");
        }
      }
      fprintf(bound_file, "]\t");
      fprintf(bound_file, "Расстояние: %d\n", d[j]);
    }
  }
  fprintf(bound_file, "~~~~~~~~~~~~~~~\n");
  fprintf(bound_file, "Время: %f\n", t);

  fclose(bound_file);
}
double wtime() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec * 1000 + (double)t.tv_usec / 1000;
}

void print_grid(int *prev2, int *d2, int src, double t2) {
  int path2[SIZE2], pathlen2;

  FILE *grid_file = fopen("GridGraphRes.txt", "w");
  fprintf(grid_file, "Кратчайший путь от вершины <1>:\n");
  for (int j = 0; j < SIZE2; j++) {
    if (j != 1) {
      search_path(prev2, src, j, &pathlen2, path2);

      fprintf(grid_file, "\t<%d>: ", j);
      fprintf(grid_file, "[");
      for (int i = 1; i <= pathlen2; i++) {
        fprintf(grid_file, "%d", path2[i]);
        if (i < pathlen2) {
          fprintf(grid_file, "->");
        }
      }
      fprintf(grid_file, "]  ");
      fprintf(grid_file, "Расстояние: %d\n", d2[j]);
    }
  }
  fprintf(grid_file, "~~~~~~~~~~~~~~~");
  fprintf(grid_file, "\nВремя: %f\n", t2);

  fclose(grid_file);
}
//реализует алгоритм Дейкстры для нахождения кратчайших путей от одной вершины до всех остальных в графе. 
//Алгоритм использует структуру данных "мин-куча" для эффективного нахождения вершины с минимальным расстоянием. 
//В конце работы алгоритма выводится массив кратчайших расстояний dist и массив родительских вершин parent, который 
//позволяет восстановить путь от исходной вершины до всех остальных.
/*
void dijkstra(int graph[V][V], int src) {
    // Здесь мы создаём массив для хранения кратчайших расстояний от начальной вершины до всех остальных
    int dist[V];    

    // Мин-куча для эффективного извлечения вершины с минимальным расстоянием
    struct MinHeap* minHeap = createMinHeap(V);

    // Массив для хранения родительских вершин в пути
    int parent[V];    

    // Инициализация мин-кучи и массива dist
    for (int v = 0; v < V; ++v) {
        parent[src] = -1;
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Устанавливаем расстояние до исходной вершины равным нулю
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]); 

    // Устанавливаем размер кучи равный количеству вершин
    minHeap->size = V;

    // Пока мин-куча не пуста, продолжаем извлекать вершины с минимальным расстоянием
    while (!isEmpty(minHeap)) {
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;

        // Перебираем всех соседей вершины u
        for (int i = 0; i < V; ++i) {
            // Проверяем, находится ли вершина i в куче и обновляем расстояние, если найден более короткий путь через u
            if (isInMinHeap(minHeap, i) && graph[u][i] && dist[u] != INT_MAX && dist[u] + graph[u][i] < dist[i]) {
                // Обновляем расстояние до вершины i
                dist[i] = dist[u] + graph[u][i];
                parent[i] = u;
                decreaseKey(minHeap, i, dist[i]);
            }
        }
    }

    // Выводим полученные кратчайшие пути
    printSolution(dist, V, parent);
}
}*/

int main() {
  FILE *output = fopen("output.txt", "w");
  int src = 1;
  graph *g1 = graph_create(SIZE1);
  graph *g2 = graph_create(SIZE2);
  create_bound_graph(g1);
  create_grid_graph(g2);
  fprint_graph(output, g1);
  fputc('\n', output);
  int D1[g1->nvertices], D2[g2->nvertices];
  int prev1[g1->nvertices], prev2[g2->nvertices];
  double t = wtime();
  Dijkstra_shortest_path(g1, src, D1, prev1);
  t = wtime() - t;
  fputc('\n', output);
  print_connected(prev1, D1, src, t);
  fprintf(output, "Кратчайшие пути из %d в связанном графе:\n", src);
  t = wtime();
  Dijkstra_shortest_path(g2, src, D2, prev2);
  t = wtime() - t;
  print_grid(prev2, D2, src, t);
  for (int i = 0; i < g1->nvertices; i++) {
    fprintf(output, "%d ", D1[i]);
  }
  fprintf(output, "\n\n");
  fprint_graph(output, g2);
  fputc('\n', output);
  fprintf(output, "Кратчайшие пути из %d в графе сетке:\n", src);
  for (int i = 0; i < g2->nvertices; i++) {
    fprintf(output, "%d ", D2[i]);
  }

  return 0;
}