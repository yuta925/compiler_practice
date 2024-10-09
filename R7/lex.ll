%{
/*===================================================================

     lex.ll: mini-C プログラムの lex ファイル
            コンパイラ演習  2004 (c) 平岡佑介, 石浦菜岐佐

=====================================================================*/

/*********************************************************************
    A パート ( %} の行まで)
    ・字句解析の C プログラムの冒頭ににそのまま取り込まれる
    ・ヘッダのインクルードやグローバル変数の宣言等をここに書く
*********************************************************************/

#include "lex.h"

int linenum;

/*********************************************************************
    B パート ( %} の 次の行から %% の行まで )
    ・正規表現の定義 
    ・C パートに書くには少し複雑なパターンをここに定義する 
*********************************************************************/
%}

num [0-9]+
id [a-zA-Z_][a-zA-Z_0-9]*
esc_seq  \\(n|t|\\|\')

%{
/*********************************************************************
   C パート (%% の行から次の %% の行まで)
   ・字句のパターンと, それを見つけたときのアクションを書く
*********************************************************************/
%}
%%

[ \t\r]    {}
"\n"       {linenum++;}

"char"     { return lex_KW_CHAR; }
"int"      { return lex_KW_INT; }
"else"     { return lex_KW_ELSE; }
"if"       { return lex_KW_IF; }
"return"   { return lex_KW_RETURN; }
"while"    { return lex_KW_WHILE; }

","        { return lex_COMMA; }
";"        { return lex_SEMICOLON; }
"("        { return lex_LPAREN; }
")"        { return lex_RPAREN; }
"{"        { return lex_LBRACE; }
"}"        { return lex_RBRACE; }
"["        { return lex_LBRACK; }
"]"        { return lex_RBRACK; }
"+"        { return lex_PLUS; }
"-"        { return lex_MINUS; }
"*"        { return lex_STAR; }
"/"        { return lex_SLASH; }
"%"        { return lex_PERCENT; }
"&"        { return lex_AND; }
"="        { return lex_EQ; }
"=="       { return lex_EQEQ; }
"!="       { return lex_NE; }
">"        { return lex_GT; }
">="       { return lex_GE; }
"<"        { return lex_LT; }
"<="       { return lex_LE; }

{num}      { sscanf(yytext, "%d", &yylval.val); return lex_INT; }
{id}       { yylval.string = strdup(yytext); return lex_ID; }

"'"."'"    { yylval.val = yytext[1]; return lex_CHAR; }

'({esc_seq}|[^'\\])' { 
                        switch (yytext[2]) {
                          case 'n': yylval.val = '\n'; break;
                          case 't': yylval.val = '\t'; break;
                          case '\\': yylval.val = '\\'; break;
                          case '\'': yylval.val = '\''; break;
                          default: 
                              fprintf(stderr, "%d: 不正なエスケープシーケンス '%s'\n", linenum, yytext); 
                              exit(4);
                        }
                        return lex_CHAR; 
                      }

.          { fprintf(stderr, "%d: 不正な文字 '%c'\n", linenum, yytext[0]); exit(4);}


%%

/*********************************************************************
   D パート
   ・字句解析の C プログラムの末尾にそのまま取り込まれる
   ・関数の実装等をここに書く
*********************************************************************/

/*============================================
 *  yyerror()
 *  構文解析のエラー時に yacc から呼び出される
 *============================================*/
void yyerror(const char *s)
{
  fprintf(stderr, "line %d: %s\n", linenum, s);
  exit(1);
}

/*============================================
 *  yywrap()
 *  ファイル1つだけを扱う場合は, 常に 1 を返す
 *============================================*/

int yywrap()
{
  return 1;
}

