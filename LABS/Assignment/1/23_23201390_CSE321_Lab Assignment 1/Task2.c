#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define BREAD 0
#define CHEESE 1
#define LETTUCE 2
sem_t a;
sem_t b[3];
int c;
int d = 0;
int e = -1;
int f = -1;
const char *g[3] = {"Bread", "Cheese", "Lettuce"};
const char *h[3] = {"Maker A", "Maker B", "Maker C"};
int i1[4] = {BREAD, CHEESE, BREAD, BREAD};
int i2[4] = {CHEESE, LETTUCE, LETTUCE, CHEESE};
int m[4] = {2, 0, 1, 2};
void *supplier(void *arg) {
  for (int x = 0; x < c; x++) {
    sem_wait(&a);
    e = i1[x];
    f = i2[x];
    printf("Supplier places: %s and %s\n", g[e], g[f]);
    sem_post(&b[m[x]]);
  }
  sem_wait(&a);
  e = -1;
  f = -1;
  d = 1;
  for (int x = 0; x < 3; x++) {
    sem_post(&b[x]);
  }
  return NULL;
}

void *maker(void *arg) {
  int x = *(int *)arg;
  free(arg);
  while (1) {
    sem_wait(&b[x]);
    if (d && e == -1 && f == -1) {
      break;
    }
    printf("%s picks up %s and %s\n", h[x], g[e], g[f]);
    printf("%s is making the sandwich...\n", h[x]);
    printf("%s finished making the sandwich and eats it\n", h[x]);
    printf("%s signals Supplier\n\n", h[x]);
    sem_post(&a);
  }
  return NULL;
}

int main() {
  scanf("%d", &c);
  sem_init(&a, 0, 1);
  for (int x = 0; x < 3; x++) {
    sem_init(&b[x], 0, 0);
  }
  pthread_t s, mk[3];
  for (int x = 0; x < 3; x++) {
    int *id = malloc(sizeof(int));
    *id = x;
    pthread_create(&mk[x], NULL, maker, id);
  }
  pthread_create(&s, NULL, supplier, NULL);
  pthread_join(s, NULL);
  for (int x = 0; x < 3; x++) {
    pthread_join(mk[x], NULL);
  }
  return 0;
}