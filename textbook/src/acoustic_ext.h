/* Number of nodes in mesh. */
#define N 100

/*
 * *_c contains current time layer and *_n - next,
 * and d* - derivatives.
 * Currently solve in Riemann invariants.
 */
double W1_c[N], W2_c[N], W1_n[N], W2_n[N];
double dW1_c[N], dW2_c[N], dW1_n[N], dW2_n[N];

/* Set initial conditions. */
void initialize(void);
/* Do 1 step of numerical scheme. */
void singleStep(void);
/* Return interpolated u value based on stencil values.  */
double interpolatedValue(double *u, double *du, int dir);
/* Return interpolated du value based on stencil values.  */
double interpolatedDValue(double *u, double *du, int dir);
/* Fill values in stencil nodes. */
void fillStencilValues(int ind);
/* Print simple representation of current * values. */
void debugPrint(void);
