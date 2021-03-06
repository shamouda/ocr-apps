#ifndef CHEBYSHEV_DEGREE
#define CHEBYSHEV_DEGREE 4
#endif
#define h2inv ((double)H2INV) \

#define rstream_calculate_Dinv()                                              \
(                                                                             \
  1.0 / (a*alpha[k][j][i] - b*h2inv*(                                         \
           + beta_i[k][j][i]  *( valid[k][j][i-1] - 2.0 )                     \
           + beta_j[k][j][i]  *( valid[k][j-1][i] - 2.0 )                     \
           + beta_k[k][j][i]  *( valid[k-1][j][i] - 2.0 )                     \
           + beta_i[k][j][i+1]*( valid[k][j][i+1] - 2.0 )                     \
           + beta_j[k][j+1][i]*( valid[k][j+1][i] - 2.0 )                     \
           + beta_k[k+1][j][i]*( valid[k+1][j][i] - 2.0 )                     \
        ))                                                                    \
)
#define rstream_Dinv_ijk() rstream_calculate_Dinv()
#define rstream_apply_op_ijk(x)                                                               \
  (                                                                                           \
    a*alpha[k][j][i]*x[k][j][i] - b*h2inv*(                                                   \
      + beta_i[k][j][i]  *( valid[k][j][i-1]*( x[k][j][i] + x[k][j][i-1] ) - 2.0*x[k][j][i] ) \
      + beta_j[k][j][i]  *( valid[k][j-1][i]*( x[k][j][i] + x[k][j-1][i] ) - 2.0*x[k][j][i] ) \
      + beta_k[k][j][i]  *( valid[k-1][j][i]*( x[k][j][i] + x[k-1][j][i] ) - 2.0*x[k][j][i] ) \
      + beta_i[k][j][i+1]*( valid[k][j][i+1]*( x[k][j][i] + x[k][j][i+1] ) - 2.0*x[k][j][i] ) \
      + beta_j[k][j+1][i]*( valid[k][j+1][i]*( x[k][j][i] + x[k][j+1][i] ) - 2.0*x[k][j][i] ) \
      + beta_k[k+1][j][i]*( valid[k+1][j][i]*( x[k][j][i] + x[k+1][j][i] ) - 2.0*x[k][j][i] ) \
    )                                                                                         \
  )

        // These are the original statements for c1 and c2. For reference data
        // 0 <= ss < CHEBYSHEV_DEGREE at all times. Thus the R-Stream unfriendly
        // mod in the array access function is removed.
        //
        //const double c1 = chebyshev_c1[ss%CHEBYSHEV_DEGREE];                  \
        //const double c2 = chebyshev_c2[ss%CHEBYSHEV_DEGREE];                  \


// if (ss % 2 == 0)
#define KERNEL_1                                                              \
do {                                                                          \
double a = 0.0;                                                               \
double b = 1.0;                                                               \
const double c1 = chebyshev_c1[ss];                                           \
const double c2 = chebyshev_c2[ss];                                           \
for(k=0-ghostsToOperateOn;k<dim+ghostsToOperateOn;k++){                       \
    for(j=0-ghostsToOperateOn;j<dim+ghostsToOperateOn;j++){                   \
        for(i=0-ghostsToOperateOn;i<dim+ghostsToOperateOn;i++){               \
            x_np1_0[k][j][i] = x_np1_1[k][j][i] +                             \
                           c1*(x_np1_1[k][j][i]-x_np1_0[k][j][i]) +           \
                           c2*rstream_Dinv_ijk()*                             \
                           (rhs[k][j][i]-rstream_apply_op_ijk(x_np1_1));      \
        }}}                                                                   \
} while (0)                                                                   \

// else if (ss % 2 != 0)
#define KERNEL_2                                                              \
do {                                                                          \
double a = 0.0;                                                               \
double b = 1.0;                                                               \
const double c1 = chebyshev_c1[ss];                                           \
const double c2 = chebyshev_c2[ss];                                           \
for(k=0-ghostsToOperateOn;k<dim+ghostsToOperateOn;k++){                       \
    for(j=0-ghostsToOperateOn;j<dim+ghostsToOperateOn;j++){                   \
        for(i=0-ghostsToOperateOn;i<dim+ghostsToOperateOn;i++){               \
            x_np1_1[k][j][i] = x_np1_0[k][j][i] +                             \
                           c1*(x_np1_0[k][j][i]-x_np1_1[k][j][i]) +           \
                           c2*rstream_Dinv_ijk()*                             \
                           (rhs[k][j][i]-rstream_apply_op_ijk(x_np1_0));      \
        }}}                                                                   \
}                                                                             \
while (0)

#pragma rstream map
void cheby_mappable_kernel4_1(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[4][4][4],       double x_np1_1[4][4][4],
        const double rhs[4][4][4],    const double alpha[4][4][4],
        const double beta_i[4][4][4],  const double beta_j[4][4][4],
        const double beta_k[4][4][4],  const double Dinv[4][4][4],
        const double valid[4][4][4]) {
    int i; int j; int k;
    KERNEL_1;
}

#pragma rstream map
void cheby_mappable_kernel4_2(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[4][4][4],       double x_np1_1[4][4][4],
        const double rhs[4][4][4],    const double alpha[4][4][4],
        const double beta_i[4][4][4],  const double beta_j[4][4][4],
        const double beta_k[4][4][4],  const double Dinv[4][4][4],
        const double valid[4][4][4]) {
    int i; int j; int k;
    KERNEL_2;
}

#pragma rstream map
void cheby_mappable_kernel6_1(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[6][6][6],       double x_np1_1[6][6][6],
        const double rhs[6][6][6],    const double alpha[6][6][6],
        const double beta_i[6][6][6],  const double beta_j[6][6][6],
        const double beta_k[6][6][6],  const double Dinv[6][6][6],
        const double valid[6][6][6]) {
    int i; int j; int k;
    KERNEL_1;
}

#pragma rstream map
void cheby_mappable_kernel6_2(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[6][6][6],       double x_np1_1[6][6][6],
        const double rhs[6][6][6],    const double alpha[6][6][6],
        const double beta_i[6][6][6],  const double beta_j[6][6][6],
        const double beta_k[6][6][6],  const double Dinv[6][6][6],
        const double valid[6][6][6]) {
    int i; int j; int k;
    KERNEL_2;
}

#pragma rstream map
void cheby_mappable_kernel10_1(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[10][10][10],       double x_np1_1[10][10][10],
        const double rhs[10][10][10],    const double alpha[10][10][10],
        const double beta_i[10][10][10],  const double beta_j[10][10][10],
        const double beta_k[10][10][10],  const double Dinv[10][10][10],
        const double valid[10][10][10]) {
    int i; int j; int k;
    KERNEL_1;
}

#pragma rstream map
void cheby_mappable_kernel10_2(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[10][10][10],       double x_np1_1[10][10][10],
        const double rhs[10][10][10],    const double alpha[10][10][10],
        const double beta_i[10][10][10],  const double beta_j[10][10][10],
        const double beta_k[10][10][10],  const double Dinv[10][10][10],
        const double valid[10][10][10]) {
    int i; int j; int k;
    KERNEL_2;
}

#pragma rstream map
void cheby_mappable_kernel18_1(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[18][18][18],       double x_np1_1[18][18][18],
        const double rhs[18][18][18],    const double alpha[18][18][18],
        const double beta_i[18][18][18],  const double beta_j[18][18][18],
        const double beta_k[18][18][18],  const double Dinv[18][18][18],
        const double valid[18][18][18]) {
    int i; int j; int k;
    KERNEL_1;
}

#pragma rstream map
void cheby_mappable_kernel18_2(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[18][18][18],       double x_np1_1[18][18][18],
        const double rhs[18][18][18],    const double alpha[18][18][18],
        const double beta_i[18][18][18],  const double beta_j[18][18][18],
        const double beta_k[18][18][18],  const double Dinv[18][18][18],
        const double valid[18][18][18]) {
    int i; int j; int k;
    KERNEL_2;
}

#pragma rstream map
void cheby_mappable_kernel34_1(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[34][34][34],       double x_np1_1[34][34][34],
        const double rhs[34][34][34],    const double alpha[34][34][34],
        const double beta_i[34][34][34],  const double beta_j[34][34][34],
        const double beta_k[34][34][34],  const double Dinv[34][34][34],
        const double valid[34][34][34]) {
    int i; int j; int k;
    KERNEL_1;
}

#pragma rstream map
void cheby_mappable_kernel34_2(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[34][34][34],       double x_np1_1[34][34][34],
        const double rhs[34][34][34],    const double alpha[34][34][34],
        const double beta_i[34][34][34],  const double beta_j[34][34][34],
        const double beta_k[34][34][34],  const double Dinv[34][34][34],
        const double valid[34][34][34]) {
    int i; int j; int k;
    KERNEL_2;
}

#pragma rstream map
void cheby_mappable_kernel66_1(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[66][66][66],       double x_np1_1[66][66][66],
        const double rhs[66][66][66],    const double alpha[66][66][66],
        const double beta_i[66][66][66],  const double beta_j[66][66][66],
        const double beta_k[66][66][66],  const double Dinv[66][66][66],
        const double valid[66][66][66]) {
    int i; int j; int k;
    KERNEL_1;
}

#pragma rstream map
void cheby_mappable_kernel66_2(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[66][66][66],       double x_np1_1[66][66][66],
        const double rhs[66][66][66],    const double alpha[66][66][66],
        const double beta_i[66][66][66],  const double beta_j[66][66][66],
        const double beta_k[66][66][66],  const double Dinv[66][66][66],
        const double valid[66][66][66]) {
    int i; int j; int k;
    KERNEL_2;
}

#pragma rstream map
void cheby_mappable_kernel130_1(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[130][130][130],       double x_np1_1[130][130][130],
        const double rhs[130][130][130],    const double alpha[130][130][130],
        const double beta_i[130][130][130],  const double beta_j[130][130][130],
        const double beta_k[130][130][130],  const double Dinv[130][130][130],
        const double valid[130][130][130]) {
    int i; int j; int k;
    KERNEL_1;
}

#pragma rstream map
void cheby_mappable_kernel130_2(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[130][130][130],       double x_np1_1[130][130][130],
        const double rhs[130][130][130],    const double alpha[130][130][130],
        const double beta_i[130][130][130],  const double beta_j[130][130][130],
        const double beta_k[130][130][130],  const double Dinv[130][130][130],
        const double valid[130][130][130]) {
    int i; int j; int k;
    KERNEL_2;
}

#pragma rstream map
void cheby_mappable_kernel258_1(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[258][258][258],       double x_np1_1[258][258][258],
        const double rhs[258][258][258],    const double alpha[258][258][258],
        const double beta_i[258][258][258],  const double beta_j[258][258][258],
        const double beta_k[258][258][258],  const double Dinv[258][258][258],
        const double valid[258][258][258]) {
    int i; int j; int k;
    KERNEL_1;
}

#pragma rstream map
void cheby_mappable_kernel258_2(
        int H2INV,
        int s, int ss, int ghosts, int ghostsToOperateOn, int dim,
        const double chebyshev_c1[CHEBYSHEV_DEGREE],
        const double chebyshev_c2[CHEBYSHEV_DEGREE],
        double x_np1_0[258][258][258],       double x_np1_1[258][258][258],
        const double rhs[258][258][258],    const double alpha[258][258][258],
        const double beta_i[258][258][258],  const double beta_j[258][258][258],
        const double beta_k[258][258][258],  const double Dinv[258][258][258],
        const double valid[258][258][258]) {
    int i; int j; int k;
    KERNEL_2;
}
#undef h2inv
