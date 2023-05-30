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
    char request[MSG_SIZE];
    int request_type;

    while (1) {
        printf("Enter a clients number (1-5) and message (or 'exit' to quit):\n");
        fgets(request, MSG_SIZE, stdin);
        request[strcspn(request, "\n")] = '\0';

        if (strcmp(request, "exit") == 0) {
            break;
        }

        char* token = strtok(request, " ");
        request_type = atoi(token);
        char* message = strtok(NULL, "");

        if (request_type >= 1 && request_type <= 5 && message != NULL) {
            strncpy(shared_data[request_type - 1].msg_text, message, MSG_SIZE);
            shared_data[request_type - 1].client_connected = 1;

            while (shared_data[request_type - 1].client_connected) {
                // Wait for the server's response
                usleep(100000); // Sleep for 100 milliseconds
            }

            printf("Received response from server: %s\n", shared_data[request_type - 1].msg_text);
        } else {
            printf("Invalid request format.\n");
        }
    }

    shmdt(shared_data);

    printf("Disconnected from the server.\n");

    return 0;
}
