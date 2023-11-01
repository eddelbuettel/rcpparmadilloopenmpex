
## RcppArmadilloOpenMPEx:  Thread Throttling Example

### Synopsis

CRAN, for better or worse, insists on tests and example using only two cores
(as they need to test thousands of packages, they do so concurrently and need
to limit the load of these tests and examples running in parallel).

This repo contains a simple example package which, when tested as CRAN does,
_e.g., via

```sh
$ _R_CHECK_TIMINGS_=2 \
  _R_CHECK_EXAMPLE_TIMING_CPU_TO_ELAPSED_THRESHOLD_=2 \
  R CMD check --as-cran --no-manual RcppArmadilloOpenMPEx_0.0.1.tar.gz
```

displays the dreaded warning from the one included example (that we borrowed
from the [Armadillo
documentation](https://arma.sourceforge.net/docs.html#eigs_sym)):

```
* checking examples ... NOTE
Examples with CPU time > 2 times elapsed time
                user system elapsed ratio
openmp_example 3.323  0.033   0.629 5.335
* checking for unstated dependencies in ‘tests’ ... OK
```

You can verify this on any system with both OpenMP installed, and more than
two cores. This may as always be easiest on a Linux system, and for example
the [r2u](https://eddelbuettel.github.io/r2u) container `rocker/r2u` will
works fine, and was used here.

Now, if we also set the environment variable `SHOWME` to `yes`, then the
example in this package [will throttle the count
down](https://github.com/eddelbuettel/rcpparmadilloopenmpex/blob/b369c5518b01df2c0c9b9f56e22fe499cd1f3c94/src/example.cpp#L15-L16).

So running

```sh
$ SHOWME=yes \
  _R_CHECK_TIMINGS_=2 \
  _R_CHECK_EXAMPLE_TIMING_CPU_TO_ELAPSED_THRESHOLD_=2 \
  R CMD check --as-cran --no-manual RcppArmadilloOpenMPEx_0.0.1.tar.gz
```

results in a run without a warning as desired:

```
* checking examples ... OK
* checking for unstated dependencies in ‘tests’ ... OK
```

Starting with release 0.12.6.6.0,  more convenient setters are available in
[RcppArmadillo](https://github.com/RcppCore/RcppArmadillo) itself.
An example is provided by [its `fastLm`
example](https://github.com/RcppCore/RcppArmadillo/blob/779d353e48e9cae3f0b8aa02b9e3080d75e4a605/man/fastLm.Rd#L72-L98).
The underlying function (which could be copied) is a calling `omp_set_num_threads()`:

```c++
//' Set Maximum Number of Threads
//'
//' @param n Number of threads to be set
// [[Rcpp::export]]
void set_number_of_omp_threads(int n) {
#ifdef _OPENMP
    omp_set_num_threads(n);
#else
    (void)(n);                  // prevent unused variable warning
#endif
}
```

In [RcppArmadillo](https://github.com/RcppCore/RcppArmadillo), we combine
this with a package-local 'cacheing' of count of threads set at package load
to provide a pair of functions to _set_ and then _reset_ the count -- see the
[fastLm example](https://github.com/RcppCore/RcppArmadillo/blob/779d353e48e9cae3f0b8aa02b9e3080d75e4a605/man/fastLm.Rd#L72-L98).

### Author

Dirk Eddelbuettel

### License

GPL (>= 2)
