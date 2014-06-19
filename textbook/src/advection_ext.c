#include <stdio.h>
#include <math.h>

#include "advection_ext.h"

#define N 100
#define l 1.0
#define k 0.4
#define h (2.0 / N)
#define tau (k * h / l)
#define M (2.0 / h / k)

// TODO Fill this pattern and interpolate with it.

// 3rd order.
#ifdef THIRD_ORDER
#define S 1
int stencil[S + 1] = {-1, 0};
double stencil_values[S + 1];
double stencil_Dvalues[S + 1];
#endif
// FIXME Test and remove for students!
// 5th order.
#ifdef FIFTH_ORDER
#define S 2
int stencil[S + 1] = {-1, 0, 1};
double stencil_values[S + 1];
double stencil_Dvalues[S + 1];
#endif

double U_c[N], U_n[N];
double dU_c[N], dU_n[N];

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
	for (ind = 0; ind < N; ind++) {
		if ((ind >= N / 4) && (ind <= 3 * N / 4))
			U_c[ind] = 1.0;
		else
			U_c[ind] = 0.0;
		U_n[ind] = 0.0;
		dU_c[ind] = 0.0;
		dU_n[ind] = 0.0;
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
		dU_c[ind] = 4.0 * M_PI * pow(sin(M_PI * x), 3.0) * cos(M_PI * x); 
		dU_n[ind] = 0.0;
	}
}
#endif

void singleStep(void) {
	int ind;
	for (ind = 0; ind < N; ind++) {
		fillStencilValues(ind);
		// FIXME Stupid 2 calculation of a_i.
		U_n[ind] = interpolatedValue(stencil_values, stencil_Dvalues);
		dU_n[ind] = interpolatedDValue(stencil_values, stencil_Dvalues);
	}
	// FIXME For simplisity copy U_c = U_n, dU_c = dU_n.
	for (ind = 0; ind < N; ind++) {
		U_c[ind] = U_n[ind];
		dU_c[ind] = dU_n[ind];
	}
}

// FIXME The difference is only in this function + stencil (borders for for).
#ifdef THIRD_ORDER
double interpolatedValue(double *u, double *du) {
	// Cubic interpolation: y = a * x^3 + b * x^2 + c * x + d
	double a = (du[1] + du[0]) / h / h - 2.0 * (u[1] - u[0]) / h / h / h;
	double b = (2.0 * du[1] + du[0]) / h - 3.0 * (u[1] - u[0]) / h / h;
	double c = du[1];
	double d = u[1];
	double x = -l * tau;
	double val = a * x * x * x + b * x * x + c * x + d;
	return val;
}
double interpolatedDValue(double *u, double *du) {
	// Cubic interpolation: dy = 3 * a * x^2 + 2 * b * x + c
	double a = (du[1] + du[0]) / h / h - 2.0 * (u[1] - u[0]) / h / h / h;
	double b = (2.0 * du[1] + du[0]) / h - 3.0 * (u[1] - u[0]) / h / h;
	double c = du[1];
	double x = -l * tau;
	double val = 3.0 * a * x * x + 2.0 * b * x + c;
	return val;
}
#endif
// FIXME Rm me for students.
#ifdef FIFTH_ORDER
double interpolatedValue(double *u, double *du) {
	// 5th order interpolation: y = a * x^5 + b * x^4 + c * x^3 + d * x^2 + e * x + f 
	double a = -(3.0 * u[2] - 3.0 * u[0] - 4.0 * h * du[1] - h * du[2] - h * du[0]) / 4.0 / pow(h, 5.0);
	double b = -(-4.0 * u[1] + 2.0 * u[2] + 2.0 * u[0] - h * du[2] + h * du[0]) / 4.0 / pow(h, 4.0);
	double c = -(-5.0 * u[2] + 5.0 * u[0] + 8 * h * du[1] + h * du[2] + h * du[0]) / 4.0 / pow(h, 3.0);
	double d = -(8.0 * u[1] - 4.0 * u[2] - 4.0 * u[0] + h * du[2] - h * du[0]) / 4.0 / pow(h, 2.0);
	double e = du[1];
	double f = u[1];
	double x = -l * tau;
	double val = a * x * x * x * x * x + b * x * x * x * x+ c * x * x * x + d * x * x + e * x + f;
	return val;
}
double interpolatedDValue(double *u, double *du) {
	// 5th order interpolation: dy = 5 * a * x^4 + 4 * b * x^3 + 3 * c * x^2 + 2 * d * x + e
	double a = -(3.0 * u[2] - 3.0 * u[0] - 4.0 * h * du[1] - h * du[2] - h * du[0]) / 4.0 / pow(h, 5.0);
	double b = -(-4.0 * u[1] + 2.0 * u[2] + 2.0 * u[0] - h * du[2] + h * du[0]) / 4.0 / pow(h, 4.0);
	double c = -(-5.0 * u[2] + 5.0 * u[0] + 8 * h * du[1] + h * du[2] + h * du[0]) / 4.0 / pow(h, 3.0);
	double d = -(8.0 * u[1] - 4.0 * u[2] - 4.0 * u[0] + h * du[2] - h * du[0]) / 4.0 / pow(h, 2.0);
	double e = du[1];
	double x = -l * tau;
	double val = 5.0 * a * x * x * x * x + 4.0 * b * x * x * x + 3.0 * c * x * x + 2.0 * d * x + e;
	return val;
}
#endif

void fillStencilValues(int ind) {
	unsigned int j;
	int ind_new;
	for (j = 0; j < S + 1; j++) {
		ind_new = ind + stencil[j];
		if (ind_new < 0) {
			stencil_values[j] = U_c[ind_new + N];
			stencil_Dvalues[j] = dU_c[ind_new + N];
		} else if (ind_new > N - 1) {
			stencil_values[j] = U_c[ind_new - N];
			stencil_Dvalues[j] = dU_c[ind_new - N];
		} else {
			stencil_values[j] = U_c[ind_new];
			stencil_Dvalues[j] = dU_c[ind_new];
		}
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
	// FIXME Print dU too?
	for (ind = 0; ind < N; ind ++)
		fprintf(pFile, "%f\t%f\n", -1.0 + h * ind, U_c[ind]);
	fclose(pFile);
}