#ifndef S21_SMART_CALC_H
#define S21_SMART_CALC_H

int isnumber(char a);
int issign(char a);
int check_oper(char *string, int i);
int check_func(char *string, int i);
double parser(char *string, double x, int *error);
int precedence(char a, char b);
double action(double a, double b, char oper, int *error);
int number_validation(char *string, double *output, int i, int *k, int *neg,
                      int *called, int *error);
int sign_validation(char *string, char *stack, double *output, int i, int *s,
                    int *k, int *neg, int *called, int *error);
int bracket_validation(char *string, char *stack, double *output, int i, int *s,
                       int *k, int *bracket, int *b, int *called, int *error);
void func_action(char *stack, double *output, int *s, int *k);
int func_validation(char *string, char *stack, int i, int *s_count, int *called,
                    int *error);

#endif