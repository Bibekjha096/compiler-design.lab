%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
%}

%union {
    int num;
}

%token <num> NUMBER

%type <num> expr term factor

%%

input
    :
    | input line
    ;

line
    : '\n'
    | expr '\n'
      {
          printf("Result = %d\n", $1);
      }
    ;

expr
    : expr '+' term
      {
          $$ = $1 + $3;
      }
    | expr '-' term
      {
          $$ = $1 - $3;
      }
    | term
      {
          $$ = $1;
      }
    ;

term
    : term '*' factor
      {
          $$ = $1 * $3;
      }
    | term '/' factor
      {
          if ($3 == 0)
          {
              yyerror("Division by zero");
              $$ = 0;
          }
          else
          {
              $$ = $1 / $3;
          }
      }
    | factor
      {
          $$ = $1;
      }
    ;

factor
    : '(' expr ')'
      {
          $$ = $2;
      }
    | NUMBER
      {
          $$ = $1;
      }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Error: %s\n", s);
}

int main(void)
{
    printf("=================================\n");
    printf(" Arithmetic Expression Evaluator\n");
    printf("=================================\n");

    yyparse();

    return 0;
}