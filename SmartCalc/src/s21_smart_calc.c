/** \file
@brief Основной файл программы
@details Описание кодов ошибок:
1 - две точки в числе, 2 - нет числа до или после оператора, 3 - не совпадает
количество скобок 5 - деление на ноль, 6 - пустые скобки, 7 - пропущен оператор,
8 - ошибка с экспоненциальной записью, 9 - ошибка с x, 10 - недопустимые символы
*/
#include "s21_smart_calc.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isnumber(char a) {  /// Проверяет, является ли введенный аргумент цифрой
  int ret = 0;
  if ((a >= '0' && a <= '9') || a == '.' || a == 'x') ret = 1;
  return ret;
}
int issign(char a) {  /// Проверяет, является ли введенный аргумент знаком
  int ret = 0;
  if (a == '+' || a == '-' || a == '*' || a == '/' || a == '^' || a == '%')
    ret = 1;
  return ret;
}

/*!
Данная функция принимает на вход строку и число, и проверяет, чем является i + 1
элемент в этой строке. Возвращает 0 по умолчанию, 2 - если следующего элемента
не существует, или он не является числом, или является знаком (2 - код
соответствующей ошибки). Функция применяется для проверки следующего за
оператором элемента на валидность
*/
int check_oper(char *string, int i) {
  int ret = 0;
  if (i + 1 == (int)strlen(string) || issign(string[i + 1]) ||
      !isnumber(string[i + 1])) {
    ret = 2;
  }
  return ret;
}

/*!
Данная функция принимает на вход строку и число, и проверяет, чем является i - 1
элемент в этой строке. Возвращает 0 по умолчанию, и 1 если это закрывающая
скобка или число. Применяется для проверки валидности символа перед арифм
функцией (1 - не валидно)
*/
int check_func(char *string, int i) {
  int ret = 0;
  if (i != 0 && (string[i - 1] == ')' || isnumber(string[i - 1]))) {
    ret = 1;
  }
  return ret;
}

/*!
Основная фукнция. Проходит по всей данной строке и преобразует ее обратной
польской нотацией, выполняя операции на ходу. Принимает строку, переменную x и
указатель на переменную, хранящую значение ошибки
*/
double parser(char *string, double x, int *error) {
  int k = 0;   // output counter
  int s = 0;   // stack counter
  int b = -1;  // saves bracket's [i]
  char stack[512];
  double output[2048];
  output[0] = 0;
  int bracket = 0;  // counts brackets. '(' as +1, ')' as -1
  int neg = 0;      // saves info if next value must be negated (unary minus)
  int called;       // check if current character in string was read
  for (int i = 0; i < (int)strlen(string) && *error == 0; i++) {
    called = 0;
    i = number_validation(string, output, i, &k, &neg, &called, error);
    if (string[i] == 'x' && *error == 0) {
      if ((i != 0 && (isnumber(string[i - 1]) || string[i - 1] == ')')) ||
          (i + 1 != (int)strlen(string) && isnumber(string[i + 1]))) {
        *error = 9;
      } else {
        if (neg == 1) {
          x *= -1;
          neg = 0;
        }
        output[k] = x;
        k++;
      }
      called = 1;
    }
    i = sign_validation(string, stack, output, i, &s, &k, &neg, &called, error);
    i = bracket_validation(string, stack, output, i, &s, &k, &bracket, &b,
                           &called, error);
    i = func_validation(string, stack, i, &s, &called, error);
    if (string[i] == 'e' && *error == 0) {
      *error = 8;
    }
    if (called == 0 && *error == 0) {
      *error = 10;
    }
  }
  s--;
  k--;
  if (bracket != 0 && *error == 0) *error = 3;
  if (*error == 0) {
    while (s >= 0) {
      output[k - 1] = action(output[k - 1], output[k], stack[s], error);
      s--;
      k--;
    }
  }
  return output[0];
}

/*!
Сравнивает приоритет двух операторов, которые принимает на вход.
0 - приоритет первого ниже, 1 - остальные случаи
*/
int precedence(char a, char b) {
  int ret = 0;
  if ((a != '+' && a != '-' && a != '*' && a != '/' && a != '^') ||
      (b != '+' && b != '-' && b != '*' && b != '/' && b != '^')) {
    return 0;
  }
  if (a == '*' || a == '/') {
    ret = 1;
  }
  if (a == '^') {
    ret = 1;
  } else if (b == '^') {
    ret = 0;
  }
  if ((a == '+' || a == '-') && (b == '+' || b == '-')) {
    ret = 1;
  }
  if ((a == '+' || a == '-') && (b == '*' || b == '/')) {
    ret = 0;
  }
  return ret;
}

/*!
Производит действие с числами a и b используя оператор oper, и в случае ошибки
записывает ее код в error
*/
double action(double a, double b, char oper, int *error) {
  double ret = 0;
  if (oper == '+') {
    ret = a + b;
  }
  if (oper == '-') {
    ret = a - b;
  }
  if (oper == '*') {
    ret = a * b;
  }
  if (oper == '/') {
    if (b == 0) {
      *error = 5;
    } else {
      ret = a / b;
    }
  }
  if (oper == '^') {
    ret = pow(a, b);
  }
  if (oper == '%') {
    if (b == 0) {
      *error = 5;
    } else {
      ret = fmod(a, b);
    }
  }
  return ret;
}

/*!
Проверяет, является ли i-тый символ в string числом, и записывает ее в k-тый
элемент массива output. Умножает его на -1 если был использован унарный минус
(инфа об этом в neg). В случае успеха записывает 1 в called, в случае ошибки
записывает ее код в error. Возвращает i, на котором остановился в результате
своей работы.
*/
int number_validation(char *string, double *output, int i, int *k, int *neg,
                      int *called, int *error) {
  int dot = 0;
  int e = 0;
  int n = 0;
  char number[512];
  char **endptr = {0};  // for strtod
  while ((string[i] >= '0' && string[i] <= '9') || string[i] == '.' ||
         ((string[i] == 'e' || string[i] == '+' || string[i] == '-') &&
          i + 1 != (int)strlen(string))) {
    if (string[i] == '.') {
      if (dot == 0) {
        dot = 1;
      } else {
        *error = 1;
      }
    }
    if (string[i] == 'e') {
      if (e == 0 && i != 0 && isnumber(string[i - 1]) &&
          (string[i + 1] == '+' || string[i + 1] == '-')) {
        e = 1;
      } else {
        *error = 8;
      }
    }
    if ((string[i] == '+' || string[i] == '-') &&
        (i == 0 || string[i - 1] == '(' || string[i - 1] == 'e') &&
        isnumber(string[i + 1])) {
      number[n++] = string[i++];
    } else if (string[i] != '+' && string[i] != '-') {
      if (i != 0 && string[i - 1] == ')') {
        *error = 7;
      }
      number[n++] = string[i++];
    } else {
      break;
    }
  }
  if (n != 0 && *error == 0) {
    number[n] = '+';
    double d = strtod(number, endptr);
    if (*neg == 1) {
      d *= -1;
      *neg = 0;
    }
    output[*k] = d;
    *k = *k + 1;
    i--;
    *called = 1;
  }
  return i;
}

/*!
Проверяет, является ли i-тый символ в string оператором, и записывает его в
s-тый элемент массива stack, либо вызывает функцию action по правилам обратной
польской нотации. В последнем случае записывает результат в k-тый элемент
массива output. Если далее используется унарный минус, записывает инфу об этом в
neg. В случае успеха записывает 1 в called, в случае ошибки записывает ее код в
error. Возвращает i, на котором остановился в результате своей работы.
*/
int sign_validation(char *string, char *stack, double *output, int i, int *s,
                    int *k, int *neg, int *called, int *error) {
  if (issign(string[i]) && *error == 0) {
    if (i + 1 == (int)strlen(string) || i == 0 ||
        (!isnumber(string[i - 1]) && string[i - 1] != ')')) {
      *error = 2;
    } else if (!isnumber(string[i + 1]) && string[i + 1] != '(' &&
               string[i + 1] != '+' && string[i + 1] != '-' &&
               strchr("asctl", string[i + 1]) == NULL) {
      *error = 2;
    } else {
      while (*s != 0 && precedence(stack[*s - 1], string[i])) {
        output[*k - 2] =
            action(output[*k - 2], output[*k - 1], stack[*s - 1], error);
        *k = *k - 1;
        *s = *s - 1;
      }
      stack[*s] = string[i];
      *s = *s + 1;
      if (string[i + 1] == '+') {
        *error = check_oper(string, i + 1);
        i++;
      } else if (string[i + 1] == '-') {
        *error = check_oper(string, i + 1);
        i++;
        *neg = 1;
      }
    }
    *called = 1;
  }
  return i;
}

/*!
Проверяет, является ли i-тый символ в string скобкой, и производит действия по
правилам обратной польской нотации, используя стеки output (k) и stack (s). b
сохраняет инфу о позиции откр скобки, чтобы следить за выражениями вроде '()'.
bracket считает количество открывающих (+1) и закрывающих (-1) скобок.
В случае успеха записывает 1 в called, в случае ошибки записывает ее код в
error. Возвращает i, на котором остановился в результате своей работы.
*/
int bracket_validation(char *string, char *stack, double *output, int i, int *s,
                       int *k, int *bracket, int *b, int *called, int *error) {
  if (string[i] == '(' && *error == 0) {
    if (i != 0 && isnumber(string[i - 1])) {
      *error = 7;
    } else {
      stack[*s] = string[i];
      *s = *s + 1;
      *bracket = *bracket + 1;
      *b = i;
    }
    *called = 1;
  }
  if (string[i] == ')' && *error == 0) {
    *bracket = *bracket - 1;
    while (*s != 0 && stack[*s - 1] != '(') {
      output[*k - 2] =
          action(output[*k - 2], output[*k - 1], stack[*s - 1], error);
      *k = *k - 1;
      *s = *s - 1;
    }
    if (*s == 0) {
      *error = 3;
    } else if (i - *b == 1) {
      *error = 6;
    } else if (*s == 1) {
      *s = *s - 1;
    } else {
      *s = *s - 1;
      func_action(stack, output, s, k);
    }
    *called = 1;
  }
  return i;
}

/*!
Вызывает арифм функции в зависимости от элемента stack[s - 1], и записывает
результат в output[k - 1]
*/
void func_action(char *stack, double *output, int *s, int *k) {
  if (stack[*s - 1] == 'o') {
    output[*k - 1] = log10(output[*k - 1]);
    *s = *s - 1;
  }
  if (stack[*s - 1] == 'n') {
    output[*k - 1] = log(output[*k - 1]);
    *s = *s - 1;
  }
  if (stack[*s - 1] == 's') {
    output[*k - 1] = sin(output[*k - 1]);
    *s = *s - 1;
  }
  if (stack[*s - 1] == 'c') {
    output[*k - 1] = cos(output[*k - 1]);
    *s = *s - 1;
  }
  if (stack[*s - 1] == 't') {
    output[*k - 1] = tan(output[*k - 1]);
    *s = *s - 1;
  }
  if (stack[*s - 1] == 'a') {
    output[*k - 1] = asin(output[*k - 1]);
    *s = *s - 1;
  }
  if (stack[*s - 1] == 'r') {
    output[*k - 1] = acos(output[*k - 1]);
    *s = *s - 1;
  }
  if (stack[*s - 1] == 'g') {
    output[*k - 1] = atan(output[*k - 1]);
    *s = *s - 1;
  }
  if (stack[*s - 1] == 'q') {
    output[*k - 1] = sqrt(output[*k - 1]);
    *s = *s - 1;
  }
}

/*!
Проверяет, является ли i-тый элемент в string функцией, и записывает ее код в
stack[s_count]. В случае успеха записывает 1 в called, в случае ошибки
записывает ее код в error Возвращает i, на котором остановился в результате
своей работы
*/
int func_validation(char *string, char *stack, int i, int *s_count, int *called,
                    int *error) {
  int temp = i;
  int s = *s_count;
  if (i + 3 < (int)strlen(string) && string[i] == 'l' && string[i + 1] == 'o' &&
      string[i + 2] == 'g' && string[i + 3] == '(') {
    if (check_func(string, i)) *error = 7;
    i++;
    stack[s++] = string[i++];
  }
  if (i + 2 < (int)strlen(string) && string[i] == 'l' && string[i + 1] == 'n' &&
      string[i + 2] == '(') {
    if (check_func(string, i)) *error = 7;
    i++;
    stack[s++] = string[i];
  }
  if (i + 3 < (int)strlen(string) && string[i] == 's' && string[i + 1] == 'i' &&
      string[i + 2] == 'n' && string[i + 3] == '(') {
    if (check_func(string, i)) *error = 7;
    stack[s++] = string[i];
    i = i + 2;
  }
  if (i + 3 < (int)strlen(string) && string[i] == 'c' && string[i + 1] == 'o' &&
      string[i + 2] == 's' && string[i + 3] == '(') {
    if (check_func(string, i)) *error = 7;
    stack[s++] = string[i];
    i = i + 2;
  }
  if (i + 3 < (int)strlen(string) && string[i] == 't' && string[i + 1] == 'a' &&
      string[i + 2] == 'n' && string[i + 3] == '(') {
    if (check_func(string, i)) *error = 7;
    stack[s++] = string[i];
    i = i + 2;
  }
  if (i + 4 < (int)strlen(string) && string[i] == 'a' && string[i + 1] == 's' &&
      string[i + 2] == 'i' && string[i + 3] == 'n' && string[i + 4] == '(') {
    if (check_func(string, i)) *error = 7;
    stack[s++] = string[i];
    i = i + 3;
  }
  if (i + 4 < (int)strlen(string) && string[i] == 'a' && string[i + 1] == 'c' &&
      string[i + 2] == 'o' && string[i + 3] == 's' && string[i + 4] == '(') {
    if (check_func(string, i)) *error = 7;
    stack[s++] = 'r';
    i = i + 3;
  }
  if (i + 4 < (int)strlen(string) && string[i] == 'a' && string[i + 1] == 't' &&
      string[i + 2] == 'a' && string[i + 3] == 'n' && string[i + 4] == '(') {
    if (check_func(string, i)) *error = 7;
    stack[s++] = 'g';
    i = i + 3;
  }
  if (i + 4 < (int)strlen(string) && string[i] == 's' && string[i + 1] == 'q' &&
      string[i + 2] == 'r' && string[i + 3] == 't' && string[i + 4] == '(') {
    if (check_func(string, i)) *error = 7;
    stack[s++] = 'q';
    i = i + 3;
  }
  *s_count = s;
  if (temp != i) {
    *called = 1;
  }
  return i;
}
