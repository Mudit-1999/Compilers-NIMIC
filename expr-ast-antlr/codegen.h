#include "ast.h"

using namespace llvm;
using namespace std; 

static LLVMContext Context;
static Module *ModuleOb = new Module("my compiler", Context);
static IRBuilder<> Builder(Context);

typedef pair<string,string> pss;  //[Var,Type]
static string fType,fName,vName,vType,scp;
static vector<pss> curfunctArg;
static map<string, Function* > AllFun;
static map<string, AllocaInst*> NamedValues;
static vector<map<string, AllocaInst*> >OldBindings;
static map<string,map<string, AllocaInst*> >FunctionBindings;
static bool RETURN,isfunction;


AllocaInst* CreateEntryBlockAlloca(Function *fooFunc, const string& VarName, const string& type) 
{
    IRBuilder<> TmpB(&fooFunc->getEntryBlock(), fooFunc->getEntryBlock().begin());
    
    AllocaInst *alloca_instruction = nullptr;
    if (type == "char") 
    {
        alloca_instruction = TmpB.CreateAlloca(Type::getInt8Ty(Context),0, VarName);
        // alloca_instruction->setAlignment(1);
    } 
    else
    {
        alloca_instruction = TmpB.CreateAlloca(Type::getInt32Ty(Context),0, VarName);
        // alloca_instruction->setAlignment(4);
    }
    return alloca_instruction;
}



GlobalVariable *createGlob(string Name,int value=0) 
{
    cout<<vType<<" "<<Name<<endl;
    if(vType=="char")
    {
        ModuleOb->getOrInsertGlobal(Name, Builder.getInt8Ty());
        GlobalVariable *gVar = ModuleOb->getNamedGlobal(Name);
        gVar->setInitializer(Builder.getInt8(value));
        // gVar->setLinkage(GlobalValue::CommonLinkage);
        // gVar->setAlignment(1);
        return gVar;

    }
    else
    {
        ModuleOb->getOrInsertGlobal(Name, Builder.getInt32Ty());
        GlobalVariable *gVar = ModuleOb->getNamedGlobal(Name);
        gVar->setInitializer(Builder.getInt32(value));
        // gVar->setLinkage(GlobalValue::CommonLinkage);
        // gVar->setAlignment(4);
        return gVar;
    }
}


BasicBlock *createBB(Function *fooFunc, string Name="entry") 
{
    return BasicBlock::Create(Context, Name, fooFunc);
}


void setFuncArgs(Function *fooFunc) 
{
    unsigned Idx = 0;
    Function::arg_iterator AI, AE;
    for (AI = fooFunc->arg_begin(), AE = fooFunc->arg_end(); AI != AE;++AI, ++Idx)
    {
        AI->setName(curfunctArg[Idx].first);
    }
    Idx = 0;
    for (auto &Arg : fooFunc->args()) 
    {
        AllocaInst *Alloca = CreateEntryBlockAlloca(fooFunc, curfunctArg[Idx].first, curfunctArg[Idx].second);
        if (curfunctArg[Idx].second=="char")
        {
            Builder.CreateStore(&Arg, Alloca);
        }
        else
        {
            Builder.CreateStore(&Arg, Alloca);
        }
        NamedValues[Arg.getName()] = Alloca;
        Idx++;
    }
    FunctionBindings[fName]=NamedValues;
    cout<<NamedValues.size()<<endl;
    for(auto [i,j]:NamedValues)
    {
        cout<<i<<" "<<j<<endl;
    }
}


Function *createFunc(IRBuilder<> &Builder, string Name) 
{
    FunctionType *funcType;
     if (fType=="bool")
        funcType = FunctionType::get(Builder.getInt1Ty(), false);
    else if (fType=="char")
        funcType = FunctionType::get(Builder.getInt8Ty(), false);
    else if (fType=="void")
        funcType = FunctionType::get(Builder.getVoidTy(), false);
    else
        funcType = FunctionType::get(Builder.getInt32Ty(), false);
    Function *fooFunc = Function::Create(funcType,Function::ExternalLinkage, Name, ModuleOb);
    BasicBlock *bbentry = createBB(fooFunc,Name+"_entry");
    Builder.SetInsertPoint(bbentry);
    AllFun[Name]=fooFunc;
    return fooFunc;
}


Function *createFuncArg(IRBuilder<> &Builder, string Name)
{
    vector<Type *> Integers;
    for(auto [i,j]:curfunctArg)
    {
        if(j=="char")
        {
            Integers.emplace_back(Type::getInt8Ty(Context));
        }
        else
        {
            Integers.emplace_back(Type::getInt32Ty(Context));
        }
    }
    FunctionType *funcType;
    if (fType=="bool")
        funcType = FunctionType::get(Builder.getInt1Ty(), Integers, false);
    else if (fType=="char")
        funcType = FunctionType::get(Builder.getInt8Ty(), Integers, false);
    else if (fType=="void")
        funcType = FunctionType::get(Builder.getVoidTy(), Integers, false);
    else
        funcType = FunctionType::get(Builder.getInt32Ty(), Integers, false);
    Function *fooFunc = Function::Create(funcType, Function::ExternalLinkage, Name, ModuleOb);
    BasicBlock *bbentry = createBB(fooFunc,Name+"_entry");
    Builder.SetInsertPoint(bbentry);
    setFuncArgs(fooFunc);
    AllFun[Name]=fooFunc;
    return fooFunc;
}



void ASTProg::generateIR() 
{
    ModuleOb->print(errs(), nullptr);
}

// Prog

Value* ASTProg::codegen() 
{
    ModuleOb->setTargetTriple("x86_64-pc-linux-gnu");
    cout<<"In Prog Codegen\n";
	Value *V=nullptr;
    if(body!=nullptr)
        V=body->codegen();
	return V; 
}


// Body 
Value* ASTBodyDecFun::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTBodyDecFun\n";
    V=dfun->codegen();
    if(body!=nullptr)
        V=body->codegen();
    return V;
}

Value* ASTBodyDecVar::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTBodyDecVar\n";
    scp="global";
    V= dvar->codegen();
    if(body!=nullptr)
        V= body->codegen();
    return V;

}

Value* ASTBodyBlockFun::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTBodyBlockFun\n";
    V=bfun->codegen();
    if(body!=nullptr)
        V=body->codegen();
    return V;

}


// Block Function 
Value * ASTBlockFunction::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTBlockFunction\n";
    
    NamedValues.clear();
    cout<<"clearing"<<endl;
    isfunction=true;
    V=dfun->codegen();
    V=block->codegen();
    return V;
}


//  DecFunction 

Value * ASTDecFunction::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTDecFunction\n";
    fType=funType->getType();
    fName= getID();
    // Declaring  a function 
    if(AllFun.count(fName)==0)
       createFunc(Builder,fName);
    return V;
}
Value * ASTDecFunctionArg::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTDecFunctionArg\n";
    fType=funType->getType();
    fName= getID();
    // Declaring a function with argument 
    V=funArguments->codegen();
    if(AllFun.count(fName)==0)
    {
        createFuncArg(Builder,fName);
    }
    else
    {
        NamedValues=FunctionBindings[fName];
    }
    
    curfunctArg.clear();
    return V;
}

// FunType 
Value * ASTFunType::codegen()
{return nullptr;}

//  FuntionArgument 
Value * ASTFunDecVar::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTFunDecVar\n";
    curfunctArg.emplace_back(varName->getID(),varType->getType());
    return V;
}

Value * ASTFunDecArray::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTFunDecArray\n";
    curfunctArg.emplace_back(getID(),varType->getType());
    return V;
}

Value * ASTFunDecVarMul::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTFunDecVarMul\n";
    curfunctArg.emplace_back(varName->getID(),varType->getType());

    funArguments->codegen();
    return V;
}

Value * ASTFunDecArrayMul::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTFunDecArrayMul\n";
    curfunctArg.emplace_back(getID(),varType->getType());
    funArguments->codegen();
    return V;
}


// Dec Variable 

Value * ASTDecVariable::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTDecVariable\n";
    vType = varType->getType();
    varList->codegen();
    return V;
}
// VarType 

Value * ASTVarType::codegen()
{return nullptr;}

// VarList

Value * ASTVarSingle::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTVarSingle\n";
    varName->codegen();
    return V;
}


Value * ASTVarMultiple::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTVarMultiple\n";
    varName->codegen();
    varList->codegen();
    return V;
}


Value * ASTIVarSingle::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTIVarSingle\n";
    iVarName->codegen();
    return V;
}


Value * ASTIVarMultiple::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTIVarMultiple\n";
    iVarName->codegen();
    varList->codegen();
    return V;
}



//  Block
Value * ASTBlock::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTBlock\n";
    RETURN=false;
    if(!isfunction)
    {
        map<string, AllocaInst*> tmp;
        OldBindings.push_back(tmp);
    }
    else
    {
        isfunction=false;
    }
    for(auto x :statementList)
    {
        x->codegen();
        if(RETURN)
            break;
    }
    if(OldBindings.size()!=0)
    {
        for(auto it:OldBindings.back())
        {
            cout<<"Previously onborad"<<endl;
            NamedValues[it.first]=it.second;
        }
        OldBindings.pop_back();
    }
    return Builder.GetInsertBlock();
}



// IVarName
Value * ASTIVarName::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTIVarName\n";
    vName=getID();
    int value;
    if (scp=="global")
    {
        if(expr->form==exprType::literal)
        {
            value=expr->getLit();
        }
        else
        {
            cout<<"Constant Initializer for Global Variable\n";
            exit(0);
        }
        createGlob(vName,value);
    }
    else
    {
        Function *fooFunc = Builder.GetInsertBlock()->getParent();
        AllocaInst *Alloca = CreateEntryBlockAlloca(fooFunc,vName, vType);
        if(NamedValues.count(vName)!=0)
        {
            auto it=OldBindings.back();
            it[vName]=NamedValues[vName];

        }
        NamedValues[vName]=Alloca;
        for(auto [i,j]:NamedValues)
        {
            cout<<i<<" "<<j<<endl;
        }
        Value *out =expr->codegen();
        V=Builder.CreateStore(out,Alloca);

    }
    return V;
}


// VarName

Value * ASTDecVarID::codegen()
{
    cout<<"ASTDecVarID"<<endl;
    vName=getID();
    if (scp=="global")
        createGlob(vName);
    else
    {
        Function *fooFunc = Builder.GetInsertBlock()->getParent();
        AllocaInst *Alloca = CreateEntryBlockAlloca(fooFunc,vName, vType);
        if(NamedValues.count(vName)!=0)
        {
            auto it=OldBindings.back();
            it[vName]=NamedValues[vName];

        }
        NamedValues[vName]=Alloca;
        for(auto [i,j]:NamedValues)
        {
            cout<<i<<" "<<j<<endl;
        }
    }
    return nullptr;
}
Value * ASTDecVarAID::codegen()
{
    Value* V;
    cout<<"ASTDecVarAID"<<endl;
    vName=getID();
    int value;
    Type* ty=Type::getInt32Ty(Context);
    if(vType=="char")
        ty=Type::getInt8Ty(Context);
    
    for(auto ind:ArrayIndexList)
    {
        value=ind->getLit();
        ty = ArrayType::get(ty,value);
        // cout<<value<<endl;
    }
    // AllocaInst* arr_alloc = new AllocaInst(ty,0,V, "", Builder.GetInsertBlock());
    
    Function *fooFunc = Builder.GetInsertBlock()->getParent();
    AllocaInst *alloca_instruction = nullptr;
    IRBuilder<> TmpB(&fooFunc->getEntryBlock(), fooFunc->getEntryBlock().begin());
    alloca_instruction = TmpB.CreateAlloca(ty,0,id);
    if(NamedValues.count(vName)!=0)
    {
        auto it=OldBindings.back();
        it[vName]=NamedValues[vName];
    }
    NamedValues[vName]=alloca_instruction;
    for(auto [i,j]:NamedValues)
    {
        cout<<i<<" "<<j<<endl;
    }
    return alloca_instruction;
}


// UseVar Name


Value * ASTUseVarName::codegen()
{
    return nullptr;
}
Value * ASTUseDecVarID::codegen()
{
    cout<<"In ASTUseDecVarID\n";
    vName=getID();
    // Try to get the value of the variable 
    Value *V = NamedValues[vName];
    if (V == nullptr) 
    {
        V = ModuleOb->getNamedGlobal(vName);
    }
    if (V == nullptr) 
    {
        // compilerConstructs->errors++;
        auto error_msg = "Unknown Variable name " + vName;
        cout << error_msg << endl; 
        exit(0);
    }
    
    return V;
}
Value * ASTUseDecVarAID::codegen()
{
    cout<<"In ASTUseDecVarAID\n";
    vName=getID();
    Value *V = NamedValues[vName];
    vector<Value *> array_index;
    array_index.push_back(Builder.getInt32(0));
    Value  *offset;
    if (V == nullptr) 
    {
        cout<<"hi"<<endl;
        V = ModuleOb->getNamedGlobal(vName);
    }
    for(auto ind:ArrayIndexList)
    {
        array_index.push_back(ind->codegen());
    }
    cout<<vName<<V<<endl;
    V = Builder.CreateGEP(V, array_index,"Index");
    return V;
}



// Stat

Value * ASTStatExpr::codegen()
{
    cout<<"In ASTStatExpr\n";
    Value *V=nullptr;
    return V;
}


Value * ASTStatAssignExpr::codegen()
{
    cout<<"In ASTStatAssignExpr\n";
    assignment->codegen();
    // store in the variable
    Value *V=nullptr;
    return V;
}

Value * ASTStatReturn::codegen()
{
    RETURN=true;
    Value *V=nullptr;
    cout<<"In ASTStatReturn\n";
    if (expr!=nullptr)
    { 
        V=expr->codegen();
        return Builder.CreateRet(V);
    }
    return Builder.CreateRetVoid();
}

Value * ASTStatDecVar::codegen()
{
    Value *V=nullptr;
    scp="local";
    cout<<"In ASTStatDecVar\n";
    dvar->codegen();
    scp="global";
    return V;
}

// Loop

Value * ASTLoop::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTLoop\n";
    return V;
}

Value * ASTLoopFor::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTLoopFor\n";
    Function *fooFunc = Builder.GetInsertBlock()->getParent();
    
    BasicBlock *StartBB = createBB(fooFunc, "startloop");
    BasicBlock *LoopBB = createBB(fooFunc, "loop");
    BasicBlock *UpdateBB = createBB(fooFunc, "updateloop");
    BasicBlock *AfterBB = createBB(fooFunc, "afterloop");
    Builder.CreateBr(StartBB);
    Builder.SetInsertPoint(StartBB);
    Value* Init=lassignment->codegen();
    Value* ICond=expr->codegen();
    Builder.CreateCondBr(ICond, LoopBB, AfterBB);
    Builder.SetInsertPoint(LoopBB);
    block->codegen();
    Builder.CreateBr(UpdateBB);
    Builder.SetInsertPoint(UpdateBB);
    Value* NextVal = rassignment->codegen();
    Value* ECond=expr->codegen();
    Builder.CreateCondBr(ECond, LoopBB, AfterBB);
    Builder.SetInsertPoint(AfterBB);
    return V;
}

Value * ASTLoopWhile::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTLoopWhile\n";
    Function *fooFunc = Builder.GetInsertBlock()->getParent();

    BasicBlock *StartBB = createBB(fooFunc, "startloop");
    BasicBlock *LoopBB = createBB(fooFunc, "loop");
    BasicBlock *AfterBB = createBB(fooFunc, "afterloop");
    Builder.CreateBr(StartBB);
    Builder.SetInsertPoint(StartBB);
    Value* ICond=expr->codegen();
    Builder.CreateCondBr(ICond, LoopBB, AfterBB);
    Builder.SetInsertPoint(LoopBB);
    block->codegen();
    Builder.CreateBr(StartBB);
    Builder.SetInsertPoint(AfterBB);
    return V;
}

// Assignment

Value * ASTAssignment::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTAssignment\n";
    V=useVarName->codegen();
    Value* out=expr->codegen();
    return Builder.CreateStore(out,V);
}

// CondIf


Value * ASTCondIf::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTCondIf\n";
    Function *fooFunc = Builder.GetInsertBlock()->getParent();
    Value * Compare = expr->codegen();
    Value *Condtn = Builder.CreateICmpNE(Compare, Builder.getInt1(false),"ifcond");
    BasicBlock *ThenBB = createBB(fooFunc, "then");
    BasicBlock *MergeBB = createBB(fooFunc, "ifcont");
    Builder.CreateCondBr(Condtn, ThenBB, MergeBB);
    Builder.SetInsertPoint(ThenBB);
    block->codegen();
    Builder.CreateBr(MergeBB);
    Builder.SetInsertPoint(MergeBB);
    return V;
}


Value * ASTCondIfElse::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTCondIfElse\n";
    Function *fooFunc = Builder.GetInsertBlock()->getParent();
    Value * Compare = expr->codegen();
    Value *Condtn = Builder.CreateICmpNE(Compare, Builder.getInt1(false),"ifcond");
    BasicBlock *ThenBB = createBB(fooFunc, "then");
    BasicBlock *ElseBB = createBB(fooFunc, "else");
    BasicBlock *MergeBB = createBB(fooFunc, "ifcont");
    Builder.CreateCondBr(Condtn, ThenBB, ElseBB);
    
    Builder.SetInsertPoint(ThenBB);
    ifblock->codegen();
    Builder.CreateBr(MergeBB);

    Builder.SetInsertPoint(ElseBB);
    elseblock->codegen();
    Builder.CreateBr(MergeBB);
    
    unsigned PhiBBSize = 2;
    Builder.SetInsertPoint(MergeBB);
    return V;
}


// Funcall

Value * ASTFunCall::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTFunCall\n";
    return V;
}


Value * ASTMethodCall::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTMethodCall\n";
    string id=getID();
    Function* fcall=ModuleOb->getFunction(id);
    V = Builder.CreateCall(fcall);
    return V;
}


Value * ASTMethodCallArg::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTMethodCallArg\n";
    string id=getID();
    Function* fcall=ModuleOb->getFunction(id);
    vector<Value *> Args;
    for(auto expression:Argument->expressionList)
    {
        Args.emplace_back(expression->codegen());
    }
    V = Builder.CreateCall(fcall,Args);
    return V;
}

// IO

Value * ASTIoInput::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTIoInput\n";
    return V;
}
Value * ASTMethodArgument::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTMethodArgument\n";
    return V;
}
Value * ASTIoOutput::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTIoOutput\n";
    return V;
}
Value * ASTIoOutputBlank::codegen()
{
    Value *V=nullptr;
    cout<<"InASTIoOutputBlank\n";
    return V;
}

// Expr 

Value * ASTExprBinary::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTExprBinary\n";
    ASTExpr * L=getLeft();
    ASTExpr * R=getRight();
    Value *left =L->codegen();
    Value *right=R->codegen();
    string op=getBin_operator();
    if (left ==nullptr)
    {
        //errors++;
        auto error_msg = "Error in left operand of " + op;
        cout<<error_msg<<endl;
        return nullptr; 
    } else if (right == nullptr) 
    {
        //errors++;
        auto error_msg = "Error in right operand of " + op;
        cout<<error_msg<<endl;
        return nullptr; 
    }

    if (op == "+") 
        V = Builder.CreateAdd(left, right, "add");
    else if (op == "-") 
        V = Builder.CreateSub(left, right, "sub");
    else if (op == "*") 
        V = Builder.CreateMul(left, right, "mult");
    else if (op == "/") 
        V = Builder.CreateSDiv(left, right, "div");
    else if (op == "%") 
        V = Builder.CreateSRem(left, right, "mod");
    else if (op == "<") 
        V = Builder.CreateICmpSLT(left, right, "lt");
    else if (op == ">") 
        V = Builder.CreateICmpSGT(left, right, "gt");
    else if (op == "<=") 
        V = Builder.CreateICmpSLE(left, right, "le");
    else if (op == ">=") 
        V = Builder.CreateICmpSGE(left, right, "ge");
    else if (op == "==") 
        V = Builder.CreateICmpEQ(left, right, "equality");
    else if (op == "!=") 
        V = Builder.CreateICmpNE(left, right, "inequality");
    else if (op == "&") 
        V = Builder.CreateAnd(left, right, "band");
    else if (op == "|") 
        V = Builder.CreateOr(left, right, "bor");
    else if (op == "^") 
        V = Builder.CreateXor(left, right, "bxor");
    else if(op=="&&")
    {
        Value * Compare = Builder.CreateAnd(left, right, "land");
        V = Builder.CreateICmpNE(Compare, Builder.getInt1(false),"andcond");
    }
    else if(op=="||")
    {
        Value * Compare = Builder.CreateOr(left, right, "lor");
        V= Builder.CreateICmpNE(Compare, Builder.getInt1(false),"orcond");
    }
    return V;
}

Value * ASTExprTernary::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTExprTernary\n";
    return V;
}

Value * ASTExprID::codegen()
{
    vName=getID();
    // Try to get the value of the variable 
    Value *V = NamedValues[vName];
    if (V == nullptr) 
    {
        V = ModuleOb->getNamedGlobal(vName);
    }
    if (V == nullptr) 
    {
        // compilerConstructs->errors++;
        for(auto [i,j]:NamedValues)
        {
            cout<<i<<" "<<j<<endl;
        }
        auto error_msg = "Unknown Variable name " + vName;
        cout << error_msg << endl; 
        exit(0);
    }
    cout<<"In ASTExprID\n";
    return Builder.CreateLoad(V);
}

Value * ASTExprAID::codegen()
{
    cout<<"In ASTExprAID\n";
    vName=getID();
    Value *V = NamedValues[vName];
    vector<Value *> array_index;
    array_index.push_back(Builder.getInt32(0));
    Value  *offset;
    if (V == nullptr) 
    {
        cout<<"hi"<<endl;
        V = ModuleOb->getNamedGlobal(vName);
    }
    for(auto ind:ArrayIndexList)
    {
        array_index.push_back(ind->codegen());
    }
    cout<<vName<<V<<endl;
    V = Builder.CreateGEP(V, array_index,"Index");
    return V;
}

Value * ASTExprINT::codegen()
{
    Value *V=nullptr;
    cout<<"In ASTExprINT\n";
    int value= getLit();
    cout<<value<<endl;
    return ConstantInt::get(Context, APInt(32, static_cast<uint64_t>(value)));
}
Value * ASTExprCHAR::codegen()
{
    cout<<"In ASTExprCHAR\n";
    char value=getLit();
    return ConstantInt::get(Context, APInt(8, value));
}   