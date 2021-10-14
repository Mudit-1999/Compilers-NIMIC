grammar Expr;

/** The start rule; begin parsing here. */
prog:   body EOF; 

body:   dfun NEWLINE body       #bodyDecFun
    |   dvar NEWLINE body       #bodyDecVar
    |   bfun body               #bodyBlockFun   
    |                           #bodyEpsilon
    ;
    
bfun:   dfun  block             #blockFunction  
    ;

dfun: funType ID '(' funArguments ')'   #decFunctionArg
    | funType ID '(' ')'                #decFunction    
    ;

funType :tp=('int'|'uint'|'char'|'void');

funArguments: varType varName                       #funDecVar 
            | varType ID '[' ']'                    #funDecArray
            | varType varName ',' funArguments      #funDecVarMul
            | varType ID '[' ']' ',' funArguments   #funDecArrayMul
            ;

dvar: varType varList                  #decVariable;

varList :   varName                     #varSingle
        |   varName ',' varList         #varMultiple
        |   ivar                        #ivarSingle
        |   ivar ',' varList            #ivarMultiple
        ;

varType:   tp=('int'|'uint'|'char');

ivar    :   ID '=' expr                 #iVariable;

varName :   ID                          #varNameID
        |   ID ('[' expr ']')+          #varNameAID    
        ;

stat:   funCall NEWLINE                 #statfunCall
    |   assignment NEWLINE              #statAssignExpr
    |   RETURN='return' expr NEWLINE    #statReturn
    |   io NEWLINE                      #statIO
    |   cond                            #statConditional
    |   loop                            #statLoopVar
    |   expr NEWLINE                    #statExpr
    |   dvar NEWLINE                    #statDecVar
    |   NEWLINE                         #statNewLine
    ;

loop:   'for'   '(' assignment NEWLINE  expr NEWLINE assignment  ')' block  #loopFor
    |   'while' '(' expr ')' block                                          #loopWhile
    ;

assignment  : useVarName '=' expr 
            ;

cond:   'if' '(' expr ')' block                 #condIf
    |   'if' '(' expr ')' block 'else' block    #condIfElse
    ;

block   : '{' stat* '}'      #blockStat
        ;

funCall :   ID '(' ')'              #methodCall
        |   ID '(' argumnts ')'     #methodCallArg
        ;   

argumnts:   expr (',' expr)*        #methodArgument
        ;


io  :   'input' '('useVarName')'    #ioInput 
    |   'output' '(' ')'            #ioOutputBlank
    |   'output' '('expr')'         #ioOutput
    ;

useVarName  :   ID                      #useVarNameID
            |   ID ('[' expr ']')+      #useVarNameAID    
            ;       

expr:   expr op=('*'|'/') expr                      #exprMulDiv
    |   expr op=('+'|'-'|'%') expr                  #exprAddSub
    |   expr bop=('&'|'|'|'^') expr                 #exprBitOp
    |   expr rop=('<'|'=='|'!='|'>'|'<='|'>=') expr #exprRelOp
    |   expr lop=('&&'|'||') expr                   #exprLogOp
    |   funCall                                     #exprFunCall
    |   INT                                         #exprINT
    |   ID                                          #exprID
    |   CHAR                                        #exprCHAR
    |   ID ('[' expr ']')+                          #exprAID
    |   '(' expr ')'                                #exprParenthesis
    |   expr '?' expr ':' expr                      #exprTernary
    |                                               #exprEpsilon   
    ;

ID  :   [a-zA-Z][a-zA-Z0-9_]* ;      // match identifiers <label id="code.tour.expr.3"/>
INT :   [0-9]+ ;                     // match integers
CHAR: '\'' . '\'' | '\'' '\\' 'n' '\'' | '\'' '\\' '"' '\'' | '\'' '\\' '\\' '\'' | '\'' '\\' '\'' '\''; // char literal 
NEWLINE: ';' ;                       // return newlines to parser (is end-statement signal)  
WS  :   [ \r\n\t]+ -> skip ;        // toss out whitespace
