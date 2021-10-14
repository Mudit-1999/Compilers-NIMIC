#include <bits/stdc++.h>
#include "SymbolTable.h"
using namespace std;

SymbolTable *symtable ;

typedef pair<string,string> pss;  //[Var,Type]
string curType,curVar;
string curMethod;
vector<pss> curMethodArg;
map<string, vector<pss> > MethodArg;

void call_error(string a,string b)
{
    cout<<"Type checking fail: First:"<<a<<" Second:"<<b<<endl;
    exit(0);
}

class PostFixVisitor : public ASTvisitor
{
public:
// Prog
    virtual void visit(ASTProg &node)
    {
        cout << "PostFixVisit traversal invoked" << endl;
        symtable = new SymbolTable();
        symtable->flag=true;
        if(node.body!=nullptr)
            node.body->accept(*this);
        for(auto [i,t]:symtable->scope)
        {
            cout<<i<<" "<<t<<endl;
        }
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
        if(node.body!=nullptr)
            node.body->accept(*this);
    }

    virtual void visit(ASTBodyDecVar &node)
    {
        node.dvar->accept(*this);
        cout<<endl;
        if(node.body!=nullptr)
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
        // initializing scope;
        symtable=addScope(symtable,curMethod);
        for(auto [var,type]:MethodArg[curMethod])
        {
            cout<<endl;
            cout<<type<<"  "<<var;
            add(symtable,type,var);
        }
        node.block->accept(*this);
        symtable=symtable->parentptr;
    }

// FunctionDeclaration
    virtual void visit(ASTFunArgument &node)
    {
        node.accept(*this);
    }
    virtual void visit(ASTDecFunction &node)
    {
        node.funType->accept(*this);
        curVar=node.getID();
        curMethod=curVar;
        curMethodArg.clear();
        if(!MethodArg.count(curMethod))
        {
            add(symtable,curType,curVar);
        }
        MethodArg[curMethod]=curMethodArg;
        cout<<node.getID()<<" ( ) ";

    }
    virtual void visit(ASTDecFunctionArg &node)
    {
        node.funType->accept(*this);
        cout<<node.getID()<<" ( ";
        curVar=node.getID();
        curMethod=curVar;
        curMethodArg.clear();
        if(!MethodArg.count(curMethod))
        {
            add(symtable,curType,curMethod);
        }
        symtable=addScope(symtable,curMethod);
        node.funArguments->accept(*this);
        MethodArg[curMethod]=curMethodArg;
        symtable=symtable->parentptr;
        cout<<")";
        symtable->childptr.erase(curMethod);
        // cout<<"\nErase" <<" "<<curMethod<<endl;

    }
// Function Type
    virtual void visit(ASTFunType& node)
    {
        cout<<node.getType()<<" ";
        curType=node.getType();
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
        curVar=node.getID();
        add(symtable,curType,curVar);
        curMethodArg.emplace_back(curVar,curType);
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
        curVar=node.getID();
        add(symtable,curType,curVar);
        curMethodArg.emplace_back(curVar,curType);
        for(auto [var,type]:curMethodArg)
        {
            cout<<type<<"  "<<var<<endl;
            // add(symtable,type,var);
        }
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
// Var Type
    virtual void visit(ASTVarType &node)
    {
        cout<<node.getType()<<" ";
        curType=node.getType();
        node.ntype=node.getType();
    }

// Initialize Variable 
    virtual void visit(ASTIVarName &node)
    {
        cout<<node.getID()<<" ";
        curVar=node.getID();
        add(symtable,curType,curVar);
        node.expr->accept(*this);
        if(node.expr->ntype!=curType)
            call_error(node.expr->ntype,curType);
        node.ntype=curType;
    }


// Var Name 
    virtual void visit(ASTVarName &node)
    {
       node.accept(*this);
    } 
    virtual void visit(ASTDecVarID &node)
    {
        curVar=node.getID();
        add(symtable,curType,curVar);
        curMethodArg.emplace_back(curVar,curType);
        cout<<node.getID()<<" ";
    }

    virtual void visit(ASTDecVarAID &node)
    {
        curVar=node.getID();
        add(symtable,curType,curVar);
        curMethodArg.emplace_back(curVar,curType);
        cout<<node.getID()<<" ";
        int i=0;
        for (auto expression : node.ArrayIndexList)
        {
            cout <<  i << " : "<<" [ ";
            if(expression)
                expression->accept(*this);
            cout<<" ] ";
            i = i + 1;
        }
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
        string tmptype=search(symtable,curMethod);
        if(node.expr)
        {
            node.expr->accept(*this);
            if(tmptype!=node.expr->ntype)
                call_error(tmptype,node.expr->ntype);

        }
        else
        {
            if(tmptype!="void")
                call_error(tmptype,"void");

        }
        
        cout<<" ;";
    }
    virtual void visit(ASTStatDecVar &node)
    {
        node.dvar->accept(*this);
    }
// Loop
    virtual void visit(ASTLoop &node)
    {
        node.accept(*this);
    }

    virtual void visit(ASTLoopWhile &node)
    {

        cout<<"While ( ";
        symtable=addScope(symtable,"While");
        node.expr->accept(*this);
        cout<<" ) \n";
        node.block->accept(*this);
        symtable=symtable->parentptr;
        symtable->childptr.erase("While");
    }
    virtual void visit(ASTLoopFor &node)
    {
        cout<<"For ( ";
        symtable=addScope(symtable,"For");
        node.lassignment->accept(*this);
        cout<<" ; ";
        node.expr->accept(*this);
        cout<<" ; ";
        node.rassignment->accept(*this);
        cout<<" ) ";
        node.block->accept(*this);
        symtable=symtable->parentptr;
        symtable->childptr.erase("For");

        
    }

// Assignment 
    virtual void visit(ASTAssignment &node)
    {
        node.useVarName->accept(*this);
        cout<<" = ";
        node.expr->accept(*this);

        if(node.useVarName->ntype!=node.expr->ntype)
        {
            call_error(node.useVarName->ntype,node.expr->ntype);
        }
        node.ntype=node.expr->ntype;

    }

// Conditional 

    virtual void visit(ASTCondIf &node)
    {
        cout<<"if ( ";
        symtable=addScope(symtable,"If");
        node.expr->accept(*this);
        cout<<" ) ";
        node.block->accept(*this);
        symtable=symtable->parentptr;

    }
    
    virtual void visit(ASTCondIfElse &node)
    {
        cout<<"if ( ";
        symtable=addScope(symtable,"If");
        node.expr->accept(*this);
        cout<<" ) ";
        node.ifblock->accept(*this);
        symtable=symtable->parentptr;
        cout<<" else ";
        symtable=addScope(symtable,"Else");
        node.elseblock->accept(*this);
        symtable=symtable->parentptr;

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
        string id=node.getID();
        node.ntype=search(symtable,id);
        symtable=addScope(symtable,id);  // better to use change scope
        cout<< node.getID()<<" ";
        cout<<" ( ) ";
        symtable=symtable->parentptr;
    }
    virtual void visit(ASTMethodCallArg &node)
    {
        string id=node.getID();
        node.ntype=search(symtable,id);
        symtable=addScope(symtable,id);  // better to use change scope
        cout<< node.getID()<<" ";
        cout<<"( ";
        node.Argument->accept(*this);
        cout<<" ) ";
        
        auto argids=MethodArg[id];
        for(int i=0;i<argids.size();i++)
        {
            auto expresstion=node.Argument->expressionList[i];
            string tmptype = argids[i].second;
            if(expresstion->ntype!=tmptype)
            {
                cout<<expresstion->ntype<<" "<<argids[i].second<<" "<<argids[i].first<<endl;
                call_error(expresstion->ntype,tmptype);
            }
        }
        symtable=symtable->parentptr;
    }
// Argument
    virtual void visit(ASTMethodArgument &node)
    {
        int i = 1;
        for (auto expression : node.expressionList)
        {
            cout << "Arg" << i << " : ";
            expression->accept(*this);
            cout<<" ";
            i = i + 1;
        }
    }
// IO
    virtual void visit(ASTIoInput &node)
    {
        cout<<"Input ";
        node.useVarName->accept(*this);
    }
    virtual void visit(ASTIoOutput &node)
    {
        cout<<"Output ";
        if(node.expr)
            node.expr->accept(*this);
    }
    virtual void visit(ASTIoOutputBlank &node)
    {
        cout<<"Output Newline\n";
    }

// Use Var Name 
    virtual void visit(ASTUseVarName &node)
    {
       node.accept(*this);
    } 
    virtual void visit(ASTUseDecVarID &node)
    {
        cout<<node.getID()<<" ";
        string id =node.getID();
        node.ntype=search(symtable,id);
    }

    virtual void visit(ASTUseDecVarAID &node)
    {
        cout<<node.getID()<<" [";
        string id=node.getID();
        node.ntype=search(symtable,id);
        int i=0;
        for (auto expression : node.ArrayIndexList)
        {
            cout <<  i << " : "<<" [ ";
            expression->accept(*this);
            cout<<" ] ";
            i = i + 1;
        }
    }


// Expr

    virtual void visit(ASTExpr &node)
    {
        cout << "ASTExpr visit function" << endl;
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
        if(second->ntype!=third->ntype)
        {
            call_error(second->ntype,third->ntype);
        }
        node.ntype=second->ntype;
    }

    virtual void visit(ASTExprBinary &node)
    {
        ASTExpr *left = node.getLeft();
        ASTExpr *right = node.getRight();

        left->accept(*this);
        cout << " " + node.getBin_operator();
        right->accept(*this);

        if((left->ntype)!= (right->ntype) )
        {
            call_error(left->ntype,right->ntype);
        }
        node.ntype=left->ntype;
    }



    virtual void visit(ASTExprID &node)
    {
        string id=node.getID();
        node.ntype=search(symtable,id); 
        cout << " " << node.getID();
        // cout<<id<<" "<<node.ntype<<endl;

    }
    virtual void visit(ASTExprAID &node)
    {
        cout<<node.getID()<<" ";
        string id =node.getID();
        node.ntype=search(symtable,id);
        int i=0;
        for (auto expression : node.ArrayIndexList)
        {
            cout <<  i << " : "<<" [ ";
            expression->accept(*this);
            cout<<" ] ";
            i = i + 1;
        }
        
    }

    virtual void visit(ASTExprINT &node)
    {
        cout << " " << node.getLit();
        node.ntype="int";
        // cout<<node.ntype<<endl;
    }
    virtual void visit(ASTExprCHAR &node)
    {
        node.ntype="char";
        cout << " " << node.getLit();
    }
};