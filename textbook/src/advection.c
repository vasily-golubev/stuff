#include <stdio.h>

#include "advection.h"

#define N 100
#define l 1.0
#define k 0.4
#define h (2.0 / N)
#define tau (k * h / l)
#define M (2.0 / l / tau)

// TODO Implement clear logic for stencil.
#define S 1

double U_c[N], U_n[N];

int main() {
	unsigned int step;
	initialize();
	for (step = 0; step < M; step++) {
		debugPrint();
		singleStep();
	}
	return 0;
}

void initialize(void) {
	unsigned int ind;
	for (ind = 0; ind < N; ind++)
		if ((ind >= N / 4) && (ind <= 3 * N / 4)) {
			U_c[ind] = 1.0;
			U_n[ind] = -.0;
		} else {
			U_c[ind] = 0.0;
			U_n[ind] = 0.0;
		}
}

void singleStep(void) {
	int ind, ind_0, ind_1;
	for (ind = 0; ind < N; ind++) {
		ind_0 = ind;
		ind_1 = ind - S;
		if (ind_1 < 0)
			ind_1 += N; 
		U_n[ind] = interpolatedValue(U_c[ind_0], U_c[ind_1]);
	}
	// FIXME For simplisity copy U_c = U_n
	for (ind = 0; ind < N; ind++)
		U_c[ind] = U_n[ind];
}

// FIXME The difference is only in this function + stencil (borders for for).
double interpolatedValue(double u_0, double u_1) {
	// Linear interpolation: y = a * x + b
	double a = (u_0 - u_1) / h;
	double b = u_0;
	double val = a * (-l * tau) + b;
	return val;
}

void debugPrint(void) {
	unsigned int ind;
	for (ind = 0; ind < N; ind++)
#define eps 0.1
		if (U_c[ind] < eps)
			printf("_");
		else
			printf("|");
	printf("\n");
}
