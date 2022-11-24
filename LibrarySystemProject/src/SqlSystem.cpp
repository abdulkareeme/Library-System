#include "../include/SqlSystem.h"
#include <iostream>

using namespace std;


void MakeDMLQuery(string Query)
{
    sqlite3* DB;
    sqlite3_stmt* statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    int result =sqlite3_prepare_v2(DB,Query.c_str(),-1,&statment,NULL);
    sqlite3_step(statment);
    sqlite3_finalize(statment);
    if(result != SQLITE_OK)
    {
        cout<<"Error : "<<sqlite3_errmsg(DB)<<"\n";
    }
    sqlite3_close(DB);
}

void UpdateObjactOnDB(string TableName , string ColumnName , string Value , string Condition)
{
    string Query="UPDATE "+TableName+" SET "+ColumnName+" = "+Value+" WHERE "+Condition+" ;";

    MakeDMLQuery(Query.c_str());
}

bool ValidateEmail(string Email)
{
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(Email,pattern);
}

bool ValidateText(string Text)
{
    if(Text.size()==0 || count(Text.begin(),Text.end() , ' ')==Text.size())return false;
    string valid="/.,!?%^&*()-_+=' 1234567890";
    for(int i=0;i<Text.size(); i++)
    {
        if((Text[i]>='a' && Text[i]<='z') || (Text[i]>='A' && Text[i]<='Z'))continue;
        if(valid.find(Text[i])==-1) return false;
    }
    return true;
}
