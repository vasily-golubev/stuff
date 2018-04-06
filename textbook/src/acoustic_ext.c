#include <stdio.h>
#include <math.h>

#include "acoustic_ext.h"

#define c_0 1
#define Z_0 1 // Z_0=rho_0*c_0
#define k 0.4
#define h (2.0 / N)
#define tau (k * h / c_0)
#define M (0.25 / h / k)

// 3rd order extended scheme is realized.
#define S 1
int stencil_l[S + 1] = {-1, 0};
int stencil_r[S + 1] = {0, 1};
double stencil_values_w1[S + 1];
double stencil_values_w2[S + 1];
double stencil_Dvalues_w1[S + 1];
double stencil_Dvalues_w2[S + 1];
#define LEFT 0
#define RIGHT 1

int main() {
	unsigned int step;
	initialize();
	debugPrint();
	for (step = 0; step < M; step++) {
		singleStep();
	}
	debugPrint();
	return 0;
}

// Rect impulse as initial data.
void initialize(void) {
	unsigned int ind;
	for (ind = 0; ind < N; ind++) {
		if ((ind >= N / 4) && (ind <= 3 * N / 4)) {
			W1_c[ind] = 1.0 / (2.0 * Z_0);
			W2_c[ind] = -1.0 / (2.0 * Z_0);
		} else {
			W1_c[ind] = 0.0;
			W2_c[ind] = 0.0;
		}
		W1_n[ind] = 0.0;
		W2_n[ind] = 0.0;
		dW1_c[ind] = 0.0;
		dW2_c[ind] = 0.0;
		dW1_n[ind] = 0.0;
		dW2_n[ind] = 0.0;
	}
}

void singleStep(void) {
	int ind;
	for (ind = 0; ind < N; ind++) {
		fillStencilValues(ind);
		// FIXME Remove calculating a_i twice.
		W1_n[ind] = interpolatedValue(stencil_values_w1, stencil_Dvalues_w1, RIGHT);
		dW1_n[ind] = interpolatedDValue(stencil_values_w1, stencil_Dvalues_w1, RIGHT);
		W2_n[ind] = interpolatedValue(stencil_values_w2, stencil_Dvalues_w2, LEFT);
		dW2_n[ind] = interpolatedDValue(stencil_values_w2, stencil_Dvalues_w2, LEFT);
	}
	// FIXME For simplisity copy Wi_c = Wi_n, dWi_c = dWi_n.
	for (ind = 0; ind < N; ind++) {
		W1_c[ind] = W1_n[ind];
		dW1_c[ind] = dW1_n[ind];
		W2_c[ind] = W2_n[ind];
		dW2_c[ind] = dW2_n[ind];
	}
}

double interpolatedValue(double *u, double *du, int dir) {
	// Cubic interpolation: y = a * x^3 + b * x^2 + c * x + d
	double a, b, c, d, x;
	if (dir == LEFT) {
		a =  (du[1] + du[0]) / h / h - 2.0 * (u[1] - u[0]) / h / h / h;
		b = (2.0 * du[1] + du[0]) / h - 3.0 * (u[1] - u[0]) / h / h;
		c = du[1];
		d = u[1];
		x = -c_0 * tau;
	} else {
		a =  (du[1] + du[0]) / h / h - 2.0 * (u[1] - u[0]) / h / h / h;
		b = -(2.0 * du[0] + du[1]) / h + 3.0 * (u[1] - u[0]) / h / h;
		c = du[0];
		d = u[0];
		x = c_0 * tau;
	}
	double val = a * x * x * x + b * x * x + c * x + d;
	return val;
}
double interpolatedDValue(double *u, double *du, int dir) {
	// Cubic interpolation: dy = 3 * a * x^2 + 2 * b * x + c
	double a, b, c, x;
	if (dir == LEFT) {
		a = (du[1] + du[0]) / h / h - 2.0 * (u[1] - u[0]) / h / h / h;
		b = (2.0 * du[1] + du[0]) / h - 3.0 * (u[1] - u[0]) / h / h;
		c = du[1];
		x = -c_0 * tau;
	} else {
		a = (du[1] + du[0]) / h / h - 2.0 * (u[1] - u[0]) / h / h / h;
		b = -(2.0 * du[0] + du[1]) / h + 3.0 * (u[1] - u[0]) / h / h;
		c = du[0];
		x = c_0 * tau;
	}
	double val = 3.0 * a * x * x + 2.0 * b * x + c;
	return val;
}

void fillStencilValues(int ind) {
	unsigned int j;
	int ind_new_l, ind_new_r;
	for (j = 0; j < S + 1; j++) {
		ind_new_l = ind + stencil_l[j];
		if (ind_new_l < 0) {
			stencil_values_w2[j] = W2_c[ind_new_l + N];
			stencil_Dvalues_w2[j] = dW2_c[ind_new_l + N];
		} else {
			stencil_values_w2[j] = W2_c[ind_new_l];
			stencil_Dvalues_w2[j] = dW2_c[ind_new_l];
		}
		ind_new_r = ind + stencil_r[j];
		if (ind_new_r > N - 1) {
			stencil_values_w1[j] = W1_c[ind_new_r - N];
			stencil_Dvalues_w1[j] = dW1_c[ind_new_r - N];
		} else {
			stencil_values_w1[j] = W1_c[ind_new_r];
			stencil_Dvalues_w1[j] = dW1_c[ind_new_r];
		}
	}
}

void debugPrint(void) {
	unsigned int ind;
#define eps 0.1
	printf("W1:\n");
	for (ind = 0; ind < N; ind++)
		if ((W1_c[ind] < eps) && (W1_c[ind] > -eps))
			printf("_");
		else
			printf("|");
	printf("\n");
	printf("W2:\n");
	for (ind = 0; ind < N; ind++)
		if ((W2_c[ind] < eps) && (W2_c[ind] > -eps))
			printf("_");
		else
			printf("|");
	printf("\n");
}
