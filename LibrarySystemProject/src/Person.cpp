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
    while(true)
    {
        int Index=1;
        for(auto message : Person::ListMyMessages)
        {
            cout<<Index<<" - Message :\n";
            message.DisplayMessageTo();
            Index++;
        }
        cout<<"\n1 - Replay on message\n2 - Back\nInput Number : ";
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois >1)
        {
            if(!cin.fail() && Chois==2)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        int MessageIndex;
        cout<<"Input Message Number : ";
        cin>>MessageIndex;
        if(cin.fail() || MessageIndex<1 || MessageIndex>ListMyMessages.size())
        {
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"Input Your Replay : ";
        string ReplayMessage;
        cin.ignore();
        getline(cin,ReplayMessage,'\n');
        ListMyMessages[MessageIndex-1].AddReplay(ID , FullName , ReplayMessage );
    }

}

void Person::ViewRecivedMessages()
{
    while(true)
    {
        int Index=1;
        for(auto message : Person::ListRecivedMessages)
        {
            cout<<Index<<" - Message :\n";
            message.DisplayMessageFrom();
            Index++;
        }
        cout<<"\n1 - Replay on message\n2 - Back\nInput Number : ";
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois >1)
        {
            if(!cin.fail() && Chois==2)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        int MessageIndex;
        cout<<"Input Message Number : ";
        cin>>MessageIndex;
        if(cin.fail() || MessageIndex<1 || MessageIndex>ListRecivedMessages.size())
        {
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"Input Your Replay : ";
        string ReplayMessage;
        cin.ignore();
        getline(cin,ReplayMessage,'\n');
        ListRecivedMessages[MessageIndex-1].AddReplay(ID , FullName , ReplayMessage );
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
