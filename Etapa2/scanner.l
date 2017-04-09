%{

#include "hash.h"
#include "y.tab.h"

int running = 1;
int lineNumber = 1;

int getLineNumber();
int isRunning();
void initMe();

%}

%option nounput
%option noinput

%x COMMENT

%%

		/* init */
		static int initialized = 0;

		if(!initialized){
			initMe();
			initialized = 1;
		}

	/* RESERVED WORDS */
int 					{ return KW_INT; }
float 					{ return KW_FLOAT; }
bool 					{ return KW_BOOL; }
char 					{ return KW_CHAR; }
if 						{ return KW_IF; }
then 					{ return KW_THEN; }
else 					{ return KW_ELSE; }
for 					{ return KW_FOR; }
read 					{ return KW_READ; }
print 					{ return KW_PRINT; }
return 					{ return KW_RETURN; }
to 						{ return KW_TO; }

	/* SPECIAL CHARACTERS */
[-,;:()\[\]{}+*/<>=!&$] 	{ return yytext[0];}

	/* COMPOUNDS OPERATORS */
\<=						{ return OPERATOR_LE; }
>=						{ return OPERATOR_GE; }
==						{ return OPERATOR_EQ; }
!=						{ return OPERATOR_NE; }
&&						{ return OPERATOR_AND; }
\|\|					{ return OPERATOR_OR; }	

	/* LITERALS */
FALSE					{ yylval.symbol = hashInsert(SYMBOL_LIT_FALSE, yytext); return LIT_FALSE; }
TRUE					{ yylval.symbol = hashInsert(SYMBOL_LIT_TRUE, yytext); return LIT_TRUE; }
[0-9]+					{ yylval.symbol = hashInsert(SYMBOL_LIT_INT, yytext); return LIT_INTEGER; }
\'.\'					{ yylval.symbol = hashInsert(SYMBOL_LIT_CHAR, yytext); return LIT_CHAR; }
\"([^\"\\\n]|\\.)*\"	{ yylval.symbol = hashInsert(SYMBOL_LIT_STRING, yytext); return LIT_STRING; }

	/* IDENTIFIERS  */
[A-Za-z][A-Za-z_0-9]* 	{ yylval.symbol = hashInsert(SYMBOL_IDENTIFIER, yytext); return TK_IDENTIFIER; }

	/* LINE COMMENT */
"//".*					{ }

	/* COMMENT */
"/*" 					{ BEGIN(COMMENT); }
<COMMENT>"\n" 			{ lineNumber++; }
<COMMENT>"*/"			{ BEGIN(INITIAL); }
<COMMENT>.				{ }

	/* TABS AND NEW LINE */
\n 						{ lineNumber++; }
[ \t]					{ }

	/* ERROR */
.						{ return TOKEN_ERROR; }

%%

int yywrap(){
	running = 0;
	return 1;
}

void initMe(){
	running = 1;
	lineNumber = 1;
	hashInit();
}

int getLineNumber() {
	return lineNumber;
}

int isRunning() {
	return running;
}
