#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define MPI_TYPE_REAL MPI_FLOAT
typedef float real_t;

#define MPI_CHECK(x) \
	{ if (x != MPI_SUCCESS) cout << "Error connected with MPI system" << endl; }
#define MALLOC_CHECK(x) \
	{ if (x == NULL) cout << "Error connected with malloc call" << endl; }

// 2D parameters
#define NX 501
#define NY 1
#define NZ 1
#define NT 401

// 3D parameters
//#define NX 201
//#define NY 201
//#define NZ 51
//#define NT 2001
#define dim 3 // 3D data collected (now true for 2D tests too)
#define comp 2 // 0 = VX, 1 = VY, 2 = VZ

using namespace std;

int D = 0;
real_t h = 20.0f; // 2D test
//real_t h = 50.0f; // 3D test

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
	//ifstream file("./modelc_3D_50_d.bin"); // 3D test
	ifstream file("./modelc_full_20_10.bin"); // 2D test
	file.read((char *)input_data, dim * NX * NY * NT * sizeof(real_t));
	file.close();
}

void save_input(real_t *input_data) {
	//ofstream file("./modelc_3D_50_d_input.vtk"); // 3D test
	ofstream file("./modelc_full_20_10_input.vtk"); // 2D test
	file << make_vtk_header(
		"Elastic Seismogram",
		NX, NY, NT,
		h, h, h,
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
	//ofstream file("./modelc_3D_50_d.vtk"); // 3D test
	ofstream file("./modelc_full_20_10.vtk"); // 2D test
	file << make_vtk_header(
		"Migration Image",
		NX, NY, NZ,
		h, h, h,
		0, 0, 0,
		NX * NY * NZ);
	file << "SCALARS M float 1 \n";
	file << "LOOKUP_TABLE M_table \n";
	for (int k = 0; k < NZ; k++) {
		for (int j = 0; j < NY; j++) {
			for (int i = 0; i < NX; i++) {
				int ind = i * NY * NZ + j * NZ + k;
				write_float(file, data[ind]);
			}
		}
	}
	file.close();
}

void process_local_data(real_t *input_data, real_t *local_data, int rank) {
	real_t tay = 0.01f; // 2D test
	//real_t tay = 0.002f; // 3D test
	real_t cP = 2500.0f / 2.0f;
	real_t cS = 1250.0f / 2.0f;
	real_t div0 = h / 2.0f;
	int Kh = 1;

	real_t M_Q[3][2];
	real_t M_S[3][2];
	real_t M_Rz3[3];
	real_t M_Rxyz[2][2];
	real_t M_Raab[3][2][3][3];

	// FIXME Correct size for MPI processes
	real_t *U1 = (real_t *)malloc(D * sizeof(real_t));
	MALLOC_CHECK(U1);
	real_t *U2 = (real_t *)malloc(D * sizeof(real_t));
	MALLOC_CHECK(U2);
	real_t *U3 = (real_t *)malloc(D * sizeof(real_t));
	MALLOC_CHECK(U3);
	real_t *U_norm = (real_t *)malloc(D * sizeof(real_t));
	MALLOC_CHECK(U_norm);

	for (int ind = 0; ind < D; ind++) {
		if (rank == 0)
			if (ind % 100 == 0)
				cout << ind << " from " << D << endl;
		int index = rank * D + ind;
		int I = index / (NY * NZ);
		int J = (index - I * NY * NZ) / NZ;
		int K = index - I * NY * NZ - J * NZ;

		U1[ind] = 0.0f;
		U2[ind] = 0.0f;
		U3[ind] = 0.0f;

		for (int i = 0; i < NX; i++) {
			for (int j = 0; j < NY; j++) {
				real_t M_X = I * h - i * h;
				real_t M_Y = J * h - j * h;
				real_t M_Z = K * h;
				real_t M_r = sqrt(M_X * M_X + M_Y * M_Y + M_Z * M_Z);

				if (M_r > div0) {
					M_X = M_X / M_r;
					M_Y = M_Y / M_r;
					M_Z = M_Z / M_r;
					for (int M_n = 0; M_n < 3; M_n++) {
						for (int M_A = 0; M_A < 2; M_A++) {
							real_t M_cA, M_cB, M_U;
							if (M_A == 0)
								M_cA = cP;
							if (M_A == 1)
								M_cA = cS;

							if (M_n == 2)
								M_cB = cP;
							else
								M_cB = cS;
							int M_LrcA = (int)(M_r / (M_cA * tay));
							M_S[M_n][M_A] = 0.0f;
							if (M_LrcA < NT) {
								if (M_n == 0)
									M_U = input_data[0 * (NX * NY * NT) + i * Kh * NT + M_LrcA];
								if (M_n == 1)
									M_U = input_data[1 * (NX * NY * NT) + i * Kh * NT + M_LrcA];
								if (M_n == 2)
									M_U = input_data[2 * (NX * NY * NT) + i * Kh * NT + M_LrcA];
								M_S[M_n][M_A] = (M_cB / M_cA) * (M_cB / M_cA) * M_U;

							}
							M_Q[M_n][M_A] = 0.0f;
							if (M_LrcA > NT)
								M_LrcA = NT;
							for (int l = 0; l <  M_LrcA; l++) {
								if (M_n == 0)
									M_U = input_data[0 * (NX * NY * NT) + i * Kh * NT + l];
								if (M_n == 1)
									M_U = input_data[1 * (NX * NY * NT) + i * Kh * NT + l];
								if (M_n == 2)
									M_U = input_data[2 * (NX * NY * NT) + i * Kh * NT + l];
								M_Q[M_n][M_A] = M_Q[M_n][M_A] + M_U * l;
							}
							M_Q[M_n][M_A] = M_Q[M_n][M_A] * (M_cB * tay / M_r) * (M_cB * tay / M_r);

							for (int M_a2 = 0; M_a2 < 3; M_a2++) {
								for (int M_b = 0; M_b < 3; M_b++) {
									int M_aCo, M_bCo;
									if (M_a2 == M_b)
										M_Raab[M_n][M_A][M_a2][M_b] = 0.0f;
									else {
										if (M_a2 == 0)
											M_aCo = M_X;
										if (M_a2 == 1)
											M_aCo = M_Y;
										if (M_a2 == 2)
											M_aCo = M_Z;
										if (M_b == 0)
											M_bCo = M_X;
										if (M_b == 1)
											M_bCo = M_Y;
										if (M_b == 2)
											M_bCo = M_Z;
										M_Raab[M_n][M_A][M_a2][M_b] = M_bCo * ( (3.0 - 7.0 * M_aCo * M_aCo) * M_Q[M_n][M_A] + (2.0 * M_aCo * M_aCo - 1.0) * M_S[M_n][M_A]);
									}
								}
							}
						}
						int M_A;
						if (M_n == 2)
							M_A = 0;
						else
							M_A = 1;
						M_Rz3[M_n] = 3.0 * M_Z * ( (3.0 - 5.0 * M_Z * M_Z) * M_Q[M_n][M_A] + (2.0 * M_Z * M_Z - 1.0) * M_S[M_n][M_A]);
					}

					for (int M_n = 0; M_n < 2; M_n++) {
						for (int M_A = 0; M_A < 2; M_A++) {
							M_Rxyz[M_n][M_A] = 0.0 - M_X * M_Y * M_Z * M_Q[M_n][M_A];
						}
					}

					U1[ind] = U1[ind] + (0.0 - M_Raab[0][0][0][2] - M_Raab[0][1][1][2] - M_Rz3[0] - M_Rxyz[1][0] + M_Rxyz[1][1] - M_Raab[2][0][2][0] + M_Raab[2][1][2][0]) * (h / M_r) * (h / M_r);
					U2[ind] = U2[ind] + (0.0 - M_Raab[1][0][1][2] - M_Raab[1][1][0][2] - M_Rz3[1] - M_Rxyz[0][0] + M_Rxyz[0][1] - M_Raab[2][0][2][1] + M_Raab[2][1][2][1]) * (h / M_r) * (h / M_r);
					U3[ind] = U3[ind] + (0.0 - M_Rz3[2] - M_Raab[2][1][0][2] - M_Raab[2][1][1][2] - M_Raab[0][0][2][0] + M_Raab[0][1][2][0] - M_Raab[1][0][2][1] + M_Raab[1][1][2][1]) * (h / M_r) * ( h / M_r);

				}
			}
		}
		U_norm[ind] = sqrt(U1[ind] * U1[ind] + U2[ind] * U2[ind] + U3[ind] * U3[ind]);
		// TODO Prepare for sending result
		local_data[ind] = U_norm[ind];
	}
	free(U1);
	free(U2);
	free(U3);
	free(U_norm);
}

int main(int argc, char *argv[]) {
	int size, rank;
	real_t *local_data = NULL;
	real_t *data = NULL;
	real_t *input_data = NULL;
	MPI_CHECK(MPI_Init(&argc, &argv));
	MPI_CHECK(MPI_Comm_size(MPI_COMM_WORLD, &size));
	MPI_CHECK(MPI_Comm_rank(MPI_COMM_WORLD, &rank));
	// FIXME Calculate local length D. Now think (NX * NY * NZ) % size == 0
	if ( (NX * NY * NZ) % size != 0) {
		cout << "Doesn't support non uniform task splitting" << endl;
		return EXIT_FAILURE;
	}
	D = (NX * NY * NZ) / size;
	if (rank == 0)
		cout << "CHAIN (D) = " << D << ", THREADS = " << size << endl;
	// Read input data
	input_data = (real_t *)malloc(dim * NX * NY * NT * sizeof(real_t));
	MALLOC_CHECK(input_data);
	if (rank == 0) {
		cout << "READING" << endl;
		read_data(input_data);
	}
	// Send input data to all processes
	MPI_CHECK(MPI_Bcast(input_data, dim * NX * NY * NT, MPI_TYPE_REAL, 0, MPI_COMM_WORLD));
	// Fill local data
	local_data = (real_t *)malloc(D * sizeof(real_t));
	if (rank == 0)
		cout << "PROCESSING" << endl;
	process_local_data(input_data, local_data, rank);
	// Allocate storage at MASTER
	if (rank == 0) {
		data = (real_t *)malloc(NX * NY * NZ * sizeof(real_t));
		MALLOC_CHECK(data);
	}
	// Send and receive
	if (rank == 0)
		cout << "SENDING" << endl;
	MPI_CHECK(MPI_Gather(local_data, D, MPI_TYPE_REAL, data, D, MPI_TYPE_REAL, 0, MPI_COMM_WORLD));
	if (rank == 0) {
		cout << "SAVING" << endl;
		save_input(input_data);
		save_model(data);
	}
	// Clean memory
	free(local_data);
	free(input_data);
	if (rank == 0)
		free(data);
	MPI_CHECK(MPI_Finalize());
	return EXIT_SUCCESS;
}
