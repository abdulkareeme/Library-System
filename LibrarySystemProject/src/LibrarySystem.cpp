#include "LibrarySystem.h"
#include <iostream>
using namespace std;

void LibrarySystem::DisplayDoctorMenu()
{
    cout<<"\n1 - Create Course\n\
2 - Send invite to a teacher assistant\n\
3 - View courses\n\
4 - Delete Course\n\
5 - Logout\n\
Input Number : ";
}

void LibrarySystem::RunDoctorOptions(Doctor Dr)
{
    while(true)
    {
        LibrarySystem::DisplayDoctorMenu();
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois>4)
        {
            if(!cin.fail() && Chois==5)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            Dr.CreateCourse();
        }
        else if(Chois==2)
        {
            LibrarySystem::SendInviteForTA( Dr );
        }
        else if(Chois==3)
        {
            Dr.ViewCourses();
        }
        else if(Chois==4)
        {
            Dr.DeleteCourse();
        }
    }

}
void LibrarySystem::DisplayStudentMenu()
{
    cout<<"\n1 - Regiter to course\n\
2 - View courses\n\
3 - Logout\n\
Input Number : ";
}
void LibrarySystem::RunStudentOptions(Student Stu)
{
    while(true)
    {
        LibrarySystem::DisplayStudentMenu();
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois>2)
        {
            if(!cin.fail() && Chois==3)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            Stu.RegisterOnCourse();
        }
        else if(Chois==2)
        {
            Stu.ViewCourse();
        }
    }
}
void LibrarySystem::DisplayTeacherAssistantMenu()
{
    cout<<"\n1 - View courses\n\
2 - View Invitations\n\
3 - Logout\n\
Input Number : ";
}
void LibrarySystem::RunTeacherAssistantOptions(TeacherAssistant TA)
{
    while(true)
    {
        LibrarySystem::DisplayTeacherAssistantMenu();
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois>2)
        {
            if(!cin.fail() && Chois==3)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            TA.ViewCourses();
        }
        else if(Chois==2)
        {
            TA.AnswereInvitation();
        }
    }
}
void LibrarySystem::Login()
{
    //Check Email and password
    vector<string>Emails,Passwords, UserNames,FullNames ;
    vector<int>Confirmeds,AccountTypes,UserIDs;
    sqlite3* DB;
    sqlite3_stmt* statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    int result=MakeDQLQuery(choose(DB,&statment,"SELECT email , password , confirmed , \"account type\", username , \"full name\" , \"user id\" FROM user ; "));
    if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        Emails.push_back((char*)sqlite3_column_text(statment,0));
        Passwords.push_back((char*)sqlite3_column_text(statment,1));
        Confirmeds.push_back((int)sqlite3_column_int(statment,2));
        AccountTypes.push_back((int)sqlite3_column_int(statment,3));
        UserNames.push_back((char*)sqlite3_column_text(statment,4));
        FullNames.push_back((char*)sqlite3_column_text(statment,5));
        UserIDs.push_back((int)sqlite3_column_int(statment,6));
    }
    sqlite3_close(DB);
    //End

    string Email="",Password="";
    int EmailIndex;
    cin.ignore();
    while(true)
    {
        cout<<"Input Your Email : ";
        getline(cin,Email,'\n');//validate email

        bool exist=0;
        int Index=0;
        for(auto email : Emails)
        {
            if(email==Email)
            {
                exist=1;
                EmailIndex=Index;
                break;
            }
            Index++;
        }
        if(!exist)
        {
            cout<<"Email Not Exists\n";
            continue;
        }
        break;
    }

    while(true)
    {
        cout<<"Input Your Password , or press -1 to cansel : ";
        getline(cin,Password,'\n');

        if(Password=="-1") return;

        if(Password==Passwords[EmailIndex])
        {
            break;
        }
        cout<<"Wrong Password\n";
    }

    if(!Confirmeds[EmailIndex])
    {
        cout<<"You have to wait until the admin confirm your account :( \n";
        return;
    }

    int UserID=UserIDs[EmailIndex], AccountType=AccountTypes[EmailIndex], Confirmed=Confirmeds[EmailIndex];
    string FullName=FullNames[EmailIndex], UserName=UserNames[EmailIndex] ;

    //Clear RAM
    UserIDs.clear();
    AccountTypes.clear();
    Confirmeds.clear();
    FullNames.clear();
    UserNames.clear();
    //END Clear

    vector<Message>ListMyMessage,ListReciveMessage;
    vector<Course>MyListCourses;

    LibrarySystem::QueryGetMessagesData_DB(ListMyMessage,ListReciveMessage,UserID,FullName);
    LibrarySystem::QueryGetCourseData_DB(MyListCourses,UserID, AccountType);

    if(AccountTypes[EmailIndex]==1)
    {
        Student Stu = Student(UserID,FullName,UserName,Email,Password,AccountType,ListMyMessage,ListReciveMessage,MyListCourses);
        LibrarySystem::RunStudentOptions(Stu);
    }
    else if(AccountTypes[EmailIndex]==2)
    {
        Doctor Dr= Doctor(UserID,FullName,UserName,Email,Password,AccountType,ListMyMessage,ListReciveMessage,MyListCourses,Confirmed);
        LibrarySystem::RunDoctorOptions(Dr);
    }
    else
    {
        vector<Course>Invitations;
        LibrarySystem::QueryGetCourseData_DB(Invitations,UserID,5);
        TeacherAssistant TA = TeacherAssistant(UserID,FullName,UserName,Email,Password,AccountType,ListMyMessage,ListReciveMessage,MyListCourses,Confirmed,Invitations);
        LibrarySystem::RunTeacherAssistantOptions(TA);
    }
}
void LibrarySystem::Signup()
{
    vector<string>UserNames,Emails;

    sqlite3* DB;
    sqlite3_stmt* statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    int result=MakeDQLQuery(choose(DB,&statment,"SELECT username ,email FROM user ; "));
    if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        UserNames.push_back((char*)sqlite3_column_text(statment,0));
        Emails.push_back((char*)sqlite3_column_text(statment,1));
    }
    sqlite3_close(DB);

    string FullName="",UserName="",Email="",Password="";
    int AccountTpye;
    cin.ignore();
    cout<<"Input your full name : ";
    getline(cin,FullName,'\n');
    while(!ValidateText(FullName))
    {
        cout<<"Invalid Input\n";
        cout<<"Input your full name : ";
        getline(cin,FullName,'\n');
    }
    cout<<"Input Username : ";
    getline(cin,UserName,'\n');
    while(!ValidateText(UserName))
    {
        cout<<"Invalid Input\n";
        cout<<"Input your Username : ";
        getline(cin,UserName,'\n');
    }
    while(true)
    {
        bool taken=0;
        for(auto userN : UserNames)
        {
            if(userN==UserName)
            {
                taken=1;
                break;
            }
        }
        if(taken)
        {
            cout<<"Username already exist\nInput Username : ";
            getline(cin,UserName,'\n');
            while(!ValidateText(UserName))
            {
                cout<<"Invalid Input\n";
                cout<<"Input your Username : ";
                getline(cin,UserName,'\n');
            }
        }
        else break;
    }

    cout<<"Input Your Email : ";
    getline(cin,Email,'\n');
    while(!ValidateEmail(Email))
    {
        cout<<"Invalid Email\n";
        cout<<"Input Your Email : ";
        getline(cin,Email,'\n');
    }
    while(true)
    {
        bool taken=0;
        for(auto email : Emails)
        {
            if(email==Email)
            {
                taken=1;
                break;
            }
        }
        if(taken)
        {
            cout<<"Email already exist\nInput Email: ";
            getline(cin,Email,'\n');
            while(!ValidateEmail(Email))
            {
                cout<<"Invalid Email\n";
                cout<<"Input Your Email : ";
                getline(cin,Email,'\n');
            }
        }
        else break;
    }

    cout<<"Input Your Password : ";
    getline(cin,Password,'\n');
    while(!ValidateText(Password))
    {
        cout<<"Invalid Input\n";
        cout<<"Input Your Password : ";
        getline(cin,Password,'\n');
    }
    cout<<"Are you :\n1 - Student\n2 - Doctor\n3 - Teacher Assistant\nInput Number : ";
    cin>>AccountTpye;
    if(cin.fail() || AccountTpye<1 || AccountTpye>3)
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid Input\n";
        return;
    }
    bool Confirmed=0;
    if(AccountTpye==1)Confirmed=1;
    string Quary = "INSERT INTO user (\"username\" , \"full name\" , \"password\" , \"email\" , \"account type\" , \"confirmed\" )\
    VALUES ( '"+UserName+"' , '"+FullName+"' , '"+Password+"' , '"+Email+"' , "+to_string(AccountTpye)+" , "+to_string(Confirmed)+" ) ;";
    MakeDMLQuery(Quary.c_str());
}

void LibrarySystem::SendInviteForTA(Doctor Dr)
{
    while(true)
    {
        Dr.DisplayCourses();
        cout<<"\n";
        cout<<"Input Course Number  , or -1 to cancel : ";
        int CourseIndex;
        cin>>CourseIndex;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(CourseIndex==-1)return;
        Dr.QuarySendInviteForTA_DB(CourseIndex);
    }
}

void LibrarySystem::QueryGetMessagesData_DB(vector<Message> &ListMyMessage,vector<Message> &ListReciveMessage, int UserID,string FullName)
{
    {

        sqlite3* DB;
        sqlite3_stmt* statment;
        sqlite3_open("Database/librarySystem.db",&DB);

        string Query = "SELECT \"message id\" , \"message type\" , \"reciver id\",\"reciver name\" , \"text message\" FROM message WHERE \"user id\" = "+to_string(UserID)+" ;";
        int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
        if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
        while((result=sqlite3_step(statment))==SQLITE_ROW)
        {
            sqlite3_stmt * statment2;
            string Query2="SELECT replay.\"replay id\" , replay.\"replayer id\" , replay.\"replayer name\" , replay.\"replayed message\" FROM replay , message WHERE replay.\"message id\" = "
                          +to_string((int)sqlite3_column_int(statment,0))+" AND message.\"message type\" = 0 ;";
            int result2=MakeDQLQuery(choose(DB,&statment2, Query2.c_str()));
            if(result2 != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
            vector<Replay>ListReplays;
            while((result2=sqlite3_step(statment2))==SQLITE_ROW)
            {
                Replay rp=Replay((int)sqlite3_column_int(statment2,0),(int)sqlite3_column_int(statment,0),(int)sqlite3_column_int(statment2,1),(char*)sqlite3_column_text(statment2,2),(char*)sqlite3_column_text(statment2,3));
                ListReplays.push_back(rp);
            }
            Message ms=Message((int)sqlite3_column_int(statment,0),UserID,(int)sqlite3_column_int(statment,2),FullName,
                               (char*)sqlite3_column_text(statment,3),(char*)sqlite3_column_text(statment,4),(int)sqlite3_column_int(statment,1),ListReplays);
            ListMyMessage.push_back(ms);
        }
        sqlite3_close(DB);
    }
    {
        sqlite3* DB;
        sqlite3_stmt* statment;
        sqlite3_open("Database/librarySystem.db",&DB);
        string Query = "SELECT \"message id\" , \"message type\" , \"user id\",\"reciver name\" , \"text message\" FROM message WHERE \"reciver id\" = "+to_string(UserID)+" AND \"message type\" = 0 ;";
        int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
        if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
        while((result=sqlite3_step(statment))==SQLITE_ROW)
        {
            sqlite3_stmt * statment2;
            string Query2="SELECT replay.\"replay id\" , replay.\"replayer id\" , replay.\"replayer name\" , replay.\"replayed message\" FROM replay , message WHERE replay.\"message id\" = "
                          +to_string((int)sqlite3_column_int(statment,0))+" AND message.\"message type\" = 0 ;";
            int result2=MakeDQLQuery(choose(DB,&statment2, Query2.c_str()));
            if(result2 != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
            vector<Replay>ListReplays;
            while((result2=sqlite3_step(statment2))==SQLITE_ROW)
            {
                Replay rp=Replay((int)sqlite3_column_int(statment2,0),(int)sqlite3_column_int(statment,0),(int)sqlite3_column_int(statment2,1),(char*)sqlite3_column_text(statment2,2),(char*)sqlite3_column_text(statment2,3));
                ListReplays.push_back(rp);
            }
            Message ms=Message((int)sqlite3_column_int(statment,0),(int)sqlite3_column_int(statment,2),UserID,Person::QueryGetName_DB((int)sqlite3_column_int(statment,2))
                               ,FullName,(char*)sqlite3_column_text(statment,4),(int)sqlite3_column_int(statment,1),ListReplays);
            ListReciveMessage.push_back(ms);
        }
        sqlite3_close(DB);
    }

}

void LibrarySystem::QueryGetCourseData_DB(vector<Course> &MyListCourses, int UserID, int AccountType)
{
    string QueryCourse="";
    if(AccountType==2)
        QueryCourse="SELECT T1.\"course id\" , T1.\"course name\" , T1.\"owner id\" , T1.\"password\" , T1.\"dead line\" \
        FROM course T1  WHERE (T1.\"owner id\" = "+to_string(UserID)+"); ";
    else
        QueryCourse="SELECT T1.\"course id\" , T1.\"course name\" , T1.\"owner id\" , T1.\"password\" , T1.\"dead line\" \
        FROM course T1 , \"requests and enrolled\" T2 \
        WHERE T2.\"user id\" = "+to_string(UserID)+" AND T2.\"account type\" = "+to_string(AccountType)+" AND T2.\"course id\" = T1.\"course id\" ; ";

    sqlite3* DB;
    sqlite3_stmt* corse;
    sqlite3_open("Database/librarySystem.db",&DB);
    int ResultCourse=MakeDQLQuery(choose(DB,&corse,QueryCourse.c_str()));
    if(ResultCourse!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<11<<"\n";
    while((ResultCourse=sqlite3_step(corse))==SQLITE_ROW)
    {
        vector<int>ListRegisterdStudentID;
        sqlite3_stmt* register_student;
        string QueryRegister="SELECT \"user id\" FROM \"requests and enrolled\" \
         WHERE  \"account type\" = 1 AND \"course id\" = "+to_string(((int)sqlite3_column_int(corse,0)))+" ;";
        int ResultRegister=MakeDQLQuery(choose(DB,&register_student,QueryRegister.c_str()));
        if(ResultRegister!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<22<<"\n";
        while((ResultRegister=sqlite3_step(register_student))==SQLITE_ROW)
        {
            ListRegisterdStudentID.push_back((int)sqlite3_column_int(register_student,0));
        }

        vector<int> ListEnrolldTA_ID;
        sqlite3_stmt* enrolled_ta;
        string QueryEnrolled="SELECT \"user id\" FROM \"requests and enrolled\" \
         WHERE  \"account type\" = 3 AND \"course id\" = "+to_string(((int)sqlite3_column_int(corse,0)))+" ;";
        int EnrolledResult=MakeDQLQuery(choose(DB,&enrolled_ta,QueryEnrolled.c_str()));
        if(EnrolledResult!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<33<<"\n";
        while((EnrolledResult=sqlite3_step(enrolled_ta))==SQLITE_ROW)
        {
            ListEnrolldTA_ID.push_back((int)sqlite3_column_int(enrolled_ta,0));
        }

        vector<int>ListOfRequiredCoursesID;
        sqlite3_stmt* required_courses;
        string QueryRequiredCourses="SELECT \"required id\" FROM \"requirment courses\" \
         WHERE \"course id\" = "+to_string((int)sqlite3_column_int(corse,0))+" ;";
        int RequiredCoursesResult=MakeDQLQuery(choose(DB,&required_courses,QueryRequiredCourses.c_str()));
        if(RequiredCoursesResult!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<44<<"\n";
        while((RequiredCoursesResult=sqlite3_step(required_courses))==SQLITE_ROW)
        {
            ListOfRequiredCoursesID.push_back((int)sqlite3_column_int(required_courses,0));
        }


        vector<Assignment>ListOfAssignments;
        sqlite3_stmt* assignment;
        string QueryAssignment="SELECT \"assignment id\" , \"assignment question\" , \"full mark\" , \"deadline\" \
        FROM assignment WHERE \"course id\" = "+to_string(((int)sqlite3_column_int(corse,0)))+" ;";
        int AssignmentResult=MakeDQLQuery(choose(DB,&assignment,QueryAssignment.c_str()));
        if(AssignmentResult!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<55<<"\n";
        while((AssignmentResult=sqlite3_step(assignment))==SQLITE_ROW)
        {
            map<int,SolutionInfo> ListSolutions;
            string QuerySolution="SELECT \"user id\" , \"full name\" , \"grade\" , \"comment\" , \"text solution\" \
            FROM solution WHERE \"assignment id\" = "+to_string((int)sqlite3_column_int(assignment,0));
            sqlite3_stmt* solution;
            int SolutionResult=MakeDQLQuery(choose(DB,&solution,QuerySolution.c_str()));
            if(SolutionResult!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<66<<"\n";
            while((SolutionResult=sqlite3_step(solution))==SQLITE_ROW)
            {
                SolutionInfo info= {(char*)sqlite3_column_text(solution,1),(char*)sqlite3_column_text(solution,4),
                                    (float)sqlite3_column_double(solution,2),(char*)sqlite3_column_text(solution,3)
                                   };

                ListSolutions[(int)sqlite3_column_int(solution,0)]= info;
            }

            string date =(char*) sqlite3_column_text(assignment,3);

            int LastIndex=0;
            string dayy="";
            for(int i=0; i<date.size(); i++)
            {
                if(date[i]=='/')
                {
                    LastIndex=i+1;
                    break;
                }
                dayy+=date[i];
            }
            int day=stoi(dayy);

            string mounthh="";
            for(int i=LastIndex; i<date.size(); i++)
            {
                if(date[i]=='/')
                {
                    LastIndex=i+1;
                    break;
                }
                mounthh+=date[i];
            }
            int mounth=stoi(mounthh);

            string yearr="";
            for(int i=LastIndex; i<date.size(); i++)
            {
                yearr+=date[i];
            }
            int year=stoi(yearr);

            Date AssignmentDeadline = Date(day,mounth,year);

            Assignment assig=Assignment((int)sqlite3_column_int(assignment,0),(int)sqlite3_column_int(corse,0),
                                        (char*)sqlite3_column_text(assignment,1),(float)sqlite3_column_double(assignment,2),AssignmentDeadline,ListSolutions);

            ListOfAssignments.push_back(assig);
        }

        vector<Message>ListCoursePosts;
        sqlite3_stmt* statment;
        string Query = "SELECT \"message id\" , \"message type\" , \"user id\", \"reciver name\" , \"text message\" FROM message WHERE \"reciver id\" = "+to_string((int)sqlite3_column_int(corse,0))+" AND \"message type\" = 1 ;";
        int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
        if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<77<<"\n";
        while((result=sqlite3_step(statment))==SQLITE_ROW)
        {
            sqlite3_stmt * statment2;
            string Query2="SELECT replay.\"replay id\" , replay.\"replayer id\" , replay.\"replayer name\" , replay.\"replayed message\" FROM replay , message WHERE replay.\"message id\" = "
                          +to_string((int)sqlite3_column_int(statment,0))+" AND message.\"message type\" = 1 ;";
            int result2=MakeDQLQuery(choose(DB,&statment2, Query2.c_str()));
            if(result2 != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<88<<"\n";
            vector<Replay>ListReplays;
            while((result2=sqlite3_step(statment2))==SQLITE_ROW)
            {
                Replay rp=Replay((int)sqlite3_column_int(statment2,0),(int)sqlite3_column_int(statment,0),(int)sqlite3_column_int(statment2,1),(char*)sqlite3_column_text(statment2,2),(char*)sqlite3_column_text(statment2,3));
                ListReplays.push_back(rp);
            }
            Message ms=Message((int)sqlite3_column_int(statment,0),(int)sqlite3_column_int(statment,2),(int)sqlite3_column_int(corse,0),Person::QueryGetName_DB((int)sqlite3_column_int(statment,2))
                               ,(char*)sqlite3_column_text(statment,3),(char*)sqlite3_column_text(statment,4),(int)sqlite3_column_int(statment,1),ListReplays);
            ListCoursePosts.push_back(ms);
        }

        string date=(char*)sqlite3_column_text(corse,4);
        int LastIndex=0;
        string dayy="";
        for(int i=0; i<date.size(); i++)
        {
            if(date[i]=='/')
            {
                LastIndex=i+1;
                break;
            }
            dayy+=date[i];
        }
        int day=stoi(dayy);

        string mounthh="";
        for(int i=LastIndex; i<date.size(); i++)
        {
            if(date[i]=='/')
            {
                LastIndex=i+1;
                break;
            }
            mounthh+=date[i];
        }
        int mounth=stoi(mounthh);

        string yearr="";
        for(int i=LastIndex; i<date.size(); i++)
        {
            yearr+=date[i];
        }
        int year=stoi(yearr);
        Date CourseDeadline=Date(day,mounth,year) ;

        Course crs=Course((int)sqlite3_column_int(corse,0),(char*)sqlite3_column_text(corse,1),ListRegisterdStudentID,(int)sqlite3_column_int(corse,2),
                          (char*)sqlite3_column_text(corse,3),CourseDeadline,ListEnrolldTA_ID,ListOfRequiredCoursesID,ListOfAssignments,ListCoursePosts);

        MyListCourses.push_back(crs);
    }
    sqlite3_close(DB);
}

void LibrarySystem::FillAllCourses()
{
    string QueryCourse="SELECT \"course id\" , \"course name\" , \"owner id\" , \"password\" , \"dead line\" FROM course ;";

    sqlite3* DB;
    sqlite3_stmt* corse;
    sqlite3_open("Database/librarySystem.db",&DB);
    int ResultCourse=MakeDQLQuery(choose(DB,&corse,QueryCourse.c_str()));
    if(ResultCourse!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<11<<"\n";
    while((ResultCourse=sqlite3_step(corse))==SQLITE_ROW)
    {
        vector<int>ListRegisterdStudentID;
        sqlite3_stmt* register_student;
        string QueryRegister="SELECT \"user id\" FROM \"requests and enrolled\" \
         WHERE  \"account type\" = 1 AND \"course id\" = "+to_string(((int)sqlite3_column_int(corse,0)))+" ;";
        int ResultRegister=MakeDQLQuery(choose(DB,&register_student,QueryRegister.c_str()));
        if(ResultRegister!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<22<<"\n";
        while((ResultRegister=sqlite3_step(register_student))==SQLITE_ROW)
        {
            ListRegisterdStudentID.push_back((int)sqlite3_column_int(register_student,0));
        }

        vector<int> ListEnrolldTA_ID;
        sqlite3_stmt* enrolled_ta;
        string QueryEnrolled="SELECT \"user id\" FROM \"requests and enrolled\" \
         WHERE  \"account type\" = 3 AND \"course id\" = "+to_string(((int)sqlite3_column_int(corse,0)))+" ;";
        int EnrolledResult=MakeDQLQuery(choose(DB,&enrolled_ta,QueryEnrolled.c_str()));
        if(EnrolledResult!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<33<<"\n";
        while((EnrolledResult=sqlite3_step(enrolled_ta))==SQLITE_ROW)
        {
            ListEnrolldTA_ID.push_back((int)sqlite3_column_int(enrolled_ta,0));
        }

        vector<int>ListOfRequiredCoursesID;
        sqlite3_stmt* required_courses;
        string QueryRequiredCourses="SELECT \"required id\" FROM \"requirment courses\" \
         WHERE \"course id\" = "+to_string((int)sqlite3_column_int(corse,0))+" ;";
        int RequiredCoursesResult=MakeDQLQuery(choose(DB,&required_courses,QueryRequiredCourses.c_str()));
        if(RequiredCoursesResult!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<44<<"\n";
        while((RequiredCoursesResult=sqlite3_step(required_courses))==SQLITE_ROW)
        {
            ListOfRequiredCoursesID.push_back((int)sqlite3_column_int(required_courses,0));
        }


        vector<Assignment>ListOfAssignments;
        sqlite3_stmt* assignment;
        string QueryAssignment="SELECT \"assignment id\" , \"assignment question\" , \"full mark\" , \"deadline\" \
        FROM assignment WHERE \"course id\" = "+to_string(((int)sqlite3_column_int(corse,0)))+" ;";
        int AssignmentResult=MakeDQLQuery(choose(DB,&assignment,QueryAssignment.c_str()));
        if(AssignmentResult!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<55<<"\n";
        while((AssignmentResult=sqlite3_step(assignment))==SQLITE_ROW)
        {
            map<int,SolutionInfo> ListSolutions;
            string QuerySolution="SELECT \"user id\" , \"full name\" , \"grade\" , \"comment\" , \"text solution\" \
            FROM solution WHERE \"assignment id\" = "+to_string((int)sqlite3_column_int(assignment,0));
            sqlite3_stmt* solution;
            int SolutionResult=MakeDQLQuery(choose(DB,&solution,QuerySolution.c_str()));
            if(SolutionResult!= SQLITE_OK)cout<<sqlite3_errmsg(DB)<<66<<"\n";
            while((SolutionResult=sqlite3_step(solution))==SQLITE_ROW)
            {
                SolutionInfo info= {(char*)sqlite3_column_text(solution,1),(char*)sqlite3_column_text(solution,4),
                                    (float)sqlite3_column_double(solution,2),(char*)sqlite3_column_text(solution,3)
                                   };

                ListSolutions[(int)sqlite3_column_int(solution,0)]= info;
            }

            string date =(char*) sqlite3_column_text(assignment,3);

            int LastIndex=0;
            string dayy="";
            for(int i=0; i<date.size(); i++)
            {
                if(date[i]=='/')
                {
                    LastIndex=i+1;
                    break;
                }
                dayy+=date[i];
            }
            int day=stoi(dayy);

            string mounthh="";
            for(int i=LastIndex; i<date.size(); i++)
            {
                if(date[i]=='/')
                {
                    LastIndex=i+1;
                    break;
                }
                mounthh+=date[i];
            }
            int mounth=stoi(mounthh);

            string yearr="";
            for(int i=LastIndex; i<date.size(); i++)
            {
                yearr+=date[i];
            }
            int year=stoi(yearr);

            Date AssignmentDeadline = Date(day,mounth,year);

            Assignment assig=Assignment((int)sqlite3_column_int(assignment,0),(int)sqlite3_column_int(corse,0),
                                        (char*)sqlite3_column_text(assignment,1),(float)sqlite3_column_double(assignment,2),AssignmentDeadline,ListSolutions);

            ListOfAssignments.push_back(assig);
        }

        vector<Message>ListCoursePosts;
        sqlite3_stmt* statment;
        string Query = "SELECT \"message id\" , \"message type\" , \"user id\", \"reciver name\" , \"text message\" FROM message WHERE \"reciver id\" = "+to_string((int)sqlite3_column_int(corse,0))+" AND \"message type\" = 1 ;";
        int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
        if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<77<<"\n";
        while((result=sqlite3_step(statment))==SQLITE_ROW)
        {
            sqlite3_stmt * statment2;
            string Query2="SELECT replay.\"replay id\" , replay.\"replayer id\" , replay.\"replayer name\" , replay.\"replayed message\" FROM replay , message WHERE replay.\"message id\" = "
                          +to_string((int)sqlite3_column_int(statment,0))+" AND message.\"message type\" = 1 ;";
            int result2=MakeDQLQuery(choose(DB,&statment2, Query2.c_str()));
            if(result2 != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<88<<"\n";
            vector<Replay>ListReplays;
            while((result2=sqlite3_step(statment2))==SQLITE_ROW)
            {
                Replay rp=Replay((int)sqlite3_column_int(statment2,0),(int)sqlite3_column_int(statment,0),(int)sqlite3_column_int(statment2,1),(char*)sqlite3_column_text(statment2,2),(char*)sqlite3_column_text(statment2,3));
                ListReplays.push_back(rp);
            }
            Message ms=Message((int)sqlite3_column_int(statment,0),(int)sqlite3_column_int(statment,2),(int)sqlite3_column_int(corse,0),Person::QueryGetName_DB((int)sqlite3_column_int(statment,2))
                               ,(char*)sqlite3_column_text(statment,3),(char*)sqlite3_column_text(statment,4),(int)sqlite3_column_int(statment,1),ListReplays);
            ListCoursePosts.push_back(ms);
        }

        string date=(char*)sqlite3_column_text(corse,4);
        int LastIndex=0;
        string dayy="";
        for(int i=0; i<date.size(); i++)
        {
            if(date[i]=='/')
            {
                LastIndex=i+1;
                break;
            }
            dayy+=date[i];
        }
        int day=stoi(dayy);

        string mounthh="";
        for(int i=LastIndex; i<date.size(); i++)
        {
            if(date[i]=='/')
            {
                LastIndex=i+1;
                break;
            }
            mounthh+=date[i];
        }
        int mounth=stoi(mounthh);

        string yearr="";
        for(int i=LastIndex; i<date.size(); i++)
        {
            yearr+=date[i];
        }
        int year=stoi(yearr);
        Date CourseDeadline=Date(day,mounth,year) ;

        Course crs=Course((int)sqlite3_column_int(corse,0),(char*)sqlite3_column_text(corse,1),ListRegisterdStudentID,(int)sqlite3_column_int(corse,2),
                          (char*)sqlite3_column_text(corse,3),CourseDeadline,ListEnrolldTA_ID,ListOfRequiredCoursesID,ListOfAssignments,ListCoursePosts);

        Course::AllCourses.push_back(crs);
    }
    sqlite3_close(DB);
}
