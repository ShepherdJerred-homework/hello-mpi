#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

int MAX_NAME_SIZE = 100;

int ROOT_PROCESS_ID = 0;

int DEFAULT_TAG = 0;
int SIZE_OF_NULL_TERMINATOR = 1;
unsigned int SIZE_OF_INT = sizeof(int);

// https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
void printName(int rank, char* name) {
    char *rankString = malloc(SIZE_OF_INT + SIZE_OF_NULL_TERMINATOR);

    snprintf(rankString, SIZE_OF_INT + SIZE_OF_NULL_TERMINATOR, "%d", rank);

    char *MESSAGE_PART_ONE = "Hello, ";
    char *MESSAGE_PART_TWO = ", from process ";
    char *MESSAGE_PART_THREE = "!";

    char *message = malloc(
            strlen(MESSAGE_PART_ONE) + strlen(MESSAGE_PART_TWO) + strlen(MESSAGE_PART_THREE) + strlen(name) + SIZE_OF_INT + SIZE_OF_NULL_TERMINATOR);
    message[0] = '\0';

    int lastNameIndex = (int) strlen(name) - 1;
    name[lastNameIndex] = '\0';

    strcat(message, MESSAGE_PART_ONE);
    strcat(message, name);
    strcat(message, MESSAGE_PART_TWO);
    strcat(message, rankString);
    strcat(message, MESSAGE_PART_THREE);

    puts(message);

    free(message);
    free(rankString);
}

void sendMessage(int rank) {
    int numberOfProcesses;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    char name[MAX_NAME_SIZE + SIZE_OF_NULL_TERMINATOR];
    puts("What is your name?");
    fgets(name, MAX_NAME_SIZE, stdin);

    int i;
    for (i = ROOT_PROCESS_ID + 1; i < numberOfProcesses; i++) {
        MPI_Send(&name, (int) strlen(name), MPI_CHAR, i, DEFAULT_TAG, MPI_COMM_WORLD);
    }

    printName(rank, name);
}

void receiveMessage(int rank) {
    char* name = malloc(MAX_NAME_SIZE + SIZE_OF_NULL_TERMINATOR);
    MPI_Status status;

    MPI_Recv(name, MAX_NAME_SIZE + SIZE_OF_NULL_TERMINATOR, MPI_CHAR, ROOT_PROCESS_ID, DEFAULT_TAG, MPI_COMM_WORLD, &status);

    printName(rank, name);
}

int main(int argc, char **argv) {
    int error;
    error = MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        sendMessage(rank);
    } else {
        receiveMessage(rank);
    }

    error = MPI_Finalize();
}