%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(const char *s);
int isError = 0;
%}
%token IF ELSE ID RELOP ASSIGN NUM SEMICOLON
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%start stmt
%%
stmt : if_stmt {
if (!isError)
printf("valid\n");
}
;
/* Compiler Design Lab Report 2 */
if_stmt : IF '(' cond ')' stmt_block %prec LOWER_THAN_ELSE
| IF '(' cond ')' stmt_block ELSE stmt_block
;
stmt_block : ID ASSIGN expr SEMICOLON
| if_stmt
;
cond : ID RELOP ID
| ID RELOP NUM
;
expr : ID
| NUM
;
%%
void yyerror(const char *s) {
isError = 1;
printf("Syntax Error: Invalid statement\n");
// Flush remaining tokens on the current line using the lexer itself,
// not raw stdio, so we stay in sync with flex's internal buffer.
while (yylex() != 0)
;
}
int main() {
printf("=== IF-ELSE Statement Parser ===\n");
printf("Enter one statement per line, eg:\n");
printf("if (a>b) x=y;\n");
printf("if (a>b) x=y; else x=z;\n");
printf("Press Ctrl+C or Ctrl+Z (Windows) to exit.\n\n");
while (1) {
isError = 0;
printf("> ");
if (yyparse() != 0 && feof(stdin))
break;
}
return 0;
}