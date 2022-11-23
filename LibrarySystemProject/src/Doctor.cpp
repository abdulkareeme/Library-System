#include "Doctor.h"

Doctor::Doctor(int ID, string FullName, string UserName, string Email, string Password, int AccountType
               , vector<Message>ListMyMessages, vector<Message>ListRecivedMessages
               , vector<Course>MyListCourses, bool Confirmed)
    : Teacher (ID, FullName, UserName, Email, Password, AccountType, ListMyMessages, ListRecivedMessages, MyListCourses, Confirmed) {}


void Doctor::QuarySendInviteForTA_DB(int CourseIndex)
{
    if(CourseIndex<1 || CourseIndex > (int) Doctor::MyListCourses.size())
    {
        cout<<"Invalid Input\n";
        return;
    }

    map<int,int>QueryResultTeacherID;

    //Display all Teacher Assistant who not enrolled in the course and not invited before
    string Query ="SELECT \"user id\" , \"full name\" FROM user WHERE\"account type\" = 3 AND \"user id\" NOT IN (SELECT \"user id\" FROM \"requests and enrolled\" WHERE \"course id\" = "
                  + to_string(Doctor::MyListCourses[CourseIndex-1].GetCourseID())+" )  ;";
    sqlite3* DB;
    sqlite3_stmt* statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
    if(result != SQLITE_OK)
    {
        cout<<sqlite3_errmsg(DB)<<"\n";
        return ;
    }
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        cout<<"Teacher Assistant ID : "<<sqlite3_column_int(statment,0)<<" ,  Name : "<<sqlite3_column_text(statment,1)<<"\n";
        QueryResultTeacherID[(int)sqlite3_column_int(statment,0)]=0;
    }
    sqlite3_close(DB);
//Add Teacher Query
    cout<<"\nInput Teacher ID : ";
    int TeacherID;
    cin>>TeacherID;
    if(cin.fail() || !QueryResultTeacherID.count(TeacherID))
    {
        cout<<"Invalid Input\n";
        return ;
    }
    Query="INSERT INTO \"requests and enrolled\" (\"user id\", \"course id\", \"account type\") VALUES ("
          +to_string(TeacherID)+" , "+to_string(Doctor::MyListCourses[CourseIndex-1].GetCourseID())+" , 5 ) ;";

    MakeDMLQuery(Query.c_str());
}


void Doctor::CreateCourse()
{
    string CourseName;
    vector<int> ListOfRequiredCoursesID;
    string CoursePassword;

    cout<<"Input Course Name : ";
    cin.ignore();
    getline(cin,CourseName,'\n'); //validate
    cout<<"\n";
    for(int Index =0 ; Index <(int) Course::AllCourses.size() ; Index++)
    {
        cout<<Index+1<<" - ";
        Course::AllCourses[Index].DisplayCourseInfo();
        cout<<"\n";
    }
    while(true)
    {
        cout<<"\n1 - Add Required Course \n2 - Next\nInput Number : ";
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
        cout<<"\nInput Course Number : ";
        int CourseIndex;
        cin>>CourseIndex;
        if(cin.fail() || CourseIndex<1 || CourseIndex > (int)Course::AllCourses.size())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        ListOfRequiredCoursesID.push_back( Course::AllCourses[CourseIndex-1].GetCourseID() );
    }
    int day,mounth,year;
    cout<<"DeadLine of this Course Day/Mounth/Year\n";
    cout<<"Enter Day : ";
    cin>>day;
    while(cin.fail())
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid Input\n";
        cout<<"Enter Day : ";
        cin>>day;
    }
    cout<<"Enter Mounth : ";
    cin>>mounth;
    while(cin.fail())
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid Input\n";
        cout<<"Enter Mounth : ";
        cin>>mounth;
    }
    cout<<"Enter Year : ";
    cin>>year;
    while(cin.fail())
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid Input\n";
        cout<<"Enter Year : ";
        cin>>year;
    }
    Date CourseDeadLine=Date(day,mounth,year);
    cout<<"Input Course Password : ";
    cin.ignore();
    getline(cin,CoursePassword,'\n');//validate
    Course crs = Course ( CourseName , ListOfRequiredCoursesID , CourseDeadLine , Doctor::GetUserID() , CoursePassword );
    Doctor::MyListCourses.push_back(crs);
    while(true)
    {
        cout<<"\n1 - Creat Assignment \n2 - Done\nInput Number : ";
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
        Doctor::CreatAssignment(Doctor::MyListCourses.size());//Add assignment for last course created
    }
}

void Doctor::DeleteCourse()
{
    while(true)
    {
        Doctor::DisplayCourses();
        cout<<"\n1 - Confirm Delete\n2 - Back\nInput Number : ";
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois!=1)
        {
            if(!cin.fail() && Chois==2)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"Input Course Number : ";
        int CourseIndex;
        cin>>CourseIndex;
        if(cin.fail() || CourseIndex<1 || CourseIndex > (int)Doctor::MyListCourses.size())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            Doctor::QueryDeleteCourse_DB(CourseIndex);
            int GlobalIndex=Course::GetIndex(Doctor::MyListCourses[CourseIndex-1].GetCourseID());
            auto it=Course::AllCourses.begin();
            Course::AllCourses.erase(it+GlobalIndex);
            it=Doctor::MyListCourses.begin();
            CourseIndex--;
            Doctor::MyListCourses.erase(it+CourseIndex);
        }
    }
}

void Doctor::QueryDeleteCourse_DB(int CourseIndex)
{
    for(auto assignment : MyListCourses[CourseIndex-1].GetListOfAssignments())
    {
        MakeDMLQuery("DELETE FROM solution WHERE \"assignment id\" = "+to_string(assignment.GetID())+";");
        MakeDMLQuery("DELETE FROM assignment WHERE \"assignment id\" = "+to_string(assignment.GetID())+";");
    }
    for(auto message : MyListCourses[CourseIndex-1].GetListCoursePosts())
    {
        MakeDMLQuery("DELETE FROM replay WHERE \"message id\" = "+to_string(message.GetID())+";");
        MakeDMLQuery("DELETE FROM message WHERE \"message id\" = "+to_string(message.GetID())+";");
    }
    MakeDMLQuery("DELETE FROM \"requirment courses\" WHERE \"course id\" = "+to_string(MyListCourses[CourseIndex-1].GetCourseID())+";");
    MakeDMLQuery("DELETE FROM \"requests and enrolled\" WHERE \"course id\" = "+to_string(MyListCourses[CourseIndex-1].GetCourseID())+";");
    MakeDMLQuery("DELETE FROM course WHERE \"course id\" = "+to_string(MyListCourses[CourseIndex-1].GetCourseID())+" ; ");
}
