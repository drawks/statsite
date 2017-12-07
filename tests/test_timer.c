#include <check.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <math.h>
#include "timer.h"

START_TEST(test_timer_init_and_destroy)
{
    timer t;
    double quants[] = {0.5, 0.90, 0.99, 0.999};
    int res = init_timer(0.01, 200, (double*)&quants, 3, &t);
    fail_unless(res == 0);

    res = destroy_timer(&t);
    fail_unless(res == 0);
}
END_TEST

START_TEST(test_timer_init_add_destroy)
{
    timer t;
    double quants[] = {0.5, 0.90, 0.99, 0.999};
    int res = init_timer(0.01, 200, (double*)&quants, 3, &t);
    fail_unless(res == 0);

    fail_unless(timer_add_sample(&t, 100, 1.0) == 0);
    fail_unless(timer_count(&t) == 1);
    fail_unless(timer_min(&t) == 100);
    fail_unless(timer_min_tdigest(&t) == 100);
    fail_unless(timer_max(&t) == 100);
    fail_unless(timer_max_tdigest(&t) == 100);
    fail_unless(timer_mean(&t) == 100);
    fail_unless(timer_query(&t, 0.5) == 100);
    fail_unless(timer_quantile_tdigest(&t, 0.999) == 100);

    res = destroy_timer(&t);
    fail_unless(res == 0);
}
END_TEST

START_TEST(test_timer_add_loop)
{
    timer t;
    double quants[] = {0.5, 0.90, 0.99, 0.999};
    int res = init_timer(0.01, 200, (double*)&quants, 3, &t);
    fail_unless(res == 0);

    for (int i=1; i<=100; i++)
        fail_unless(timer_add_sample(&t, i, 1.0) == 0);

    fail_unless(timer_count(&t) == 100);
    fail_unless(timer_min(&t) == 1);
    fail_unless(timer_max(&t) == 100);
    fail_unless(timer_min_tdigest(&t) == 1);
    fail_unless(timer_max_tdigest(&t) == 100);
    fail_unless(timer_mean(&t) == 50.5);
    fail_unless(timer_query(&t, 0.5) == 50);
    fail_unless(timer_quantile_tdigest(&t, 0.90) >= 89 && timer_quantile_tdigest(&t, 0.90) <= 91);
    fail_unless(timer_quantile_tdigest(&t, 0.99) >= 98 && timer_quantile_tdigest(&t, 0.99) <= 100);
    fail_unless(timer_quantile_tdigest(&t, 0.999) >= 99 && timer_quantile_tdigest(&t, 0.999) <= 100);

    res = destroy_timer(&t);
    fail_unless(res == 0);
}
END_TEST

START_TEST(test_timer_sample_rate)
{
  timer t;
  double quants[] = {0.5, 0.90, 0.99, 0.999};
  int res = init_timer(0.01, 200, (double*)&quants, 3, &t);
  fail_unless(res == 0);

  for (int i=1; i<=100; i++)
      fail_unless(timer_add_sample(&t, i, 0.5) == 0);

  fail_unless(timer_count(&t) == 200);
  fail_unless(timer_min(&t) == 1);
  fail_unless(timer_max(&t) == 100);
  fail_unless(timer_min_tdigest(&t) == 1);
  fail_unless(timer_max_tdigest(&t) == 100);
  fail_unless(timer_mean(&t) == 50.5);
  fail_unless(timer_query(&t, 0.5) == 50);
  fail_unless(timer_quantile_tdigest(&t, 0.90) >= 89 && timer_quantile_tdigest(&t, 0.90) <= 91);
  fail_unless(timer_quantile_tdigest(&t, 0.99) >= 98 && timer_quantile_tdigest(&t, 0.99) <= 100);
  fail_unless(timer_quantile_tdigest(&t, 0.999) >= 99 && timer_quantile_tdigest(&t, 0.999) <= 100);

  res = destroy_timer(&t);
  fail_unless(res == 0);
}
END_TEST
