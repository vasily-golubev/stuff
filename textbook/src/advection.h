/* Number of nodes in mesh. */
#define N 100

/*
 * U_c contains current time layer and U_n - next.
 */
double U_c[N], U_n[N];

/* Set initial conditions. */
void initialize(void);
/* Do 1 step of numerical scheme. */
void singleStep(void);
/* Return interpolated value based on stencil values.  */
double interpolatedValue(double *u);
/* Fill values in stencil nodes. */
void fillStencilValues(int ind);
/* Print simple representation of current U values. */
void debugPrint(void);
/* Save current U values in file for Gnuplot processing. */
void debugGnuplot(void);
