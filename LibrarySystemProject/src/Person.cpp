#include "Person.h"
#include "Teacher.h"
#include "SqlSystem.h"

Person::Person(int ID, string FullName, string UserName, string Email, string Password, int AccountType, vector<Message>ListMyMessages, vector<Message>ListRecivedMessages)
{
    this->FullName=FullName;
    this->UserName=UserName;
    this->Email=Email;
    this->Password=Password;
    this->AccountType=AccountType;
    this->ID=ID;
    this->ListMyMessages=ListMyMessages;
    this->ListRecivedMessages=ListRecivedMessages;
}

string Person::GetFullName()
{
    return Person::FullName;
}

void Person::GetEmail()
{
    cout<<Person::Email;
}

int Person::GetAccountType()
{
    return Person::AccountType;
}

void Person::GetUserName()
{
    cout<<Person::UserName;
}

void Person::ViewMyMessages()
{
    for(auto message : Person::ListMyMessages)
    {
        message.DisplayMessageTo();
    }
}

void Person::ViewRecivedMessages()
{
    for(auto message : Person::ListMyMessages)
    {
        message.DisplayMessageFrom();
    }
}

void Person::SendMessage(int SenderID, int ReciverID,string SenderNmae, string TextMessage)
{
    string ReciverName=QueryGetName_DB(ReciverID);
    if(ReciverName=="")
    {
        cout<<"Invalid Input\n";
        return;
    }
    Message mes=Message(SenderID,ReciverID,SenderNmae,ReciverName,TextMessage,0);
    Person::ListMyMessages.push_back(mes);
}

int Person::GetUserID()
{
    return Person::ID;
}

string Person::QueryGetName_DB(int ID)
{
    string  Name="";
    string Query="SELECT \"full name\" FROM user WHERE \"user id\" = " + to_string(ID) + " ;";
    sqlite3* DB;
    sqlite3_stmt* statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
    if(result != SQLITE_OK)
    {
        cout<<"There is no user has id "<<ID<<"\n";
        return "";
    }
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        Name=(char*)sqlite3_column_text(statment,0);
    }
    sqlite3_close(DB);
    return Name;
}
