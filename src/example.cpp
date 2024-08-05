#include <RcppArmadillo.h>
#include <spdl.h>               		// has to come after RcppArmadillo as former checks for Rcpp

//using namespace arma;

//' Example Function to Illustrate OpenMP Use
//'
//' This example is taken directly from the Armadillo documentation.
//' When built with OpenMP support, and running on a system with
//' multiple cores, R will likely complain about 'CPU time > 2 elapsed
//' time'. However, when environment variable 'SHOWME' is set to 'yes'
//' we throttle and no warning appears.
//'
//' For the example size used here, we see two threads (a little) faster
//' in elapsed 'wall time' than one. For larger sizes more threads gain more.
//'
//' This is simply not the strongest example of OpenMP benefits but simply
//' an 'existence proof' for a working OpenMP configuration.
//'
//' @examples
//' if (Sys.getenv("SHOWME", "") == "yes")
//'    set_number_of_omp_threads(1)
//' openmp_example()
//'
// [[Rcpp::export]]
void openmp_example() {
    spdlog::stopwatch sw;
    // generate sparse matrix
    arma::sp_mat A = arma::sprandu<arma::sp_mat>(1500, 1500, 0.05);
    arma::sp_mat B = A.t()*A;

    arma::vec eigval;
    arma::mat eigvec;

    arma::eigs_sym(eigval, eigvec, B, 5);  // find 5 eigenvalues/eigenvectors
    spdl::warn("[openmp_example] time elapsed {}", sw);

    arma::eigs_opts opts;
    opts.maxiter = 10000;            // increase max iterations to 10000

    arma::eigs_sym(eigval, eigvec, B, 5, "lm", opts);
    spdl::warn("[openmp_example] time elapsed {}", sw);
    //eigval.print("eigval");
    //eigvec.print("eigvec");
}

//' Report Maximum Number of Threads
//'
// [[Rcpp::export]]
int get_number_of_omp_threads() {
#ifdef _OPENMP
    spdl::warn("[get_number_of_omp_threads] seeing {}", omp_get_max_threads());
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
    omp_set_num_threads(n);
#else
    (void)(n);  					// prevent unused variable warning
#endif
}
