grammar NIMIC;

prog:  body EOF; 


body    :   dec_var NEWLINE body    | dec_var NEWLINE  
        |   dec_fun  NEWLINE body   | dec_fun  NEWLINE
        |   block_fun body | block_fun
        ;


dec_var :   Type var_list ;
var_list:   (ID|aid) 
        |   (ID|aid) ','  var_list
        |   init_assignment 
        |   init_assignment ',' var_list
        ;

dec_fun  :    Type ID '(' dec_arguments ')'| Type ID '(' ')' ;
dec_arguments   : (Type ID | Type ID'[' ']'| Type aid)  
                | (Type ID | Type ID'[' ']'| Type aid) ',' dec_arguments 
                ;

block_fun: dec_fun block;

function_call   :   ID '(' ')' 
                |   ID '(' arguments ')'
                ;

arguments       :   expr
                |   expr ',' arguments
                ;

block       : '{'  blockItem '}';

blockItem   : dec_var NEWLINE | dec_var NEWLINE blockItem
            | statements | statements blockItem
            ;

statements  : assignment NEWLINE
            | 'return' expr NEWLINE
            | io_stat NEWLINE
            | conditional
            | loop_stat
            | function_call NEWLINE
            ;


io_stat     : 'input'  '(' (ID | aid) ')'
            | 'output' '(' ')' 
            | 'output' '(' expr ')' 
            ;

loop_stat   :   'for' '(' forCondition  ')' block
            |   'while' '(' forCheck ')' block
            ;   
forCondition: forDeclaration NEWLINE forCheck NEWLINE forUpdate
            ;

forDeclaration  :   dec_var
                |   (assignment',')* assignment
                |  
                ;

forCheck    :   expr
            |   expr ',' forCheck
            ;
forUpdate   :   assignment
            |   assignment ',' forUpdate
            ;


conditional : 'if' '(' expr')' block ('else if' '(' expr')' block)*
            | 'if' '(' expr')' block ('else if' '(' expr')' block)* 'else'  block
            ;

init_assignment   : ID '=' expr;

assignment    : (aid|ID) update_op expr;


aid     :   ID ('[' expr  ']')+ ;

expr:   expr ('*'|'/') expr   
    |   expr ('+'|'-'|'%') expr   
    |   expr bitop expr
    |   expr logop expr
    |   expr relop expr   
    |   function_call
    |   uop expr
    |   LINT                    
    |   LDOUBLE                
    |   ID    
    |   ID ('[' expr ']')+
    |   '(' expr ')' 
    |   CHARACTER_LITERAL
    |   STRING_LITERAL        
    ;

Type    :   'int'|'unit'|'double'|'void'|'char';
relop   :   '<'|'=='|'<'|'>'|'<='|'>='| '<=>';
ID      :   [_a-zA-Z][a-zA-Z0-9_]* ;      // match identifiers <label id="code.tour.expr.3"/>
LINT    :   [0-9]+ ;         // match integers
bitop   :   '&' | '|' |'^';
uop     :   '~'|'!';
logop   :   '&&'| '||' ;
update_op     : '+='|'-='|'*='|'/='|'=';
CHARACTER_LITERAL : '\'' . '\'' | '\'' '\\' 'n' '\'' | '\'' '\\' '"' '\'' | '\'' '\\' '\\' '\'' | '\'' '\\' '\'' '\'';
STRING_LITERAL : '"' .*? '"';
LDOUBLE : ([0-9]+('.'[0-9]+)('f'|('e'[+-][0-9]+))?|([0-9]+)(('e'[+-][0-9]+)|'f'));
NEWLINE :';';     // return newlines to parser (is end-statement signal)
WS      :   [ \r\n\t]+  -> skip ; // toss out whitespace