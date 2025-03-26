#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 1000

int main(int argc, char** argv) {
    int rank, Process_total;
    int* array = NULL;
    int subArray_size, start, end; //delimiters
    long local_sum = 0, global_sum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Process_total);

    // Ranges
    subArray_size = ARRAY_SIZE / Process_total;
    start = rank * subArray_size; //automatize each task to work in a determined chunk.
    end = (rank == Process_total - 1) ? ARRAY_SIZE : (rank + 1) * subArray_size;
    int local_size = end - start;

    // Allocate local array for all processes
    int* local_array = (int*)malloc(local_size * sizeof(int));

    // Rank 0 initializes the full array and distributes it
    if (rank == 0) {
        array = (int*)malloc(ARRAY_SIZE * sizeof(int));
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = 1; // Fill with 1s (sum should be 10000)
        }

        // Send chunks to other processes
        for (int i = 1; i < Process_total; i++) {
            int send_start = i * subArray_size;
            int send_end = (i == Process_total - 1) ? ARRAY_SIZE : (i + 1) * subArray_size;
            int send_size = send_end - send_start;
            MPI_Send(&array[send_start], send_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Copy rank 0's chunk into its local_array
        for (int i = 0; i < local_size; i++) {
            local_array[i] = array[i];
        }
    }
    else {
        // Other ranks receive their chunk
        MPI_Recv(local_array, local_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Each process computes its local sum
    for (int i = 0; i < local_size; i++) {
        local_sum += local_array[i];
    }

    // Rank 0 collects results
    if (rank == 0) {
        global_sum = local_sum;
        for (int i = 1; i < Process_total; i++) {
            long recv_sum;
            MPI_Recv(&recv_sum, 1, MPI_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_sum += recv_sum;
        }
        printf("Global sum: %ld\n", global_sum);
    }
    else {
        // Other ranks send their local sum to rank 0
        MPI_Send(&local_sum, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
    }

    // Clean up
    free(local_array);
    if (rank == 0) {
        free(array);
    }

    MPI_Finalize();
    return 0;
}