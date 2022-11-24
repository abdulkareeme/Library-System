#include <iostream>
#include "include/LibrarySystem.h"
#include "include/Admin.h"
#include <regex>
using namespace std;

int main()
{
    LibrarySystem Library;
    Library.FillAllCourses();
    while(true)
    {
        cout<<"\n1 - Login\n2 - Signup\n3 - Exit\nInput Number : ";
        int Chois;
        cin>>Chois;
        if(!cin.fail() && Chois==1000000007)
        {
            Admin admin;
            admin.RunAdimnOption();
            continue;
        }
        if(cin.fail() || Chois<1 || Chois>2)
        {
            if(!cin.fail() && Chois==3)break;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            Library.Login();
        }
        else
        {
            Library.Signup();
        }
    }
}
//////ALL TABLES//////

//MakeDMLQuery("CREATE TABLE IF NOT EXISTS user(\"user id\" INTEGER PRIMARY KEY AUTOINCREMENT , email VARCHAR(100), username VARCHAR(30),\"full name\" VARCHAR(50),password VARCHAR(100),\"account type\" INTEGER DEFAULT 1 ) ;");

//MakeDMLQuery("CREATE TABLE IF NOT EXISTS course(\"course id\" INTEGER PRIMARY KEY AUTOINCREMENT,\"course name\" VARCHAR(30), \"owner id\"INTEGER NOT NULL ,password VARCHAR(100) ,  \"dead line\" DATE , \
FOREIGN KEY(\"owner id\") REFERENCES user(\"user id\") ON DELETE CASCADE ON UPDATE CASCADE); ");

//MakeDMLQuery("CREATE TABLE IF NOT EXISTS \"requirment courses\" (\"course id\" INTGER , \"required id\" INTEGER,FOREIGN KEY (\"course id\") REFERENCES course(\"course id\") ON DELETE CASCADE ON UPDATE CASCADE ) ; ");

//MakeDMLQuery("CREATE TABLE IF NOT EXISTS \"requests and enrolled\" (\"user id\" INTEGER , \"course id\" INTEGER , \"account type\" INTEGER , FOREIGN KEY (\"user id\") REFERENCES user(\"user id\") ON DELETE CASCADE ON UPDATE CASCADE ,\
FOREIGN KEY (\"course id\") REFERENCES course(\"course id\") ON DELETE CASCADE ON UPDATE CASCADE ) ;");

//MakeDMLQuery("CREATE TABLE IF NOT EXISTS assignment (\"assignment id\" INTEGER PRIMARY KEY AUTOINCREMENT, \"course id\" INTEGER ,\"assignment question\" VARCHAR(255) , \"full mark\" DECIMAL , deadline DATE,\
FOREIGN KEY (\"course id\") REFERENCES course(\"course id\") ON DELETE CASCADE ON UPDATE CASCADE );" );

//MakeDMLQuery("CREATE TABLE IF NOT EXISTS solution(\"assignment id\" INTEGET , \"user id\" INTEGER , \"full name\" VARCHAR(50), grade DECIMAL ,comment VARCHAR(255), \"text solution\" VARCHAR(1000) , \
FOREIGN KEY (\"assignment id\") REFERENCES assignment(\"assignment id\") ON DELETE CASCADE ON UPDATE CASCADE , FOREIGN KEY (\"user id\") REFERENCES user(\"user id\") ON DELETE CASCADE ON UPDATE CASCADE ,\
FOREIGN KEY (\"full name\") REFERENCES user(\"full name\") ON DELETE CASCADE ON UPDATE CASCADE); ");

//MakeDMLQuery("CREATE TABLE IF NOT EXISTS message(\"user id\" INTEGER , \"reciver id\" INTEGER ,\"reciver name\" VARCHAR(50), \"message id\" INTEGER PRIMARY KEY AUTOINCREMENT, \"message type\" INTEGER, \"text message\" VARCHAR(1000),\
FOREIGN KEY (\"user id\") REFERENCES user(\"user id\") ON DELETE CASCADE ON UPDATE CASCADE , \
FOREIGN KEY (\"reciver id\") REFERENCES user(\"user id\") ON DELETE CASCADE ON UPDATE CASCADE ) ; ");

//MakeDMLQuery("CREATE TABLE replay(\"replay id\" INTEGER PRIMARY KEY AUTOINCREMENT , \"message id\" INTEGER , \"replayer id\" INTEGER,\"replayer name\" VARCHAR(50) , \"replayed message\" VARCHAR(1000) ,\
FOREIGN KEY (\"message id\") REFERENCES message(\"message id\") ON DELETE CASCADE ON UPDATE CASCADE ,\
FOREIGN KEY (\"replayer id\") REFERENCES user (\"user id\") ON DELETE CASCADE ON UPDATE CASCADE ); " );
