#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#define MPI_TYPE_REAL MPI_FLOAT
typedef float real_t;

#define NX 200
#define NY 4
#define NZ 6
#define NT 10

using namespace std;

int main(int argc, char *argv[]) {
	int size, rank;
	real_t *local_data = NULL;
	real_t *data = NULL;
	real_t *input_data = NULL;
	// FIXME Add error checking everywhere.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// FIXME Calculate local length D. Now think (NX * NY * NZ) % size == 0.
	int D = (NX * NY * NZ) / size;
	if (rank == 0)
		cout << "D = " << D << endl;
	// Read input data.
	input_data = (real_t *)malloc(NX * NY * NT * sizeof(real_t));
	if (rank == 0)
		// FIXME Read seismogram from file.
		for (int i = 0; i < NX * NY * NT; i++)
			input_data[i] = i;
	// Send input data to all processes.
	MPI_Bcast(input_data, NX * NY * NT, MPI_TYPE_REAL, 0, MPI_COMM_WORLD);
	// Fill local data.
	local_data = (real_t *)malloc(D * sizeof(real_t));
	for (int i = 0; i < D; i++) {
		//local_data[i] = rank;
		local_data[i] = input_data[rank + i];
		// TODO Insert local logic.
	}
	// Allocate storage at MASTER.
	if (rank == 0)
		data = (real_t *)malloc(NX * NY * NZ * sizeof(real_t));
	// Send and receive.
	MPI_Gather(local_data, D, MPI_TYPE_REAL, data, D, MPI_TYPE_REAL, 0, MPI_COMM_WORLD);
	// Visualize results.
	if (rank == 0) {
		for (int i = 0; i < D * size; i++) {
			if (i % D == 0)
				cout << endl;
			cout << data[i] << " ";
		}
		cout << endl;
	}
	MPI_Finalize();
	return 0;
}
