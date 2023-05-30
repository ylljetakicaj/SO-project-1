#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_CLIENTS 5
#define MSG_SIZE 256

typedef struct {
    int client_connected;
    char msg_text[MSG_SIZE];
} SharedData;

SharedData* shared_data;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t available_clients;

void* clientThread(void* arg) {
    int client_index = *((int*)arg);

    while (1) {
        sem_wait(&available_clients);
        pthread_mutex_lock(&clients_mutex);
        if (shared_data[client_index].client_connected) {
            printf("Received message from client %d: %s\n", client_index + 1, shared_data[client_index].msg_text);

            // Process the request and generate a response
            char response[MSG_SIZE + 300];
            snprintf(response, sizeof(response), "Server response to client %d: %s", client_index, shared_data[client_index].msg_text);
            strncpy(shared_data[client_index].msg_text, response, MSG_SIZE);

            shared_data[client_index].client_connected = 0;
        }
        pthread_mutex_unlock(&clients_mutex);
        sem_post(&available_clients);
    }

    pthread_exit(NULL);
}
int main() {
    key_t key;
    int shm_id;
    pthread_t client_threads[MAX_CLIENTS];
    int client_indices[MAX_CLIENTS];

    if ((key = ftok("server.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((shm_id = shmget(key, sizeof(SharedData) * MAX_CLIENTS, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

    if ((shared_data = (SharedData*)shmat(shm_id, NULL, 0)) == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    printf("Server started. Listening for connections...\n");

    for (int i = 0; i < MAX_CLIENTS; i++) {
        shared_data[i].client_connected = 0;
    }

    sem_init(&available_clients, 0, MAX_CLIENTS);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_indices[i] = i;
        pthread_create(&client_threads[i], NULL, clientThread, &client_indices[i]);
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(client_threads[i], NULL);
    }

    sem_destroy(&available_clients);
    shmdt(shared_data);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
