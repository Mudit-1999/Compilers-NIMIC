#ifndef __AST_H__
#define __AST_H__
#include <bits/stdc++.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/Support/raw_ostream.h>
using namespace llvm;
using namespace std; 

enum exprType {
    binary = 1, loca = 2, literal = 3, enclExpr = 4, ternary = 5
};



class ASTProg;

class ASTBody;
class ASTBodyDecFun;
class ASTBodyDecVar;
class ASTBodyBlockFun;

class ASTBlockFunction;

class ASTDecFunction;
class ASTDecFunctionArg;

class ASTFunType;

class ASTFunArgument;
class ASTFunDecVar;
class ASTFunDecArray;
class ASTFunDecVarMul;
class ASTFunDecArrayMul;

class ASTDecVariable;

class ASTVarList;
class ASTVarSingle;
class ASTVarMultiple;
class ASTIVarSingle;
class ASTIVarMultiple;

class ASTIVarName;

class ASTVarType;

class ASTVarName;
class ASTDecVarID;
class ASTDecVarAID;

class ASTUseVarName;
class ASTUseDecVarID;
class ASTUseDecVarAID;

class ASTStatExpr;
class ASTStatAssignExpr;
class ASTStatReturn;
class ASTStatDecVar;

class ASTLoop;
class ASTLoopFor;
class ASTLoopWhile;

class ASTAssignment;


class ASTCondIf;
class ASTCondIfElse;

class ASTBlock;

class ASTFunCall;
class ASTMethodCall;
class ASTMethodCallArg;

class ASTIoInput;
class ASTMethodArgument;
class ASTIoOutput;
class ASTIoOutputBlank;


class ASTExpr;
class ASTExprBinary;
class ASTExprTernary;
class ASTExprID;
class ASTExprAID;
class ASTExprINT;
class ASTExprCHAR;


class ASTvisitor
{
public:
    virtual void visit(ASTProg &node) = 0;

    virtual void visit(ASTBody &node) = 0;
    virtual void visit(ASTBodyDecFun &node) = 0;
    virtual void visit(ASTBodyDecVar &node) = 0;
    virtual void visit(ASTBodyBlockFun &node) = 0;
    
    virtual void visit(ASTBlockFunction &node) = 0;

    virtual void visit(ASTDecFunction &node)=0;
    virtual void visit(ASTDecFunctionArg &node)=0;

    virtual void visit(ASTFunType &node)=0;

    virtual void visit(ASTFunArgument &node)=0;
    virtual void visit(ASTFunDecVar &node)=0;
    virtual void visit(ASTFunDecVarMul &node)=0;
    virtual void visit(ASTFunDecArray &node)=0;
    virtual void visit(ASTFunDecArrayMul &node)=0;

    virtual void visit(ASTStatExpr &node) = 0;
    virtual void visit(ASTStatAssignExpr &node) = 0;
    virtual void visit(ASTStatReturn &node) =0;
    virtual void visit(ASTStatDecVar &node) =0;

    virtual void visit(ASTLoop &node) =0;
    virtual void visit(ASTLoopFor &node)=0;
    virtual void visit(ASTLoopWhile &node)=0;
    
    virtual void visit(ASTAssignment &node) =0;

    virtual void visit(ASTBlock &node)=0;

    virtual void visit(ASTCondIf &node)=0;
    virtual void visit(ASTCondIfElse &node) =0;
    
    virtual void visit(ASTFunCall &node) =0;
    virtual void visit(ASTMethodCall &node) =0;
    virtual void visit(ASTMethodCallArg &node) =0;

    virtual void visit(ASTDecVariable &node) =0;
    virtual void visit(ASTVarType &node)=0;
    virtual void visit(ASTVarList &node)=0;
    virtual void visit(ASTVarName &node)=0;
    virtual void visit(ASTIVarName &node)=0;

    virtual void visit(ASTVarSingle &node)=0;
    virtual void visit(ASTVarMultiple &node)=0;
    virtual void visit(ASTIVarSingle &node)=0;
    virtual void visit(ASTIVarMultiple &node)=0;

    virtual void visit(ASTDecVarID &node) =0;
    virtual void visit(ASTDecVarAID &node) =0;

    virtual void visit(ASTUseVarName &node)=0;
    virtual void visit(ASTUseDecVarID &node) =0;
    virtual void visit(ASTUseDecVarAID &node) =0;
    

    virtual void visit(ASTMethodArgument &node) =0;

    virtual void visit(ASTIoInput &node) = 0;
    virtual void visit(ASTIoOutput &node) = 0;
    virtual void visit(ASTIoOutputBlank &node) = 0;


    virtual void visit(ASTExprBinary &node) = 0;
    virtual void visit(ASTExprTernary &node) = 0;
    virtual void visit(ASTExprID &node) = 0;
    virtual void visit(ASTExprAID &node) = 0;
    virtual void visit(ASTExprINT &node) = 0;
    virtual void visit(ASTExprCHAR &node) = 0;
    virtual void visit(ASTExpr &node) = 0;
    
};

class ASTnode
{
public:
    virtual ~ASTnode()
    {}
    virtual Value* codegen()=0;
    string ntype; 
    virtual void accept(ASTvisitor &V) = 0;
};


class ASTStat : public ASTnode
{
public:
    

    virtual ~ASTStat()
    {}
    virtual Value* codegen() =0;
    virtual void accept(ASTvisitor &V) = 0;
    
};



class ASTAssignment :public ASTnode
{
public:
    

    ASTUseVarName* useVarName;
    ASTExpr *expr;

    Value * codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTFunCall: public ASTnode
{
public:
    virtual ~ASTFunCall()
    {}
    virtual Value * codegen()=0;
    virtual void accept(ASTvisitor &V) = 0;
};


class ASTVarType: public ASTnode
{
    string vtype;
    
public:

    ASTVarType(string vtype) : vtype(vtype) {}

    string getType()
    {
        return vtype;
    }

    Value * codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};
// ComVarName
class ASTVarName:public ASTnode
{
public:

    virtual ~ASTVarName()
    {}
    virtual Value * codegen()=0;
    virtual string getID()=0;
    virtual void accept(ASTvisitor &V) = 0;

};

class ASTDecVarID : public ASTVarName
{
    string id;
public:
    

    ASTDecVarID(string id) : id(id) {}

    virtual string getID()
    {
        return id;
    }
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTDecVarAID :public ASTVarName
{
    string id;
public:

    
    std::vector<ASTExpr*> ArrayIndexList;
    ASTDecVarAID(string id): id(id){}
   
    Value* codegen();
    virtual string getID()
    {
        return id;
    }
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


class ASTUseVarName:public ASTnode
{
public:

    virtual ~ASTUseVarName()
    {}

    virtual Value * codegen()=0;
    virtual string getID()=0;
    virtual void accept(ASTvisitor &V) = 0;

};


class ASTUseDecVarID : public ASTUseVarName
{
    string id;
public:

    
    ASTUseDecVarID(string id) : id(id) {}
    virtual string getID()
    {
        return id;
    }

    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTUseDecVarAID :public ASTUseVarName
{
    string id;
public:

    
    std::vector<ASTExpr*> ArrayIndexList;
    ASTUseDecVarAID(string id): id(id){}
    virtual string getID()
    {
        return id;
    }

    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};



class ASTLoop :public ASTnode
{
public:

    virtual ~ASTLoop()
    {}

    virtual Value * codegen()=0;
    virtual void accept(ASTvisitor &v)=0;
};


class ASTLoopFor :public ASTnode
{
public:
    

    ASTAssignment *lassignment,*rassignment;
    ASTExpr *expr;
    ASTBlock *block;
    
    Value * codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTLoopWhile :public ASTnode
{
public:
    

    ASTExpr *expr;
    ASTBlock *block;
    
    Value * codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


class ASTIVarName:public ASTnode
{
    string id;
public:
    

    ASTIVarName(string id) : id(id) {}
    string getID()
    {
        return id;
    }
    ASTExpr *expr;
    
    Value * codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVarList:public ASTnode
{
public:
    
    virtual ~ASTVarList()
    {}

    virtual Value * codegen() =0;
    virtual void accept(ASTvisitor &V) = 0;

};

class ASTDecVariable: public ASTnode
{
public:
    
    ASTVarType * varType;
    ASTVarList * varList;
    
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


class ASTVarSingle: public ASTVarList
{
public:
    

    ASTVarName *varName;

    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTVarMultiple : public ASTVarList
{
public:
    

    ASTVarName *varName;
    ASTVarList *varList;

    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


class ASTIVarSingle: public ASTVarList
{
public:
    

    ASTIVarName *iVarName;

    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTIVarMultiple : public ASTVarList
{
public:
    
    ASTIVarName *iVarName;
    ASTVarList *varList;

    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};











class ASTFunArgument: public ASTnode
{
public:
    virtual ~ASTFunArgument()
    {}
    virtual void accept(ASTvisitor &V) = 0;
    virtual Value* codegen() = 0;

};



class ASTFunDecVar: public ASTFunArgument
{
public:
    

    ASTVarName *varName;
    ASTVarType *varType;
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};



class ASTFunDecArray : public ASTFunArgument
{
    string id; 
public:
    
    ASTFunDecArray(string id) : id(id) {}
    string getID()
    {
        return id;
    }
    ASTVarType *varType;
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


class ASTFunDecVarMul: public ASTFunArgument
{
public:
    

    ASTVarName *varName;
    ASTVarType *varType;
    ASTFunArgument* funArguments;
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTFunDecArrayMul : public ASTFunArgument
{
    string id; 
public:
    

    ASTFunDecArrayMul(string id) : id(id) {}
    string getID()
    {
        return id;
    }
    ASTVarType *varType;
    ASTFunArgument* funArguments;
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};







class ASTExpr : public ASTnode
{
public:
    

    virtual ~ASTExpr()
    {}
    exprType form;
    virtual Value* codegen()=0;
    virtual int getLit()=0;
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTMethodArgument : public ASTnode
{
public:
    
    std::vector <ASTExpr *> expressionList;
    
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTBlock : public ASTnode
{
public:
    
    std::vector<ASTStat *> statementList;
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTCondIf :public ASTnode
{
public:
    
    ASTExpr* expr;
    ASTBlock *block;
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTCondIfElse :public ASTnode
{
public:
    

    ASTExpr *expr;
    ASTBlock *ifblock,*elseblock;
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTIoInput :public ASTStat
{
public:
    

    ASTUseVarName *useVarName;
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTIoOutput :public ASTStat
{
public:
    Value* codegen(); 

    ASTExpr *expr;
    
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTIoOutputBlank :public ASTStat
{
public:
    

    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
    
};



class ASTStatExpr : public ASTStat
{
public:
    

    ASTExpr *expr;
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTStatReturn :public ASTStat
{
public:
    

    ASTExpr* expr;
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTStatDecVar :public ASTStat
{
public:
    

    ASTDecVariable* dvar;
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTStatAssignExpr : public ASTStat
{
public:
    

    ASTAssignment * assignment;
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};



class ASTExprBinary : public ASTExpr
{

    std::string bin_operator;
    // lhs and rhs can be of any type.
    // So we need to use BaseAST
    ASTExpr *left;
    ASTExpr *right;

public:
    

    // Constructor to initialize binary operator,
    // lhs and rhs of the binary expression.
    ASTExprBinary(std::string op, ASTExpr *_left, ASTExpr *_right) : bin_operator(op), left(_left), right(_right) {}

    int getLit()
    {
        return 0;
    }
    ASTExpr *getLeft()
    {
        return left;
    }

    ASTExpr *getRight()
    {
        return right;
    }

    std::string getBin_operator()
    {
        return bin_operator;
    }
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTExprTernary : public ASTExpr
{


    ASTExpr *first;
    ASTExpr *second;
    ASTExpr *third;

public:
    

    ASTExprTernary(ASTExpr *first, ASTExpr *second, ASTExpr *third) : first(first), second(second), third(third) {}

    int getLit()
    {
        return 0;
    }
    
    ASTExpr *getFirst()
    {
        return first;
    }

    ASTExpr *getSecond()
    {
        return second;
    }

    ASTExpr *getThird()
    {
        return third;
    }

    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTExprINT : public ASTExpr
{

    int lit;

public:
    

    ASTExprINT(int lit) : lit(lit) {}

    int getLit()
    {
        return lit;
    }

    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTExprCHAR : public ASTExpr
{

char lit;

public:
    

    ASTExprCHAR(char lit) : lit(lit) {}
    int getLit()
    {
        return (int)lit;
    }

    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


class ASTExprID : public ASTExpr
{

    string id;

public:
    

    ASTExprID(string id) : id(id) {}
    int getLit()
    {
        return 0;
    }

    string getID()
    {
        return id;
    }

    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};


class ASTExprAID :public ASTExpr
{
    string id;
public:

    
    std::vector<ASTExpr*> ArrayIndexList;
    ASTExprAID(string id): id(id){}
    string getID()
    {
        return id;
    }
    int getLit()
    {
        return 0;
    }

    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


class ASTMethodCall :public ASTnode
{
    string id;
public:
    

    ASTMethodCall(string id): id(id){}

    string getID()
    {
        return id;
    }

    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTMethodCallArg :public ASTnode
{
    string id;
public:
    

    ASTMethodCallArg(string id): id(id){}
    string getID()
    {
        return id;
    }
    ASTMethodArgument *Argument;
    
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


// FunType

class ASTFunType: public ASTnode
{
    string vtype;
public:
    ASTFunType(string vtype) : vtype(vtype) {}

    string getType()
    {
        return vtype;
    }
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


// DecFunction 
class ASTDeclarationFunction: public ASTnode
{
public:

    virtual ~ASTDeclarationFunction()
    {}
    virtual Value* codegen()=0;
    virtual void accept(ASTvisitor &V) = 0;
};

class ASTDecFunction :public ASTDeclarationFunction
{
    string id;
public:
    ASTDecFunction(string id) : id(id) {}
    
    ASTFunType *funType;
    string getID()
    {
        return id;
    }

    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }

};

class ASTDecFunctionArg :public ASTDeclarationFunction
{
    string id;
public:
    ASTDecFunctionArg(string id) : id(id) {}
    
    string getID()
    {
        return id;
    }
    ASTFunType *funType;
    ASTFunArgument *funArguments;
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};



// BlockFunction 
class ASTBlockFunction :public ASTnode
{
public: 
    

    ASTDeclarationFunction *dfun;
    ASTBlock *block;
    
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }    
};



// Body
class ASTBody : public ASTnode
{
public:
    
    virtual ~ASTBody()
    {}
    virtual Value* codegen()=0;
    virtual void accept(ASTvisitor &V) =0;
};

class ASTBodyDecFun: public ASTBody
{
public:
    
    ASTDeclarationFunction * dfun;
    ASTBody *body;
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};

class ASTBodyDecVar: public ASTBody
{
public:
    

    ASTDecVariable * dvar;
    ASTBody *body;
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};
class ASTBodyBlockFun: public ASTBody
{
public:
    

    ASTBlockFunction * bfun;
    ASTBody *body;
    Value* codegen();
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


// Program
class ASTProg : public ASTnode
{
public:
    

    ASTBody *body;
    void generateIR();
    Value* codegen(); 
    virtual void accept(ASTvisitor &v)
    {
        v.visit(*this);
    }
};


class ASTContext
{
public:

    ASTnode *root;
    ~ASTContext()
    {
        clearAST();
    }

    // free all saved expression trees
    void clearAST()
    {
        delete root;
    }
};

#endif // __AST_H__

