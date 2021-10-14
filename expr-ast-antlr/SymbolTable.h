#include <bits/stdc++.h>

using namespace std;


class SymbolTable
{
public:
    bool flag=false;
    map <string,string> scope;     //ID // VarType 
    map< string, SymbolTable * > childptr;
    SymbolTable *parentptr=NULL;
};


SymbolTable* addScope(SymbolTable *symtable, string methodname)
{
    // for(auto [i,t]:symtable->childptr)
    // {
    //     cout<<"\nMethod Name "<<i;
    // }   
    if(symtable->childptr.count(methodname))
    {
        cout<<"\nMethod Scope exists " << methodname<<endl;
        exit(0);
        return symtable->childptr[methodname];
    }
    SymbolTable * node = new SymbolTable();
    node->parentptr=symtable;
    symtable->childptr[methodname]=node;
    return node;
}

bool add(SymbolTable *symtable, string vtype ,string id )
{

    if( symtable->scope.find(id)!=symtable->scope.end())
    {
        cout<<"Redeclaration of Variable "<<id<<endl;
        exit(0);
    }
    symtable->scope[id]=vtype;
    return true;

}


string search(SymbolTable *cur, string id)
{
    if(cur->flag)
    {
        if(cur->scope.count(id))
        {
            return cur->scope[id] ;
        }
        else
        {
            cout<<"Variable Not Declared "<<id<<endl;
            exit(0);
        }
        
    }
    else
    {
        if(cur->scope.count(id))
        {
            return cur->scope[id];
        }
        cur=cur->parentptr;
        return search(cur,id);
    }
    cout<<cur->flag<<" "<< cur->scope.count(id)<<endl;
    exit(0);
    return "";
}