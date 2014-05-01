#include <stdio.h>

#include "advection.h"

#define N 100
#define l 1.0
#define k 0.4
#define h (2.0 / N)
#define tau (k * h / l)
#define M (2.0 / l / tau)

// TODO Fill this pattern and interpolate with it.

// 1st order.
#ifdef FIRST_ORDER
#define S 1
int stencil[S + 1] = {-1, 0};
double stencil_values[S + 1];
#endif
// 2nd order.
#ifdef SECOND_ORDER
#define S 2
int stencil[S + 1] = {-1, 0, 1};
double stencil_values[S + 1];
#endif

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
	int ind;
	for (ind = 0; ind < N; ind++) {
		fillStencilValues(ind);
		U_n[ind] = interpolatedValue(stencil_values);
	}
	// FIXME For simplisity copy U_c = U_n
	for (ind = 0; ind < N; ind++)
		U_c[ind] = U_n[ind];
}

// FIXME The difference is only in this function + stencil (borders for for).
#ifdef FIRST_ORDER
double interpolatedValue(double *u) {
	// Linear interpolation: y = a * x + b
	double a = (u[1] - u[0]) / h;
	double b = u[0];
	double x = -l * tau;
	double val = a * x + b;
	return val;
}
#endif
#ifdef SECOND_ORDER
double interpolatedValue(double *u) {
	// Quadratic interpolation: y = a * x^2 + b * x + c
	double a = (u[2] - 2.0 * u[1] + u[0]) / 2.0 / h / h;
	double b = (u[2] - u[0]) / 2.0 / h;
	double c = u[1];
	double x = -l * tau;
	double val = a * x * x + b * x + c;
	return val;
}
#endif
void fillStencilValues(int ind) {
	unsigned int j;
	int ind_new;
	for (j = 0; j < S; j++) {
		ind_new = ind + stencil[j];
		if (ind_new < 0)
			stencil_values[j] = U_c[ind_new + N];
		else if (ind > N - 1)
			stencil_values[j] = U_c[ind_new - N];
		else
			stencil_values[j] = U_c[ind_new];
	}
}

void debugPrint(void) {
	unsigned int ind;
	for (ind = 0; ind < N; ind++)
#define eps 0.01
		if (U_c[ind] < eps)
			printf("_");
		else
			printf("|");
	printf("\n");
}
