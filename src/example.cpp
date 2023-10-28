#define ARMA_64BIT_WORD
#include <RcppArmadillo.h>

using namespace arma;

//' Example Function
//'
//' @examples
//' ncores <- Sys.getenv("OMP_NUM_THREADS", 2)
//' Sys.setenv("OMP_NUM_THREADS"=2)
//' example()
//' Sys.setenv("OMP_NUM_THREADS"=ncores)
//'
// [[Rcpp::export]]
void example() {
    // generate sparse matrix
    sp_mat A = sprandu<sp_mat>(2000, 2000, 0.1);
    sp_mat B = A.t()*A;

    vec eigval;
    mat eigvec;

    eigs_sym(eigval, eigvec, B, 5);  // find 5 eigenvalues/eigenvectors

    eigs_opts opts;
    opts.maxiter = 10000;            // increase max iterations to 10000

    eigs_sym(eigval, eigvec, B, 5, "lm", opts);
}
