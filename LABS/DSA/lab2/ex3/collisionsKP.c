#include "hashtab.h"
#include <stdlib.h>
#include <string.h>

// Глобальная переменная для подсчета коллизий
unsigned int collisions = 0;

// Функция хеширования
unsigned int hashtab_hash(char *key) {
    unsigned int h = 0, hash_mul = 31;
    while (*key) {
        h = h * hash_mul + (unsigned int)*key++;
    }
    return h % HASHTAB_SIZE;
}

// Инициализация хеш-таблицы
void hashtab_init(struct listnode **hashtab) {
    for (int i = 0; i < HASHTAB_SIZE; i++)
        hashtab[i] = NULL;
}

// Добавление элемента в хеш-таблицу
void hashtab_add(struct listnode **hashtab, char *key, int value) {
    struct listnode *node;
    int index = hashtab_hash(key);

    // Проверяем на наличие коллизии
    if (hashtab[index] != NULL) {
        collisions++;  // Увеличиваем счетчик коллизий
    }

    node = malloc(sizeof(*node));
    if (node != NULL) {
        node->key = strdup(key); // Копируем ключ
        node->value = value;
        node->next = hashtab[index];
        hashtab[index] = node;
    }
}

// Поиск элемента в хеш-таблице
struct listnode *hashtab_lookup(struct listnode **hashtab, char *key) {
    int index = hashtab_hash(key);
    struct listnode *node = hashtab[index];
    while (node != NULL) {
        if (strcmp(node->key, key) == 0)
            return node;
        node = node->next;
    }
    return NULL;
}

// Удаление элемента из хеш-таблицы
void hashtab_delete(struct listnode **hashtab, char *key) {
    int index = hashtab_hash(key);
    struct listnode *p, *prev = NULL;
    for (p = hashtab[index]; p != NULL; p = p->next) {
        if (strcmp(p->key, key) == 0) {
            if (prev == NULL)
                hashtab[index] = p->next;
            else
                prev->next = p->next;
            free(p->key);
            free(p);
            return;
        }
        prev = p;
    }
}
