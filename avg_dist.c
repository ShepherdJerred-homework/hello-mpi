#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdlib.h>

int ROOT_PROCESS_ID = 0;
int SIZE_OF_INT = sizeof(int);
int DEFAULT_TAG = 0;

int calcDistance(int l, int r) {
    return abs(l - r);
}

int main(int argc, char **argv) {
    int error;
    error = MPI_Init(&argc, &argv);

    int processId;
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    int numberOfProcesses;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    if (processId == ROOT_PROCESS_ID) {
        int target;
//        puts("Enter a target value");
        scanf("%d", &target);

        int totalDifference = 0;

        int i;
        for (i = 0; i < numberOfProcesses; i++) {
//            printf("Enter an experimental value for pid %d", processId);
            int experimentalNumber;
            scanf("%d", &experimentalNumber + i);

            if (i == ROOT_PROCESS_ID) {
//                printf("target: %d, exp: %d", target, experimentalNumber);
                totalDifference += calcDistance(target, experimentalNumber);
            } else {
                MPI_Send(&target, SIZE_OF_INT, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD);
                MPI_Send(&experimentalNumber, SIZE_OF_INT, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD);
            }
        }

        for (i = ROOT_PROCESS_ID + 1; i < numberOfProcesses; i++) {
            MPI_Status status;
            int differenceFromProcess;
            MPI_Recv(&differenceFromProcess, SIZE_OF_INT, MPI_INT, i, DEFAULT_TAG, MPI_COMM_WORLD, &status);
            totalDifference += differenceFromProcess;
        }

        int averageDifference = totalDifference / numberOfProcesses;
        printf("%d", averageDifference);
    } else {
        MPI_Status status;
        int targetNumber;
        int experimentalNumber;
        MPI_Recv(&targetNumber, SIZE_OF_INT, MPI_INT, ROOT_PROCESS_ID, DEFAULT_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&experimentalNumber, SIZE_OF_INT, MPI_INT, ROOT_PROCESS_ID, DEFAULT_TAG, MPI_COMM_WORLD, &status);

        int distance = calcDistance(targetNumber, experimentalNumber);

        MPI_Send(&distance, SIZE_OF_INT, MPI_INT, ROOT_PROCESS_ID, DEFAULT_TAG, MPI_COMM_WORLD);
    }

    error = MPI_Finalize();
}
