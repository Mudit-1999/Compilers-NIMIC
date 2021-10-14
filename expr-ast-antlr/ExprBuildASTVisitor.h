#include <iostream>
#include "ast.h"
#include <vector>
#include "ExprVisitor.h"


using namespace std;

class ExprBuildASTVisitor : public ExprVisitor
{
public:
    /**
    * Visit parse trees produced by ExprParser.
    */
//    Program
    virtual antlrcpp::Any visitProg(ExprParser::ProgContext *context)
    {
        cout<<"In visit Prog"<<endl;
        ASTProg *node = new ASTProg();
        node->body= visit(context->body());
        return (ASTProg *) node;
    }

// Body

    virtual antlrcpp::Any visitBodyDecFun(ExprParser::BodyDecFunContext *context)
    {
        cout << "In visitBodyDecFun" << endl;
        ASTBodyDecFun *node = new ASTBodyDecFun();
        node->dfun= visit(context->dfun());
        node->body= visit(context->body());
        return (ASTBody *) node;
    }

    virtual antlrcpp::Any visitBodyDecVar(ExprParser::BodyDecVarContext *context)
    {
        cout << "In visitBodyDecVar" << endl;
        ASTBodyDecVar *node = new ASTBodyDecVar();
        node->dvar= visit(context->dvar());
        node->body= visit(context->body());
        return (ASTBody *) node;
    }

    virtual antlrcpp::Any visitBodyBlockFun(ExprParser::BodyBlockFunContext *context)
    {
        cout << "In visitBodyBlockFun" << endl;
        ASTBodyBlockFun *node = new ASTBodyBlockFun();
        node->bfun  =visit(context->bfun());
        node->body= visit(context->body());
        return (ASTBody *) node;

    }

    virtual antlrcpp::Any visitBodyEpsilon(ExprParser::BodyEpsilonContext *context)
    {
        cout << "In visitBodyEpsilon" << endl;
        return (ASTBody *) nullptr;
    }

// BlockFunction

    virtual antlrcpp::Any visitBlockFunction(ExprParser::BlockFunctionContext *context)
    {
        cout << "In visitBlockFunction" << endl;
        ASTBlockFunction *node= new ASTBlockFunction;
        node->dfun= visit(context->dfun());
        node->block = visit(context->block());
        return (ASTBlockFunction*) node;
    }


// Stat
    virtual antlrcpp::Any visitStatExpr(ExprParser::StatExprContext *context)
    {
        cout << "In visitStatExpr" << endl;
        ASTStatExpr *node = new ASTStatExpr();
        node->expr = visit(context->expr());
        return (ASTStat *) node;
    }

    virtual antlrcpp::Any visitStatAssignExpr(ExprParser::StatAssignExprContext *context)
    {
        cout << "In visitStatAssignExpr" << endl;
        ASTStatAssignExpr *node = new ASTStatAssignExpr();
        node->assignment = visit(context->assignment()) ;
        return (ASTStat *) node;
    }

    virtual antlrcpp::Any visitStatNewLine(ExprParser::StatNewLineContext *context)
    {
        cout << "In visitStatNewLine" << endl;
        return (ASTStat *) nullptr;
    }

    virtual antlrcpp::Any visitStatReturn(ExprParser::StatReturnContext *context)
    {
        cout<<" In visitStatReturn" <<endl;
        ASTStatReturn * node = new ASTStatReturn();
        node->expr= visit(context->expr());
        return (ASTStat *) node;
    }

    virtual antlrcpp::Any visitStatIO(ExprParser::StatIOContext *context) 
    {
        // evaluating io only
        cout<<"In visitStatIO"<<endl;
        return visit(context->io());
    }

    virtual antlrcpp::Any visitStatfunCall(ExprParser::StatfunCallContext *context)
    {
        cout<<"In visitStatfunCall"<<endl;
        ASTFunCall *node;
        node=visit(context->funCall());
        return (ASTStat *)node;
    }
    virtual antlrcpp::Any visitStatConditional(ExprParser::StatConditionalContext *context)
    {
        cout<<"In visitStatConditional"<<endl;
        return visit(context->cond());
    }
    virtual antlrcpp::Any visitStatDecVar(ExprParser::StatDecVarContext *context)
    {
        cout<<"In visitStatDecVar"<<endl;
        ASTStatDecVar *node=new ASTStatDecVar();
        node->dvar=visit(context->dvar());
        return (ASTStat *) node;
    }
    virtual antlrcpp::Any visitStatLoopVar(ExprParser::StatLoopVarContext *context)
    {
        cout<<"In visitStatLoopVar"<<endl;
        ASTLoop *node;
        node=visit(context->loop());
        return (ASTStat *) node;
    }


// Loop 
    virtual antlrcpp::Any visitLoopFor(ExprParser::LoopForContext *context)
    {
        cout<<"In visitLoopFor"<<endl;
        ASTLoopFor *node= new ASTLoopFor();
        node->lassignment =visit(context->assignment(0));
        node->expr =visit(context->expr());
        node->rassignment =visit(context->assignment(1));
        node->block =visit(context->block());
        return (ASTLoop *) node;
    }

    virtual antlrcpp::Any visitLoopWhile(ExprParser::LoopWhileContext *context)
    {
        cout<<"In visitLoopWhile"<<endl;
        ASTLoopWhile *node= new ASTLoopWhile();
        node->expr  = visit(context->expr());
        node->block = visit(context->block());
        return (ASTLoop *) node;
    }


// Assignment
    virtual antlrcpp::Any visitAssignment(ExprParser::AssignmentContext *context)
    {
        cout<<"In visitAssignment"<<endl;
        ASTAssignment *node= new ASTAssignment();
        node->useVarName= visit(context->useVarName());
        node->expr= visit(context->expr());
        return (ASTAssignment *) node;   
    }


//  DeclareFunction 
    virtual antlrcpp::Any visitDecFunctionArg(ExprParser::DecFunctionArgContext *context)
    {
        cout<<"In visitDecFunctionArg"<<endl;
        string id=context->ID()->getText();
        ASTDecFunctionArg *node = new ASTDecFunctionArg(id);
        node->funType =visit(context->funType());
        node->funArguments= visit(context->funArguments());
        return (ASTDeclarationFunction*) node;
    }

    virtual antlrcpp::Any visitDecFunction(ExprParser::DecFunctionContext *context)
    {
        cout<<"In visitDecFunction"<<endl;
        string id=context->ID()->getText();
        ASTDecFunction *node = new ASTDecFunction(id);
        node->funType= visit(context->funType());
        return (ASTDeclarationFunction*) node;
    }
// FunctionType
    virtual antlrcpp::Any visitFunType(ExprParser::FunTypeContext *context)
    {
        cout<<"In visitFunType"<<endl;
        string vtype=context->tp->getText();
        ASTFunType *node=new ASTFunType(vtype);
        return (ASTFunType *)node;
    }

// FunctionArguments
    virtual antlrcpp::Any visitFunDecVar(ExprParser::FunDecVarContext *context)
    {
        cout<<"In visitFunDecVar"<<endl;
        ASTFunDecVar *node =new ASTFunDecVar();
        node->varType=  visit(context->varType());
        node->varName=  visit(context->varName());
        return (ASTFunArgument*) node;
    }

    virtual antlrcpp::Any visitFunDecArray(ExprParser::FunDecArrayContext *context)
    {
        cout<<"In visitFunDecArray"<<endl;
        string id=context->ID()->getText();
        ASTFunDecArray *node = new ASTFunDecArray(id);
        node->varType=visit(context->varType());
        return (ASTFunArgument*) node;
    }

    virtual antlrcpp::Any visitFunDecVarMul(ExprParser::FunDecVarMulContext *context)
    {
        cout<<"In visitFunDecVarMul"<<endl;
        ASTFunDecVarMul *node= new ASTFunDecVarMul();
        node->varType = visit(context->varType());
        node->varName = visit(context->varName());
        node->funArguments= visit(context->funArguments());
        return (ASTFunArgument*) node;
    }

    virtual antlrcpp::Any visitFunDecArrayMul(ExprParser::FunDecArrayMulContext *context)
    {
        cout<<"In visitFunDecArrayMul"<<endl;
        string id=context->ID()->getText();
        ASTFunDecArrayMul *node= new ASTFunDecArrayMul(id);
        node->varType= visit(context->varType());
        node->funArguments= visit(context->funArguments());
        return (ASTFunArgument*) node;
    }


//  DecVariable
    virtual antlrcpp::Any visitDecVariable(ExprParser::DecVariableContext *context)
    {
        ASTDecVariable *node = new ASTDecVariable();
        node->varType=visit(context->varType());
        node->varList=visit(context->varList());
        return (ASTDecVariable*) node;
    }
// VarType
    virtual antlrcpp::Any visitVarType(ExprParser::VarTypeContext *context)
    {
        cout<<"In visitVarType"<<endl;
        string vtype=context->tp->getText();
        ASTVarType *node= new ASTVarType(vtype);
        return (ASTVarType*) node;
    }


// VarList
    virtual antlrcpp::Any visitVarSingle(ExprParser::VarSingleContext *context)
    {
        cout<<"In visitVarSingle"<<endl;
        ASTVarSingle *node= new ASTVarSingle();
        node->varName = visit(context->varName());
        return (ASTVarList*) node;
    }

    virtual antlrcpp::Any visitVarMultiple(ExprParser::VarMultipleContext *context)
    {
        cout<<"In visitVarMultiple"<<endl;
        ASTVarMultiple *node= new ASTVarMultiple();
        node->varName = visit(context->varName());
        node->varList = visit(context->varList());
        return (ASTVarList*) node;
    }

    virtual antlrcpp::Any visitIvarSingle(ExprParser::IvarSingleContext *context) 
    {
        cout<<"In visitIvarSingle"<<endl;
        ASTIVarSingle *node= new ASTIVarSingle();
        node->iVarName = visit(context->ivar());
        return (ASTVarList*) node;
    }

    virtual antlrcpp::Any visitIvarMultiple(ExprParser::IvarMultipleContext *context)
    {
        cout<<"In visitIvarMultiple"<<endl;
        ASTIVarMultiple *node= new ASTIVarMultiple();
        node->iVarName = visit(context->ivar());
        node->varList = visit(context->varList());
        return (ASTVarList*) node;
    }
// ivar
    virtual antlrcpp::Any visitIVariable(ExprParser::IVariableContext *context)
    {
        cout<<"In visitIVariable"<<endl;
        string id=context->ID()->getText();
        // cout<<id<<endl;
        ASTIVarName *node=new ASTIVarName(id);
        node->expr= visit(context->expr());
        cout<<node->getID()<<endl;

        return (ASTIVarName*) node;
    }
// varName
    virtual antlrcpp::Any visitVarNameID(ExprParser::VarNameIDContext *context)
    {
        cout<<"In visitVarNameID" <<endl;
        string id = context->ID()->getText();
        // cout<<id<<endl;
        ASTDecVarID *node= new ASTDecVarID(id);
        return (ASTVarName *)node;
    }

    virtual antlrcpp::Any visitVarNameAID(ExprParser::VarNameAIDContext *context)
    {
        cout<<"In visitVarNameAID" <<endl;
        string id = context->ID()->getText();
        ASTDecVarAID *node= new ASTDecVarAID(id);
        ASTExpr *expressionNode;
        for(auto expression:context->expr())
        {
            expressionNode= visit(expression);
            node->ArrayIndexList.push_back(expressionNode);
        }
        return (ASTVarName *)node;
    }

// Block
    virtual antlrcpp::Any visitBlockStat(ExprParser::BlockStatContext *context)
    {
        cout<<"In visitBlockStat"<<endl;
        ASTBlock *node = new ASTBlock();
        ASTStat *statementNode;

        for (auto statement : context->stat())
        {
            statementNode = visit(statement);
            if (statementNode != nullptr)
            {
                node->statementList.push_back(statementNode);
            }
        }
        return (ASTBlock *) node;
    }


// Conditional Statement

    virtual antlrcpp::Any visitCondIf(ExprParser::CondIfContext *context)
    {
        cout<<"In visitCondIf"<<endl;
        ASTCondIf *node= new ASTCondIf();
        node->expr= visit(context->expr());
        node->block=visit(context->block());
        return (ASTStat*) node;
    }

    virtual antlrcpp::Any visitCondIfElse(ExprParser::CondIfElseContext *context)
    {
        cout<<"In visitCondIfElse"<<endl;
        ASTCondIfElse *node = new ASTCondIfElse();
        node->expr= visit(context->expr());
        node->ifblock=visit(context->block(0));
        node->elseblock=visit(context->block(1));
        return (ASTStat*)node;
    }


// FunctionCall
    virtual antlrcpp::Any visitMethodCall(ExprParser::MethodCallContext *context)
    {
        cout<<"In visitMethodCall"<<endl;
        string id=context->ID()->getText();
        ASTMethodCall *node = new ASTMethodCall(id);
        return (ASTFunCall *) node;
    }

    virtual antlrcpp::Any visitMethodCallArg(ExprParser::MethodCallArgContext *context)
    {
        cout<<"In visitMethodCallArg"<<endl;
        string id=context->ID()->getText();
        ASTMethodCallArg *node = new ASTMethodCallArg(id);
        node->Argument= visit(context->argumnts());
        return (ASTFunCall *) node;  
    }


// Argumnts
    virtual antlrcpp::Any visitMethodArgument(ExprParser::MethodArgumentContext *context)
    {
        cout<<"In visitMethodArgument"<<endl;
        ASTMethodArgument *node= new ASTMethodArgument();
        ASTExpr *expressionNode;
        for(auto expression:context->expr())
        {
            expressionNode= visit(expression);
            node->expressionList.push_back(expressionNode);
        }
        return (ASTMethodArgument *) node;
    }

// IO
    virtual antlrcpp::Any visitIoInput(ExprParser::IoInputContext *context)
    {
        cout<<"In visitIoInput"<<endl;
        ASTIoInput *node = new ASTIoInput();
        node->useVarName = visit(context->useVarName());
        return (ASTStat *) node;
    }

    virtual antlrcpp::Any visitIoOutput(ExprParser::IoOutputContext *context)
    {
        cout<<"In visitIoOutput"<<endl;
        ASTIoOutput *node = new ASTIoOutput();
        node->expr= visit(context->expr());
        return (ASTStat *) node;
    } 

    virtual antlrcpp::Any visitIoOutputBlank(ExprParser::IoOutputBlankContext *context)
    {
        cout<<"In visitIoOutputBlank"<<endl;
        ASTIoOutputBlank *node= new ASTIoOutputBlank();
        return (ASTStat *) node;

    }
// Use Variable Name

    virtual antlrcpp::Any visitUseVarNameID(ExprParser::UseVarNameIDContext *context)
    {
        cout<<"In visitUseVarNameID"<<endl;
        string id  = context->ID()->getText();
        ASTUseDecVarID *node =new ASTUseDecVarID(id);
        return (ASTUseVarName *) node;
    }

    virtual antlrcpp::Any visitUseVarNameAID(ExprParser::UseVarNameAIDContext *context)
    {
        cout<<"In visitUseVarNameID"<<endl;
        string id  = context->ID()->getText();
        ASTUseDecVarAID *node= new ASTUseDecVarAID(id);
        ASTExpr *expressionNode;
        for(auto expression:context->expr())
        {
            expressionNode= visit(expression);
            node->ArrayIndexList.push_back(expressionNode);
        }
        return (ASTUseVarName *)node;
    }


// Expr
    virtual antlrcpp::Any visitExprINT(ExprParser::ExprINTContext *context)
    {
        cout << "In visitExprINT" << endl;
        ASTExprINT *node = new ASTExprINT(stoi(context->INT()->getText()));
        node->form = exprType::literal; 
        return (ASTExpr *) node;
    }
    
    virtual antlrcpp::Any visitExprCHAR(ExprParser::ExprCHARContext *context) 
    {
        cout << "In visitExprCHAR" << endl;
        ASTExprCHAR *node = new ASTExprCHAR(context->CHAR()->getText()[1]);
        node->form = exprType::literal; 
        return (ASTExpr *) node;
    }
    virtual antlrcpp::Any visitExprAddSub(ExprParser::ExprAddSubContext *context)
    {
        cout << "In visitExprAddSub" << endl;
        ASTExprBinary *node;
        ASTExpr *left, *right;

        left = visit(context->expr(0));
        right = visit(context->expr(1));
        string op = context->op->getText();

        if (op[0] == '+')
            node = new ASTExprBinary("+", left, right);
        else if(op[0]=='-')
            node = new ASTExprBinary("-", left, right);
        else
            node= new ASTExprBinary("%",left,right);
        node->form=exprType::binary;
        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExprID(ExprParser::ExprIDContext *context)
    {
        cout << "In visitExprID" << endl;
        string id = context->ID()->getText();
        ASTExprID *node = new ASTExprID(id);
        node->form=exprType::loca;
        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExprAID(ExprParser::ExprAIDContext *context)
    {
        cout<<"In visitExprAID"<<endl;
        string id= context->ID()->getText();
        ASTExprAID *node=new ASTExprAID(id);
        ASTExpr *expressionNode;
        for(auto expression:context->expr())
        {
            expressionNode= visit(expression);
            node->ArrayIndexList.push_back(expressionNode);
        }
        node->form=exprType::loca;
        return (ASTExpr*) node;

    }

    virtual antlrcpp::Any visitExprFunCall(ExprParser::ExprFunCallContext *context)
    {
        cout<<"In visitExprFunCall"<<endl;
        ASTFunCall *node;
        node=visit(context->funCall());
        return (ASTExpr *)node;       
    }

    virtual antlrcpp::Any visitExprEpsilon(ExprParser::ExprEpsilonContext *context)
    {
        cout<<"In visitExprFunCall"<<endl;
        return (ASTExpr *)nullptr;

    }


    virtual antlrcpp::Any visitExprParenthesis(ExprParser::ExprParenthesisContext *context)
    {
        cout << "In visitExprParenthesis" << endl;
        ASTExpr *node =visit(context->expr());
        node->form = exprType::enclExpr; 
        return node;
    }

    virtual antlrcpp::Any visitExprTernary(ExprParser::ExprTernaryContext *context)
    {
        cout<<"In visitExprTernary"<<endl;
        ASTExpr *first;   
        ASTExpr *second;   
        ASTExpr *third;   
        first = visit(context->expr(0));
        second = visit(context->expr(1));
        third = visit(context->expr(2));
        ASTExprTernary *node = new ASTExprTernary(first,second,third);
        node->form= exprType::ternary;
        return (ASTExpr*) node;
    }


    virtual antlrcpp::Any visitExprMulDiv(ExprParser::ExprMulDivContext *context)
    {
        cout << "In visitExprMulDiv" << endl;
        ASTExpr *left;
        ASTExpr *right;
       
        ASTExprBinary *node;

        left = visit(context->expr(0));
        right = visit(context->expr(1));

        string op = context->op->getText();

        if (op[0] == '*')
            node = new ASTExprBinary("*", left, right);
        else
            node = new ASTExprBinary("/", left, right);
        node->form=exprType::binary;
        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExprRelOp(ExprParser::ExprRelOpContext *context)
    {
        cout << "In visitExprRelOp" << endl;
        ASTExpr *left;
        ASTExpr *right;
        ASTExprBinary *node;

        left = visit(context->expr(0));
        right = visit(context->expr(1));

        string rop = context->rop->getText();
        if (rop == ">=")
            node = new ASTExprBinary(">=", left, right);
        else if(rop == ">")
            node = new ASTExprBinary(">", left, right);
        else if(rop == "<=")
            node = new ASTExprBinary("<=", left, right);
        else if(rop == "<")
            node = new ASTExprBinary("<", left, right);
        else if(rop == "==")
            node = new ASTExprBinary("==", left, right);
        else
            node = new ASTExprBinary("!=", left, right);
        node->form=exprType::binary;
        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExprBitOp(ExprParser::ExprBitOpContext *context)
    {
        cout << "In visitExprBitOp" << endl;
        ASTExpr *left;
        ASTExpr *right;
        ASTExprBinary *node;

        left = visit(context->expr(0));
        right = visit(context->expr(1));

        string bop = context->bop->getText();
        if (bop == "&")
            node = new ASTExprBinary("&", left, right);
        else if(bop == "|")
            node = new ASTExprBinary("|", left, right);
        else
            node = new ASTExprBinary("^", left, right);
        node->form=exprType::binary;
        // cout<<node->getBin_operator()<<endl;            
        return (ASTExpr *) node;
    }

    virtual antlrcpp::Any visitExprLogOp(ExprParser::ExprLogOpContext *context)
    {
        cout << "In visitExprLogOp" << endl;
        ASTExpr *left;
        ASTExpr *right;
        ASTExprBinary *node;

        left = visit(context->expr(0));
        right = visit(context->expr(1));

        string lop = context->lop->getText();
        if (lop == "&&")
            node = new ASTExprBinary("&&", left, right);
        else if(lop == "||")
            node = new ASTExprBinary("||", left, right);
        // cout<<node->getBin_operator()<<endl;            
        node->form=exprType::binary;
        return (ASTExpr *) node;
    }

};

