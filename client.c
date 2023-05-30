#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>


#define MSG_SIZE 256
#define MAX_CLIENTS 5

typedef struct {
    int client_connected;
    char msg_text[MSG_SIZE];
} SharedData;

int main() {
    key_t key;
    int shm_id;
    SharedData* shared_data;

    if ((key = ftok("server8.c", 'B')) == -1) {
        perror("ftok");
        exit(1);
    }

    if ((shm_id = shmget(key, sizeof(SharedData) * MAX_CLIENTS, 0644)) == -1) {
        perror("shmget");
        exit(1);
    }

    if ((shared_data = (SharedData*)shmat(shm_id, NULL, 0)) == (void*)-1) {
        perror("shmat");
        exit(1);
    }

    printf("Connected to the server.\n");
