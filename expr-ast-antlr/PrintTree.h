#include <iostream>
#include "SymbolTable.h"
using namespace std;

SymbolTable *symtable ;

class PostFixVisitor : public ASTvisitor
{
public:
// Prog
    virtual void visit(ASTProg &node)
    {
        cout << "PostFixVisit traversal invoked" << endl;
        symtable = new SymbolTable(true);
        node.body->accept(*this);
        cout<<"End"<<endl;
    }
// Body
    virtual void visit(ASTBody &node)
    {
        node.accept(*this);
    }
    virtual void visit(ASTBodyDecFun &node)
    {
        node.dfun->accept(*this);
        cout<<endl;
        node.body->accept(*this);
    }

    virtual void visit(ASTBodyDecVar &node)
    {
        node.dvar->accept(*this);
        cout<<endl;
        node.body->accept(*this);
    }
    virtual void visit(ASTBodyBlockFun &node)
    {
        node.bfun->accept(*this);
        if(node.body!=nullptr)
            node.body->accept(*this);
        
    }

// BlockFunction
    virtual void visit(ASTBlockFunction &node)
    {
        node.dfun->accept(*this);
        node.block->accept(*this);
    }

// FunctionDeclaration
    virtual void visit(ASTFunArgument &node)
    {
        node.accept(*this);
    }
    virtual void visit(ASTDecFunction &node)
    {
        // node.funType->accept(*this);

        string id = node.getID();
        string vtype=node.vtype;
        add(symtable,id,vtype);
    }
    virtual void visit(ASTDecFunctionArg &node)
    {
        node.funType->accept(*this);
        cout<<node.getID()<<" (";
        string id = node.getID();
        string vtype=node.vtype;
        add(symtable,id,vtype);
        node.funArguments->accept(*this);
        cout<<")";
    }
// Function Type
    virtual void visit(ASTFunType& node)
    {
        cout<<node.getType()<<" ";
    }
// Function Argument

    virtual void visit(ASTFunDecVar &node)
    {
        node.varType->accept(*this);
        node.varName->accept(*this);
    }
    virtual void visit(ASTFunDecArray &node)
    {
        node.varType->accept(*this);
        cout<<node.getID()<<" ";
    }
    virtual void visit(ASTFunDecVarMul &node)
    {
        node.varType->accept(*this);
        node.varName->accept(*this);
        node.funArguments->accept(*this);
    }
    virtual void visit(ASTFunDecArrayMul &node)
    {
        node.varType->accept(*this);
        cout<<node.getID()<<" ";
        node.funArguments->accept(*this);
    }
//  Variable Declaration

    virtual void visit(ASTDecVariable &node)
    {
        node.varType->accept(*this);
        node.varList->accept(*this);
    }

// VarList
    virtual void visit(ASTVarList &node)
    {
        node.accept(*this);
    }

    virtual void visit(ASTVarSingle &node)
    {
        node.varName->accept(*this);
    }
    virtual void visit(ASTVarMultiple &node)
    {
        node.varName->accept(*this);
        node.varList->accept(*this);
        
    }
    virtual void visit(ASTIVarSingle &node)
    {
        node.iVarName->accept(*this);

    }
    virtual void visit(ASTIVarMultiple &node)
    {
        node.iVarName->accept(*this);
        node.varList->accept(*this);
    }
// Initialize Variable 
    virtual void visit(ASTIVarName &node)
    {
        cout<<node.getID()<<" ";
        node.expr->accept(*this);
    }
// Var Type
    virtual void visit(ASTVarType &node)
    {
        cout<<node.getType()<<" ";
    }


// Var Name 
    virtual void visit(ASTVarName &node)
    {
       node.accept(*this);
    } 
    virtual void visit(ASTDecVarID &node)
    {
        cout<<node.getID()<<" ";
    }

    virtual void visit(ASTDecVarAID &node)
    {
        node.aid->accept(*this);
    }

// Stat

    virtual void visit(ASTStatExpr &node)
    {
        node.expr->accept(*this);
        cout<<" ; ";
    }
    virtual void visit(ASTStatAssignExpr &node)
    {
        node.assignment->accept(*this);
        cout<<" ;";
    }
    virtual void visit(ASTStatReturn &node)
    {
        cout<<"Return"<<" ";
        node.expr->accept(*this);
        cout<<" ;";
    }
// Loop
    virtual void visit(ASTLoop &node)
    {
        node.accept(*this);
    }

    virtual void visit(ASTLoopWhile &node)
    {
        cout<<"While ( ";
        node.expr->accept(*this);
        cout<<" ) \n";
        node.block->accept(*this);
    }
    virtual void visit(ASTLoopFor &node)
    {
        cout<<"For ( ";
        node.lassignment->accept(*this);
        cout<<" ; ";
        node.expr->accept(*this);
        cout<<" ; ";
        node.rassignment->accept(*this);
        cout<<" ) ";
        node.block->accept(*this);
    }

// Assignemt 
    virtual void visit(ASTAssignment &node)
    {
        node.varName->accept(*this);
        cout<<" = ";
        node.expr->accept(*this);
    }

// Conditional 

    virtual void visit(ASTCondIf &node)
    {
        cout<<"if ( ";
        node.expr->accept(*this);
        cout<<" ) ";
        node.block->accept(*this);
    }
    
    virtual void visit(ASTCondIfElse &node)
    {
        cout<<"if ( ";
        node.expr->accept(*this);
        cout<<" ) ";
        node.ifblock->accept(*this);
        cout<<" else ";
        node.elseblock->accept(*this);
    }
// Block

    virtual void visit(ASTBlock &node)
    {
        cout<<"\n{ \n";
        int i = 1;
        for (auto statement : node.statementList)
        {
            cout << "line " << i << " : ";
            statement->accept(*this);
            cout<<endl;
            i = i + 1;
        }
        cout<<"}\n";
    }
// FunctionCall

    virtual void visit(ASTFunCall &node)
    {
        node.accept(*this);
    }
    virtual void visit(ASTMethodCall &node)
    {
        cout<< node.getID()<<" ";
        cout<<" ( ) ";
    }
    virtual void visit(ASTMethodCallArg &node)
    {
        cout<< node.getID()<<" ";
        cout<<"( ";
        node.Argument->accept(*this);
        cout<<" ) ";
    }
// Argument
    virtual void visit(ASTMethodArgument &node)
    {
        int i = 1;
        for (auto expression : node.expressionList)
        {
            cout << "Arg" << i << " : ";
            expression->accept(*this);
            i = i + 1;
        }
    }
// IO
    virtual void visit(ASTIoInput &node)
    {
        cout<<"Input ";
        node.varName->accept(*this);
    }
    virtual void visit(ASTIoOutput &node)
    {
        cout<<"Output ";
        node.expr->accept(*this);
    }
    virtual void visit(ASTIoOutputBlank &node)
    {
        cout<<"Output Newline\n";
    }

// Expr

    virtual void visit(ASTExpr &node)
    {
        cout << "ASTExpr visit function" << endl;
    }

    virtual void visit(ASTExprBinary &node)
    {
        ASTExpr *left = node.getLeft();
        ASTExpr *right = node.getRight();

        left->accept(*this);
        cout << " " + node.getBin_operator();
        right->accept(*this);
    }

    virtual void visit(ASTExprTernary &node)
    {
        ASTExpr *first = node.getFirst();
        ASTExpr *second = node.getSecond();
        ASTExpr *third = node.getThird();

        first->accept(*this);
        cout << " ?";
        second->accept(*this);
        cout <<" : ";
        third->accept(*this);
    }

    virtual void visit(ASTExprID &node)
    {
        cout << " " << node.getID();
    }
    virtual void visit(ASTExprAID &node)
    {
        node.aid->accept(*this);
    }

    virtual void visit(ASTExprINT &node)
    {
        cout << " " << node.getIntLit();
    }
// AID
    virtual void visit(ASTAidLine &node)
    {
        cout<<node.getID()<<" ";
        int i=0;
        for (auto expression : node.ArrayIndexList)
        {
            cout <<  i << " : "<<" [ ";
            expression->accept(*this);
            cout<<" ] ";
            i = i + 1;
        }
    }

};