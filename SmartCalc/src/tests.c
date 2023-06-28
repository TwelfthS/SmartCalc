#include <check.h>
#include <stdio.h>

#include "s21_smart_calc.h"

START_TEST(add) {
  char string[10] = "2+2";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 4);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(add2) {
  char string[10] = "2+2-1";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 3);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(add3) {
  char string[10] = "2+2*5";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 12);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(sub) {
  char string[10] = "0-375";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, -375);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(sub2) {
  char string[10] = "0-3.75.";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(sub3) {
  char string[10] = "3-(2+5)6";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 7);
}
END_TEST

START_TEST(mul) {
  char string[10] = "4.1*2.5";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 10.25);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(mul2) {
  char string[10] = "4.1*+-2.5";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 2);
}
END_TEST

START_TEST(div) {
  char string[10] = "27/4";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 6.75);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(div2) {
  char string[10] = "27/0";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 5);
}
END_TEST

START_TEST(mod) {
  char string[10] = "27%5";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 2);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(mod2) {
  char string[10] = "27%0";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 5);
}
END_TEST

START_TEST(pow1) {
  char string[10] = "3^3*2";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 54);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(pow2) {
  char string[10] = "3*3^2";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 27);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(un_plus) {
  char string[10] = "25++.1";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 25.1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(un_minus) {
  char string[10] = "25+-.1";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 24.9);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(iks) {
  char string[10] = "7*x*(3+5)";
  double result;
  int error = 0;
  double x = -2;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, -112);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(iks2) {
  char string[10] = "7*-x";
  double result;
  int error = 0;
  double x = -2;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 14);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(iks3) {
  char string[10] = "7x";
  double result;
  int error = 0;
  double x = -2;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 9);
}
END_TEST

START_TEST(brackets_empty) {
  char string[10] = "()";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 6);
}
END_TEST

START_TEST(brackets_mismatch) {
  char string[10] = "(";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 3);
}
END_TEST

START_TEST(brackets) {
  char string[10] = "(8-7)*18";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 18);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(log1) {
  char string[10] = "log(x)";
  double result;
  int error = 0;
  double x = 100;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 2);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(ln1) {
  char string[10] = "ln(x)";
  double result;
  int error = 0;
  double x = 3;
  result = parser(string, x, &error);
  int success = 0;
  if (fabs(result - 1.09861228867) < 0.0000001) {
    success = 1;
  }
  ck_assert_int_eq(success, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(sin1) {
  char string[10] = "sin(x)";
  double result;
  int error = 0;
  double x = 3;
  result = parser(string, x, &error);
  int success = 0;
  if (fabs(result - 0.14112000806) < 0.0000001) {
    success = 1;
  }
  ck_assert_int_eq(success, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(sin2) {
  char string[10] = "4sin(x)";
  double result;
  int error = 0;
  double x = 3;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 7);
}
END_TEST

START_TEST(cos1) {
  char string[10] = "cos(x)";
  double result;
  int error = 0;
  double x = 3;
  result = parser(string, x, &error);
  int success = 0;
  if (fabs(result + 0.9899924966) < 0.0000001) {
    success = 1;
  }
  ck_assert_int_eq(success, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(tan1) {
  char string[10] = "tan(x)";
  double result;
  int error = 0;
  double x = 3;
  result = parser(string, x, &error);
  int success = 0;
  if (fabs(result + 0.14254654307) < 0.0000001) {
    success = 1;
  }
  ck_assert_int_eq(success, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(asin1) {
  char string[10] = "asin(x)";
  double result;
  int error = 0;
  double x = 0.7;
  result = parser(string, x, &error);
  int success = 0;
  if (fabs(result - 0.7753974966) < 0.0000001) {
    success = 1;
  }
  ck_assert_int_eq(success, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(acos1) {
  char string[10] = "acos(x)";
  double result;
  int error = 0;
  double x = 0.7;
  result = parser(string, x, &error);
  int success = 0;
  if (fabs(result - 0.7953988302) < 0.0000001) {
    success = 1;
  }
  ck_assert_int_eq(success, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(atan1) {
  char string[10] = "atan(x)";
  double result;
  int error = 0;
  double x = 0.7;
  result = parser(string, x, &error);
  int success = 0;
  if (fabs(result - 0.6107259644) < 0.0000001) {
    success = 1;
  }
  ck_assert_int_eq(success, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(sqrt1) {
  char string[10] = "sqrt(x)";
  double result;
  int error = 0;
  double x = 345;
  result = parser(string, x, &error);
  int success = 0;
  if (fabs(result - 18.574175621) < 0.0000001) {
    success = 1;
  }
  ck_assert_int_eq(success, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(exp1) {
  char string[10] = "21e+2+25";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_double_eq(result, 2125);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(exp_2) {
  char string[10] = "21e2+25";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 8);
}
END_TEST

START_TEST(exp3) {
  char string[10] = "21e";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 8);
}
END_TEST

START_TEST(rand) {
  char string[10] = "2231wfr";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 10);
}
END_TEST

START_TEST(err2_1) {
  char string[10] = "2+-+-3";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 2);
}
END_TEST

START_TEST(err2_2) {
  char string[10] = "(*5)";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 2);
}
END_TEST

START_TEST(err2_3) {
  char string[10] = "6*)";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 2);
}
END_TEST

START_TEST(err7) {
  char string[10] = "6(2+3)";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 7);
}
END_TEST

START_TEST(err3) {
  char string[10] = "5*7)";
  double result;
  int error = 0;
  double x = 0;
  result = parser(string, x, &error);
  ck_assert_int_eq(error, 3);
}
END_TEST

Suite *suite() {
  Suite *s;
  s = suite_create("SmartCalc");
  TCase *tc_all;
  tc_all = tcase_create("all tests");
  tcase_add_test(tc_all, add);
  tcase_add_test(tc_all, add2);
  tcase_add_test(tc_all, add3);
  tcase_add_test(tc_all, sub);
  tcase_add_test(tc_all, sub2);
  tcase_add_test(tc_all, sub3);
  tcase_add_test(tc_all, mul);
  tcase_add_test(tc_all, mul2);
  tcase_add_test(tc_all, div);
  tcase_add_test(tc_all, div2);
  tcase_add_test(tc_all, mod);
  tcase_add_test(tc_all, mod2);
  tcase_add_test(tc_all, pow1);
  tcase_add_test(tc_all, pow2);
  tcase_add_test(tc_all, un_plus);
  tcase_add_test(tc_all, un_minus);
  tcase_add_test(tc_all, iks);
  tcase_add_test(tc_all, iks2);
  tcase_add_test(tc_all, iks3);
  tcase_add_test(tc_all, brackets_empty);
  tcase_add_test(tc_all, brackets_mismatch);
  tcase_add_test(tc_all, brackets);
  tcase_add_test(tc_all, log1);
  tcase_add_test(tc_all, ln1);
  tcase_add_test(tc_all, sin1);
  tcase_add_test(tc_all, sin2);
  tcase_add_test(tc_all, cos1);
  tcase_add_test(tc_all, tan1);
  tcase_add_test(tc_all, asin1);
  tcase_add_test(tc_all, acos1);
  tcase_add_test(tc_all, atan1);
  tcase_add_test(tc_all, sqrt1);
  tcase_add_test(tc_all, exp1);
  tcase_add_test(tc_all, exp_2);
  tcase_add_test(tc_all, exp3);
  tcase_add_test(tc_all, rand);
  tcase_add_test(tc_all, err2_1);
  tcase_add_test(tc_all, err2_2);
  tcase_add_test(tc_all, err2_3);
  tcase_add_test(tc_all, err7);
  tcase_add_test(tc_all, err3);
  suite_add_tcase(s, tc_all);

  return s;
}

int main() {
  int success = 0;
  Suite *s;
  SRunner *runner;
  s = suite();
  runner = srunner_create(s);
  srunner_run_all(runner, CK_NORMAL);
  success = srunner_ntests_failed(runner);
  srunner_free(runner);
  return (success == 0);
}