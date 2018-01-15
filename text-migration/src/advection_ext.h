/* Number of nodes in mesh. */
#define N 100

/*
 * U_c contains current time layer and U_n - next,
 * and dU - derivatives.
 */
double U_c[N], U_n[N];
double dU_c[N], dU_n[N];

/* Set initial conditions. */
void initialize(void);
/* Do 1 step of numerical scheme. */
void singleStep(void);
/* Return interpolated U value based on stencil values.  */
double interpolatedValue(double *u, double *du);
/* Return interpolated dU value based on stencil values.  */
double interpolatedDValue(double *u, double *du);
/* Fill values in stencil nodes. */
void fillStencilValues(int ind);
/* Print simple representation of current U values. */
void debugPrint(void);
/* Print simple representation of current U values. */
void debugGnuplot(void);
