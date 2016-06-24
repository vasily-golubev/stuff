#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define MPI_TYPE_REAL MPI_FLOAT
typedef float real_t;

#define NX 1001
#define NY 1
#define NZ 2001
#define NT 2001
#define dim 3
#define comp 2

using namespace std;

string make_vtk_header(
	const char *label,
	int d1, int d2, int d3,
	real_t s1, real_t s2, real_t s3,
	real_t o1, real_t o2, real_t o3,
	int size
					)
{
	stringstream ss;
	ss << "# vtk DataFile Version 3.0\n";
	ss << label << "\n";
	ss << "BINARY\n";
	ss << "DATASET STRUCTURED_POINTS\n";
	ss << "DIMENSIONS " << d1 << " " << d2 << " " << d3 << "\n";
	ss << "SPACING " << s1 << " " << s2 << " " << s3 << "\n";
	ss << "ORIGIN " << o1 << " " << o2 << " " << o3 << "\n";
	ss << "POINT_DATA " << size << "\n";
	return ss.str();
}

void write_float(ofstream &i_stm, float i_f) {
	const char *d = (const char *)&i_f;
	i_stm.write(d + 3, sizeof(char));
	i_stm.write(d + 2, sizeof(char));
	i_stm.write(d + 1, sizeof(char));
	i_stm.write(d + 0, sizeof(char));
}

float read_float(ifstream &i_stm) {
	float f;
	char *d = (char *)&f;
	i_stm.read(d + 0, sizeof(char));
	i_stm.read(d + 1, sizeof(char));
	i_stm.read(d + 2, sizeof(char));
	i_stm.read(d + 3, sizeof(char));
	return f;
}

void read_data(real_t *input_data) {
	ifstream file("./model0_full.bin");
	file.read((char *)input_data, dim * NX * NY * NT * sizeof(real_t));
	file.close();
}

void save_input(real_t *input_data) {
	ofstream file("./model0_full_input.vtk");
	file << make_vtk_header(
		"Created by Golubev",
		NX, NY, NT,
		1, 1, 1,
		0, 0, 0,
		NX * NY * NT);
	file << "SCALARS I float 1 \n";
	file << "LOOKUP_TABLE I_table \n";
	for (int i = 0; i < NT; i++)
		for (int j = 0; j < NX; j++) {
			int ind = comp * (NX * NT) + j * NT + i;
			write_float(file, input_data[ind]);
		}
	file.close();
}

void save_model(real_t *data) {
	ofstream file("./model0_full.vtk");
	file << make_vtk_header(
		"Created by Golubev",
		NX, NY, NT,
		1, 1, 1,
		0, 0, 0,
		NX * NY * NT);
	file << "SCALARS M float 1 \n";
	file << "LOOKUP_TABLE M_table \n";
	for (int i = 0; i < NT; i++)
		for (int j = 0; j < NX; j++) {
			int ind = j * NT + i;
			write_float(file, data[ind]);
		}
	file.close();
}

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
	input_data = (real_t *)malloc(dim * NX * NY * NT * sizeof(real_t));
	if (rank == 0)
		read_data(input_data);
	// Send input data to all processes.
	MPI_Bcast(input_data, dim * NX * NY * NT, MPI_TYPE_REAL, 0, MPI_COMM_WORLD);
	// Fill local data.
	local_data = (real_t *)malloc(D * sizeof(real_t));
	for (int i = 0; i < D; i++) {
		// ind = comp * (NX * NT) + x * NT + t
		int ind = comp * (NX * NT) + rank * D + i;
		local_data[i] = input_data[ind];
	}
	// Allocate storage at MASTER.
	if (rank == 0)
		data = (real_t *)malloc(NX * NY * NZ * sizeof(real_t));
	// Send and receive.
	MPI_Gather(local_data, D, MPI_TYPE_REAL, data, D, MPI_TYPE_REAL, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		save_input(input_data);
		save_model(data);
	}
	MPI_Finalize();
	return 0;
}
