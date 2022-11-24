#include "Admin.h"

map<int,int> Admin::ViewSignupRequests()
{
    map<int,int> User;
    string Query="SELECT \"user id\" , username , email , \"full name\" , \"account type\" FROM user WHERE confirmed = 0 ; ";

    sqlite3* DB;
    sqlite3_stmt* statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
    if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
    cout<<"\n";
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        cout<<"User "<<(char*)sqlite3_column_text(statment,3)<<" want to signup as a ";
        if((int)sqlite3_column_int(statment,4)==2) cout<<"Doctor :\n";
        else cout<<"Teacher Assistant :\n";

        cout<<"User ID : "<<(int)sqlite3_column_int(statment,0)<<"   Username : "<<(char*)sqlite3_column_text(statment,1)
        <<"   Email : "<<(char*)sqlite3_column_text(statment,2)<<"   Full Name : "<<(char*)sqlite3_column_text(statment,3)<<"\n\n";

        User[(int)sqlite3_column_int(statment,0)]=0;
    }

    return User;
}

void Admin::AnswereOnRequests()
{
    while(true)
    {
        map<int,int>User= Admin::ViewSignupRequests();
        if(User.empty())
        {
            cout<<"\nThere Is No Reqests Until Now \n\n";
            break;
        }
        cout<<"\n1 - Answere\n2 - Logout\nInput Numper : ";
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois>1)
        {
            if(!cin.fail() && Chois==2)break;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"\nInput User ID You Want To Asswere : ";
        int UserID;
        cin>>UserID;
        if(cin.fail() || !User.count(UserID))
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"\n1 - Accept\n2 - Reject\n3 - Back\nInput Number : ";
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois>3)
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            UpdateObjactOnDB("user","confirmed","1","\"user id\" = "+to_string(UserID));
        }
        else
        {
            MakeDMLQuery("DELETE FROM user WHERE \"user id\" = "+to_string(UserID)+" ; ");
        }
    }
}

void Admin::ViewAllUsers()
{
    sqlite3* DB;
    sqlite3_stmt* statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    int result=MakeDQLQuery(choose(DB,&statment,"SELECT \"user id\" , email , username , password , \"full name\" , \"account type\" FROM user ; "));
    if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
    cout<<"\n";
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        if((int)sqlite3_column_int(statment,5)==1)
        {
            cout<<"Student : ";
        }
        else if((int)sqlite3_column_int(statment,5)==2)
        {
            cout<<"Doctor : ";
        }
        else
        {
            cout<<"Teacher Assistant : ";
        }
        cout<<(char*)sqlite3_column_text(statment,4)<<"\nUser ID : "<<(int)sqlite3_column_int(statment,0)<<
        "   Email : "<<(char*)sqlite3_column_text(statment,1)<<"   Username : "<<(char*)sqlite3_column_text(statment,2)<<
        "   Password : "<<(char*)sqlite3_column_text(statment,3)<<"\n\n";
    }
    sqlite3_close(DB);
}

void Admin::ViewAdminMenu()
{
    cout<<"\n1 - View all users\n2 - Answere on requests\n3 - Logout\nInput Number : ";
}

void Admin::RunAdimnOption()
{
    while(true)
    {
        Admin::ViewAdminMenu();
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois >2)
        {
            if(!cin.fail() && Chois==3)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            Admin::ViewAllUsers();
        }
        else
        {
            Admin::AnswereOnRequests();
        }
    }
}
