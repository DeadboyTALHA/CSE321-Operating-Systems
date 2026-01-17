#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void *t_func(int *arg);
sem_t s1, s2;

pthread_mutex_t seat_mutex;

int student_id[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};  // 10 students
int waiting_students = 0;
int num_of_seats = 3;
int num_of_served_students = 0;

int main() {
    pthread_t t[10];  // 10 threads of 10 students
    
    sem_init(&s1, 0, 1);  // for taking consultation 1 to 1
    sem_init(&s2, 0, 0);  // for sleeping and waking of tutor
    
    pthread_mutex_init(&seat_mutex, NULL);
    
    
    for (int i = 0; i < 10; i++) {
        pthread_create(&t[i], NULL, (void *)t_func, &student_id[i]);
    }
    
    for (int i = 0; i < 10; i++) {
        pthread_join(t[i], NULL);
    }
    return 0;

}


void *t_func(int *arg) {
    pthread_mutex_lock(&seat_mutex);
    if (waiting_students < num_of_seats) {
        printf("Student %d started waiting for consultation\n", *arg);
        waiting_students++;
    }
    else {
        printf("No chairs remaining in lobby. Student %d leaving.....\n", *arg);
        pthread_mutex_unlock(&seat_mutex);
        return NULL;
    }
    pthread_mutex_unlock(&seat_mutex);
    
    if (waiting_students > 0) {
        sem_post(&s2);
        sem_wait(&s1);
        waiting_students--;
        printf("A waiting student started getting consultation\nNumber of students now waiting: %d\n", waiting_students);
        printf("ST giving consultation\n");
        printf("Student %d is getting consultation\n", *arg);
        
        num_of_served_students++;
        printf("Student %d finished getting consultation and left\nNumber of served students: %d\n", *arg, num_of_served_students);
        sem_post(&s1);
    }
    else {
        sem_wait(&s2);
    }

}
