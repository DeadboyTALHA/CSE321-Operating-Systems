#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int* fib;
int n;
void* make_fib(void* arg) {
  int a = 0;
  int b = 1;
  int c;
  fib = (int*)malloc((n + 1) * sizeof(int));
  if (n >= 0) {
    fib[0] = 0;
  }
  if (n >= 1) {
    fib[1] = 1;
  }
  for (int i = 2; i <= n; i++) {
    c = a + b;
    fib[i] = c;
    a = b;
    b = c;
  }
  return NULL;
}

void* find_num(void* arg) {
  int idx = *((int*)arg);
  if (idx >= 0 && idx <= n) {
    *((int*)arg) = fib[idx];
  } else {
    *((int*)arg) = -1;
  }
  return NULL;
}

int main() {
  int s;
  pthread_t t1;
  printf("Enter the term of fibonacci sequence:\n");
  scanf("%d", &n);
  if (n < 0 || n > 40) {
  printf("Wrong input!\n");
  return 1;
  }
  pthread_create(&t1, NULL, make_fib, NULL);
  pthread_join(t1, NULL);
  printf("How many numbers you are willing to search?:\n");
  scanf("%d", &s);
  if (s <= 0) {
    printf("Wrong input!\n");
    free(fib);
    return 1;
  }
  int* searches = (int*)malloc(s * sizeof(int));
  pthread_t* threads = (pthread_t*)malloc(s * sizeof(pthread_t));
  for (int i = 0; i < s; i++) {
    printf("Enter search %d:\n", i + 1);
    scanf("%d", &searches[i]);
    pthread_create(&threads[i], NULL, find_num, &searches[i]);
  }
  for (int i = 0; i <= n; i++) {
    printf("a[%d] = %d\n", i, fib[i]);
  }
  for (int i = 0; i < s; i++) {
    pthread_join(threads[i], NULL);
    printf("result of search #%d = %d\n", i + 1, searches[i]);
  }
  free(fib);
  free(searches);
  free(threads);
  return 0;
}