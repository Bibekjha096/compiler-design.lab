%{
#include "ifelse.tab.h"
#include <stdlib.h>
%}
%%
"if" { return IF; }
"else" { return ELSE; }
"=" { return ASSIGN; }
";" { return SEMICOLON; }
"(" { return '('; }
")" { return ')'; }
[<>!=]=? { return RELOP; }
[0-9]+ { yylval = atoi(yytext); return NUM; }
[a-zA-Z_][a-zA-Z0-9_]* { return ID; }
[\t \r] { /* skip whitespace */ }
[\n] { return 0; /* signal end-of-input for this
yyparse() call */ }
. { printf("Unknown symbol: %s\n", yytext); }
%%
int yywrap() {
return 1;
}