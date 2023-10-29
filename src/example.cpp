#define ARMA_64BIT_WORD
#include <RcppArmadillo.h>

using namespace arma;

//' Example Function to Illustrate OpenMP Use
//'
//' This example is taken directly from the Armadillo documentation.
//' When built with OpenMP support, and running on a system with
//' multiple cores, R will likely complain about 'CPU time > 2 elapsed
//' time'. However, when environment variable 'SHOWME' is set to 'yes'
//' we throttle and no warning appears.
//'
//' @examples
//' if (Sys.getenv("SHOWME", "") == "yes")
//'    set_number_of_omp_threads(1)
//' openmp_example()
//'
// [[Rcpp::export]]
void openmp_example() {
    // generate sparse matrix
    sp_mat A = sprandu<sp_mat>(1500, 1500, 0.1);
    sp_mat B = A.t()*A;

    vec eigval;
    mat eigvec;

    eigs_sym(eigval, eigvec, B, 5);  // find 5 eigenvalues/eigenvectors

    eigs_opts opts;
    opts.maxiter = 10000;            // increase max iterations to 10000

    eigs_sym(eigval, eigvec, B, 5, "lm", opts);
}

//' Report Maximum Number of Threads
//'
// [[Rcpp::export]]
int get_number_of_omp_threads() {
#ifdef _OPENMP
    return omp_get_max_threads();
#else
    return 1;
#endif
}

//' Set Maximum Number of Threads
//'
//' @param n Number of threads to be set
// [[Rcpp::export]]
void set_number_of_omp_threads(int n) {
#ifdef _OPENMP
    return omp_set_num_threads(n);
#endif
}
