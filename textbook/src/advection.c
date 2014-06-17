#include <stdio.h>
#include <math.h>

#include "advection.h"

#define N 100
#define l 1.0
#define k 0.4
#define h (2.0 / N)
#define tau (k * h / l)
#define M (2.0 / h / k)

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
// 3rd order.
#ifdef THIRD_ORDER
#define S 3
int stencil[S + 1] = {-2, -1, 0, 1};
double stencil_values[S + 1];
#endif
// 4th order.
#ifdef FOURTH_ORDER
#define S 4
int stencil[S + 1] = {-2, -1, 0, 1, 2};
double stencil_values[S + 1];
#endif

double U_c[N], U_n[N];

int main() {
	unsigned int step;
	initialize();
	debugPrint();
	for (step = 0; step < M; step++) {
	//	debugPrint();
		singleStep();
	}
	debugPrint();
	debugGnuplot();
	return 0;
}

// Rect impulse as initial data.
#ifdef ROUGH_INITIAL
void initialize(void) {
	unsigned int ind;
	for (ind = 0; ind < N; ind++)
		if ((ind >= N / 4) && (ind <= 3 * N / 4)) {
			U_c[ind] = 1.0;
			U_n[ind] = 0.0;
		} else {
			U_c[ind] = 0.0;
			U_n[ind] = 0.0;
		}
}
#endif

// sin^4(PI*x), [-1,1] as initial data.
#ifdef SMOOTH_INITIAL
void initialize(void) {
	unsigned int ind;
	for (ind = 0; ind < N; ind++) {
		double x = -1.0 + ind * h;
		U_c[ind] = pow(sin(M_PI * x), 4.0);
		U_n[ind] = 0.0;
	}
}
#endif

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
	double b = u[1];
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
#ifdef THIRD_ORDER
double interpolatedValue(double *u) {
	// Qubic interpolation: y = a * x^3 + b * x^2 + c * x + d
	double a = (u[3] + 3.0 * u[1] - 3.0 * u[2] - u[0]) / 6.0 / h / h / h;
	double b = (u[3] - 2.0 * u[2] + u[1]) / 2.0 / h / h;
	double c = (2.0 * u[3] + 3.0 * u[2] - 6.0 * u[1] + u[0]) / 6.0 / h;
	double d = u[2];
	double x = -l * tau;
	double val = a * x * x * x + b * x * x + c * x + d;
	return val;
}
#endif
#ifdef FOURTH_ORDER
double interpolatedValue(double *u) {
	// 4th order interpolation: y = a * x^4 + b * x^3 + c * x^2 + d * x + e
	// FIXME Strange value 25!
	/*
	double a = (6.0 * u[2] - 4.0 * u[3] - 4.0 * u[1] + u[4] + u[0]) / 24.0 / h / h / h / h;
	double b = (2.0 * u[1] - 2.0 * u[3] + u[4] - u[0]) / 12.0 / h / h / h;
	double c = (16.0 * u[3] - 30.0 * u[2] + 16.0 * u[1] - u[4] + u[0]) / 24.0 / h / h;
	double d = (8.0 * u[3] - 8.0 * u[1] - u[4] + u[0]) / 12.0 / h;
	double e = u[2];
	double x = -l * tau;
	double val = a * x * x * x * x + b * x * x * x + c * x * x + d * x + e;
	return val;
	*/
	// FIXME It is from RECT and look like correct!
	///*
	double c = l * tau / h;
	double t1 = 1 / 24.0 * (-2.0 * u[4] + 16.0 * u[3] - 16.0 * u[1] + 2.0 * u[0]);
	double t2 = 1 / 24.0 * (-u[4] + 16.0 * u[3] - 30.0 * u[2] + 16.0 * u[1] - u[0]);
	double t3 = 1 / 24.0 * (2.0 * u[4] - 4.0 * u[3] + 4.0 * u[1] - 2.0 * u[0]);
	double t4 = 1 / 24.0 * (u[4] - 4.0 * u[3] + 6.0 * u[2] - 4.0 * u[1] + u[0]);
	return u[2] - c * (t1 - c * (t2 - c * (t3 - c * t4)));
	//*/

}
#endif
void fillStencilValues(int ind) {
	unsigned int j;
	int ind_new;
	for (j = 0; j < S + 1; j++) {
		ind_new = ind + stencil[j];
		if (ind_new < 0)
			stencil_values[j] = U_c[ind_new + N];
		else if (ind_new > N - 1)
			stencil_values[j] = U_c[ind_new - N];
		else
			stencil_values[j] = U_c[ind_new];
	}
}

void debugPrint(void) {
	unsigned int ind;
	for (ind = 0; ind < N; ind++)
#define eps 0.1
		if ((U_c[ind] < eps) && (U_c[ind] > -eps))
			printf("_");
		else
			printf("|");
	printf("\n");
}

void debugGnuplot(void) {
	FILE *pFile;
	pFile = fopen("plot.dat", "w");
	unsigned int ind;
	for (ind = 0; ind < N; ind ++)
		fprintf(pFile, "%f\t%f\n", -1.0 + h * ind, U_c[ind]);
	fclose(pFile);
}
