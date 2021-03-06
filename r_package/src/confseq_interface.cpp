#include <algorithm>

//#include <RcppCommon.h>
#include <Rcpp.h>

//using namespace Rcpp;

#include "uniform_boundaries.h"

using namespace std::placeholders;

Rcpp::NumericVector mapply2(std::function<double(double, double)> fn,
                            Rcpp::NumericVector arg1,
                            Rcpp::NumericVector arg2) {
  int n = std::max(arg1.size(), arg2.size());
  auto arg1_recycled = Rcpp::rep_len(arg1, n);
  auto arg2_recycled = Rcpp::rep_len(arg2, n);
  Rcpp::NumericVector out(n);
  for (int i = 0; i < n; i++) {
    out[i] = fn(arg1_recycled[i], arg2_recycled[i]);
  }
  return out;
}

Rcpp::NumericVector mapply3(std::function<double(double, double, double)> fn,
                            Rcpp::NumericVector arg1,
                            Rcpp::NumericVector arg2,
                            Rcpp::NumericVector arg3) {
  int n = std::max({arg1.size(), arg2.size(), arg3.size()});
  auto arg1_recycled = Rcpp::rep_len(arg1, n);
  auto arg2_recycled = Rcpp::rep_len(arg2, n);
  auto arg3_recycled = Rcpp::rep_len(arg3, n);
  Rcpp::NumericVector out(n);
  for (int i = 0; i < n; i++) {
    out[i] = fn(arg1_recycled[i], arg2_recycled[i], arg3_recycled[i]);
  }
  return out;
}

//' Logarithm of mixture supermartingale for the one- or two-sided normal
//' mixture.
//' @param s value of the underlying martingale
//' @param v intrinsic time value
//' @param v_opt intrinsic time value for which the corresponding boundary is
//'   optimized
//' @param alpha_opt alpha for which the corresponding boundary is optimized
//' @param is_one_sided if FALSE, use the two-sided normal mixture
//' @examples
//' normal_log_mixture(10, 100, 100)
//' normal_log_mixture(c(10, 20), c(100, 400), 100, is_one_sided=FALSE)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector normal_log_mixture(
    const Rcpp::NumericVector s, const Rcpp::NumericVector v,
    const double v_opt, const double alpha_opt=0.05,
    const bool is_one_sided=true) {
  return mapply2(std::bind(confseq::normal_log_mixture, _1, _2, v_opt,
                           alpha_opt, is_one_sided),
                 s, v);
}

//' One- or two-sided normal mixture uniform boundary.
//' @param v intrinsic time value
//' @param alpha the error / crossing probability of the boundary
//' @param v_opt intrinsic time value for which the boundary is optimized
//' @param alpha_opt alpha for which the boundary is optimized
//' @param is_one_sided if FALSE, use the two-sided normal mixture
//' @examples
//' normal_mixture_bound(c(100, 200), .05, 100)
//' normal_mixture_bound(100, c(.05, .01), 100)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector normal_mixture_bound(
    const Rcpp::NumericVector v, const Rcpp::NumericVector alpha,
    const double v_opt, const double alpha_opt=0.05,
    const bool is_one_sided=true) {
  return mapply2(std::bind(confseq::normal_mixture_bound, _1, _2, v_opt,
                           alpha_opt, is_one_sided),
                 v, alpha);
}

//' Logarithm of mixture supermartingale for the gamma-exponential mixture.
//' @inherit normal_log_mixture
//' @param c sub-exponential scale parameter
//' @examples
//' gamma_exponential_log_mixture(c(10, 20), c(100, 400), 100, 2)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector gamma_exponential_log_mixture(
    const Rcpp::NumericVector s, const Rcpp::NumericVector v,
    const double v_opt, const double c, const double alpha_opt=0.05) {
  return mapply2(std::bind(confseq::gamma_exponential_log_mixture, _1, _2,
                           v_opt, c, alpha_opt),
                 s, v);
}

//' Gamma-exponential uniform boundary.
//' @inherit normal_mixture_bound
//' @param c sub-exponential scale parameter
//' @examples
//' gamma_exponential_mixture_bound(c(100, 200), .05, 100, 2)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector gamma_exponential_mixture_bound(
    const Rcpp::NumericVector v, const Rcpp::NumericVector alpha,
    const double v_opt, const double c, const double alpha_opt=0.05) {
  return mapply2(std::bind(confseq::gamma_exponential_mixture_bound, _1, _2,
                           v_opt, c, alpha_opt),
                 v, alpha);
}

//' Logarithm of mixture supermartingale for the gamma-Poisson mixture.
//' @inherit normal_log_mixture
//' @param c sub-Poisson scale parameter
//' @examples
//' gamma_poisson_log_mixture(c(10, 20), c(100, 400), 100, 2)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector gamma_poisson_log_mixture(
    const Rcpp::NumericVector s, const Rcpp::NumericVector v,
    const double v_opt, const double c, const double alpha_opt=0.05) {
  return mapply2(std::bind(confseq::gamma_poisson_log_mixture, _1, _2, v_opt,
                           c, alpha_opt),
                 s, v);
}

//' Gamma-Poisson uniform boundary.
//' @inherit normal_mixture_bound
//' @param c sub-Poisson scale parameter
//' @examples
//' gamma_poisson_mixture_bound(c(100, 200), .05, 100, 2)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector gamma_poisson_mixture_bound(
    const Rcpp::NumericVector v, const Rcpp::NumericVector alpha,
    const double v_opt, const double c, const double alpha_opt=0.05) {
  return mapply2(std::bind(confseq::gamma_poisson_mixture_bound, _1, _2, v_opt,
                           c, alpha_opt),
                 v, alpha);
}

//' Logarithm of mixture supermartingale for the beta-binomial mixture.
//' @inherit normal_log_mixture
//' @param g lower sub-Bernoulli range parameter
//' @param h upper sub-Bernoulli range parameter
//' @param is_one_sided if FALSE, use the two-sided beta-binomial mixture
//' @examples
//' beta_binomial_log_mixture(c(10, 20), c(100, 400), 100, .2, .8)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector beta_binomial_log_mixture(
    const Rcpp::NumericVector s, const Rcpp::NumericVector v,
    const double v_opt, const double g, const double h,
    const double alpha_opt=0.05, const bool is_one_sided=true) {
  return mapply2(std::bind(confseq::beta_binomial_log_mixture, _1, _2, v_opt,
                           g, h, alpha_opt, is_one_sided),
                 s, v);
}

//' Beta-binomial uniform boundary.
//' @inherit normal_mixture_bound
//' @inherit beta_binomial_log_mixture
//' @examples
//' beta_binomial_mixture_bound(c(100, 200), .05, 100, .2, .8)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector beta_binomial_mixture_bound(
    const Rcpp::NumericVector v, const Rcpp::NumericVector alpha,
    const double v_opt, const double g, const double h,
    const double alpha_opt=0.05, const bool is_one_sided=true) {
  return mapply2(std::bind(confseq::beta_binomial_mixture_bound, _1, _2, v_opt,
                           g, h, alpha_opt, is_one_sided),
                 v, alpha);
}

//' Polynomial stitched uniform boundary.
//' @inherit normal_mixture_bound
//' @param v_min intrinsic time for which boundary is optimized
//' @param c sub-gamma scale parameter
//' @param s controls how crossing probability is distribted over epochs
//' @param eta controls the spacing of epochs
//' @examples
//' poly_stitching_bound(c(100, 200), .05, 100)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector poly_stitching_bound(
    const Rcpp::NumericVector v, const Rcpp::NumericVector alpha,
    const double v_min, const double c=0, const double s=1.4,
    const double eta=2) {
  return mapply2(
      std::bind(confseq::poly_stitching_bound, _1, _2, v_min, c, s, eta),
      v, alpha);
}

//' Empirical process finite LIL bound.
//'
//' Based on Theorem 2 of the quantile paper. Bound has the form `A
//' sqrt((loglog(et / t_min) + C) / t)`, and is valid only for t >= t_min. C is
//' chosen to achieve the desired error probability `alpha`.
//'
//' This bound controls the deviations of the empirical CDF from the true CDF
//' uniformly over x and time, or yields a confidence sequences uniform over
//' quantiles and time (Corollary 2 of the quantile paper).
//'
//' @param t number of samples observed
//' @param alpha error / crossing probability of the boundary
//' @param t_min first time at which boundary is valid
//' @param A leading constant in the bound
//' @examples
//' empirical_process_lil_bound(1000, .05, 100)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector empirical_process_lil_bound(
    const Rcpp::NumericVector t, const Rcpp::NumericVector alpha,
    const double t_min, const double A=0.85) {
  return mapply2(
      std::bind(confseq::empirical_process_lil_bound, _1, _2, t_min, A),
      t, alpha);
}

//' "Double stitching" bound (Theorem 3 of the quantile paper).
//'
//' Yields a confidence sequence uniform over quantiles and time.
//'
//' @param quantile_p designates which quantile we wish to bound
//' @param t number of samples observed
//' @param alpha error / crossing probability of the boundary
//' @param t_opt sample size for which the bound is optimized
//' @param delta controls the fineness of the quantile grid used in construction
//'   of the bound
//' @param s controls how crossing probability is distribted over epochs
//' @param eta controls the spacing of epochs
//' @examples
//' double_stitching_bound(c(.1, .5, .9), 1000, .05, 100)
//' @export
// [[Rcpp::export]]
Rcpp::NumericVector double_stitching_bound(
    const Rcpp::NumericVector quantile_p, const Rcpp::NumericVector t,
    const Rcpp::NumericVector alpha, const double t_opt,
    const double delta=0.5, const double s=1.4, const double eta=2) {
  return mapply3(std::bind(confseq::double_stitching_bound, _1, _2, _3, t_opt,
                           delta, s, eta),
                 quantile_p, t, alpha);
}

//' Confidence sequence for [0, 1]-bounded distributions.
//'
//' This function returns confidence bounds for the mean of a Bernoulli
//' distribution, or more generally, any distribution with support in the unit
//' interval [0, 1]. (This applies to any bounded distribution after rescaling.)
//' The confidence bounds form a confidence sequence, so are guaranteed to cover
//' the true mean uniformly over time with probability 1 - `alpha`.
//'
//' @param num_successes number of "successful" Bernoulli trials seen so far, or
//'   more generally, sum of observed outcomes
//' @param num_trials total number of observations seen so far
//' @param alpha 1 - confidence level
//' @param t_opt sample size for which confidence sequence is optimized
//' @param alpha_opt 1 - confidence level for which confidence is optimized
//' @return a list with named elements `lower` and `upper` giving lower and
//'   upper confidence bounds, respectively
//' @examples
//' bernoulli_confidence_interval(c(50, 500, 5000), c(100, 1000, 10000), .05,
//'                               1000)
//' @export
// [[Rcpp::export]]
Rcpp::List bernoulli_confidence_interval(
    const Rcpp::NumericVector num_successes,
    const Rcpp::NumericVector num_trials, const double alpha,
    const double t_opt, const double alpha_opt=0.05) {
  int n = std::max(num_successes.size(), num_trials.size());
  auto num_successes_recycled = Rcpp::rep_len(num_successes, n);
  auto num_trials_recycled = Rcpp::rep_len(num_trials, n);
  Rcpp::NumericVector lower_bounds(n);
  Rcpp::NumericVector upper_bounds(n);
  for (int i = 0; i < n; i++) {
    auto bounds = confseq::bernoulli_confidence_interval(
        num_successes_recycled[i], num_trials_recycled[i], alpha, t_opt,
        alpha_opt);
    lower_bounds[i] = bounds.first;
    upper_bounds[i] = bounds.second;
  }
  return Rcpp::List::create(Rcpp::_["lower"]=lower_bounds,
                            Rcpp::_["upper"]=upper_bounds);
}

//' Two-sample, sequential test of equal quantiles.
//'
//' This function tests the null that two i.i.d. samples are drawn from
//' populations with the same `quantile_p` quantile. It returns a two-tailed,
//' always-valid p-value.
//'
//' @param a_values values observed so far from the first sample
//' @param b_values values observed so far from the second samples
//' @param quantile_p designates which quantile we wish to test
//' @param t_opt sample size for which test is optimized
//' @param alpha_opt level for which test is optimized
//' @examples
//' quantile_ab_p_value(1:1000, 86:1085, .5, 100)
//' @export
// [[Rcpp::export]]
double quantile_ab_p_value(const Rcpp::NumericVector a_values,
                           const Rcpp::NumericVector b_values,
                           const double quantile_p, const int t_opt,
                           const double alpha_opt=0.05) {
  auto a_os = std::make_unique<confseq::StaticOrderStatistics>(
      a_values.begin(), a_values.end());
  auto b_os = std::make_unique<confseq::StaticOrderStatistics>(
      b_values.begin(), b_values.end());
  confseq::QuantileABTest test(quantile_p, t_opt, alpha_opt, std::move(a_os),
                               std::move(b_os));
  return test.p_value();
}
