#include "Teacher.h"
#include <iostream>

Teacher::Teacher(int ID, string FullName, string UserName, string Email, string Password, int AccountType, vector<Message>ListMyMessages, vector<Message>ListRecivedMessages, vector<Course>MyListCourses, bool Confirmed)
    : Person( ID, FullName, UserName, Email, Password, AccountType, ListMyMessages,ListRecivedMessages)
{
    this->MyListCourses=MyListCourses;
    this->Confirmed=Confirmed;
}

bool Teacher::IsConfirmed()
{
    return Confirmed;
}

void Teacher::ViewSolutions(int CourseIndex,int AssignmentIndex)
{
    while(true)
    {
        Teacher::MyListCourses[CourseIndex-1].ViewAssignmetInfo(AssignmentIndex);
        cout<<"\n1 - Set Grade\n2 - Back\nInput Number : ";
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois>1)
        {
            if(!cin.fail() && Chois==2)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        int StudentID;
        float Grade;
        cout<<"Input Student ID : ";
        cin>>StudentID;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"Input Grade : ";
        cin>>Grade;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        bool HaveComment=0;
        string Comment="";
        cout<<"Do you want to write commnet ? [0/1] : ";
        cin>>HaveComment;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(HaveComment)
        {
            cout<<"Input Comment : ";
            cin.ignore();
            getline(cin,Comment,'\n');
            while(!ValidateText(Comment))
            {
                cout<<"Invalid Input\n";
                cout<<"Input Comment : ";
                getline(cin,Comment,'\n');
            }
        }
        Teacher::MyListCourses[CourseIndex-1].SetGrade(AssignmentIndex,StudentID, Grade, Comment);
    }
}

void Teacher::ShowAssignmentInformations(int CourseIndex, int AssignmentIndex)
{
    while(true)
    {
        cout<<"\n------------------------------------------------------------------------------\n";
        cout<<"Question : ";
        Teacher::MyListCourses[CourseIndex-1].GetAssignmentQuestion(AssignmentIndex);
        cout<<"\n";
        cout<<"Full mark : ";
        Teacher::MyListCourses[CourseIndex-1].GetAssignmentFullMark(AssignmentIndex);
        cout<<"\n";
        cout<<"DeadLine : ";
        Teacher::MyListCourses[CourseIndex-1].GetAssignmentDeadLine(AssignmentIndex);
        cout<<"\n------------------------------------------------------------------------------\n";
        cout<<"\n1 - Change DeadLine \n2 - Back\nInput Number : ";
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois>1)
        {
            if(!cin.fail() && Chois==2)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        int day,mounth,year;
        cout<<"DeadLine of this Assignment day/mounth/year\n";
        cout<<"Enter Day : ";
        cin>>day;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"Enter Mounth : ";
        cin>>mounth;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"Enter Year : ";
        cin>>year;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        Date AssignmentDeadLine=Date(day,mounth,year);
        Teacher::MyListCourses[CourseIndex-1].SetAssignmentDeadLine( AssignmentDeadLine,AssignmentIndex);
    }
}

void Teacher::VeiwAssignmentsMenu()
{
    cout<<"\nAssignment Menu :\n1 - Show information\n\
2 - Veiw solutions\n\
3 - Back\n\
Input Number : ";
}

void Teacher::ViewAssignment(int CourseIndex)
{
    if(CourseIndex<1 || CourseIndex > (int)Teacher::MyListCourses.size())
    {
        cout<<"Invalid input\n";
        return;
    }
    while(true)
    {
        Teacher::MyListCourses[CourseIndex-1].ViewAllAssignments();
        Teacher::VeiwAssignmentsMenu();
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
        cout<<"Input Assignment Number : ";
        int AssignmentIndex;
        cin>>AssignmentIndex;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            Teacher::ShowAssignmentInformations(CourseIndex, AssignmentIndex);
        }
        else if(Chois==2)
        {
            Teacher::ViewSolutions(CourseIndex,AssignmentIndex);
        }
    }
}

void Teacher::CreatAssignment(int CourseIndex)
{
    string AssignmentQuestion;
    float FullMark;
    int day,mounth,year;
    cin.ignore();
    cout<<"Enter Assignment Question : ";
    getline(cin,AssignmentQuestion,'\n');
    while(!ValidateText(AssignmentQuestion))
    {
        cout<<"Invalid Input\n";
        cout<<"Enter Assignment Question : ";
        getline(cin,AssignmentQuestion,'\n');
    }
    cout<<"Enter FullMark : ";
    cin>>FullMark;
    while(cin.fail())
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid Input\n";
        cin>>FullMark;
    }
    cout<<"DeadLine of this Assignment day/mounth/year\n";
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
    Date AssignmentDeadLine=Date(day,mounth,year);
    Teacher::MyListCourses[CourseIndex-1].CreateAssignment(AssignmentQuestion, FullMark, AssignmentDeadLine);
}

void Teacher::ViewCoursesMenu()
{
    cout<<"\nCourse Menu:\n1 - View assignments\n\
2 - Create assignment\n\
3 - Add post\n\
4 - View posts\n\
5 - Change DeadLine\n\
6 - Get statistics\n\
7 - Back\n\
Input Number : ";
}

void Teacher::DisplayCourses()
{
    int Index=1;
    cout<<"\n";
    for(auto course : Teacher::MyListCourses)
    {
        cout<<Index<<" - ";
        course.DisplayCourseInfo();
        cout<<"\n";
        Index++;
    }
}

void Teacher::ViewCourses()
{
    while(true)
    {
        Teacher::DisplayCourses();
        Teacher::ViewCoursesMenu();
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois<1 || Chois>6)
        {
            if(!cin.fail() && Chois==7)return;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"Input Course Number : ";
        int CourseIndex;
        cin>>CourseIndex;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid input\n";
            continue;
        }
        if(CourseIndex<1 || CourseIndex>(int)Teacher::MyListCourses.size())
        {
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            Teacher::ViewAssignment(CourseIndex);
        }
        else if(Chois==2)
        {
            Teacher::CreatAssignment(CourseIndex);
        }
        else if(Chois==3)
        {
            Teacher::AddPost(CourseIndex);
        }
        else if(Chois==4)
        {
            Teacher::ViewPosts(CourseIndex);
        }
        else if(Chois==5)
        {
            Teacher::ChangeCourseDeadLine(CourseIndex);
        }
        else if(Chois==6)
        {
            Teacher::MyListCourses[CourseIndex-1].ShowGradeReportForTeacher();
        }
    }
}

void Teacher::ChangeCourseDeadLine(int CourseIndex)
{
    int day,mounth,year;
    cout<<"DeadLine of this course day/mounth/year\n";
    cout<<"Enter Day : ";
    cin>>day;
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid Input\n";
        return;
    }
    cout<<"Enter Mounth : ";
    cin>>mounth;
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid Input\n";
        return;
    }
    cout<<"Enter Year : ";
    cin>>year;
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid Input\n";
        return;
    }
    Date CourseDeadline=Date(day,mounth,year);
    Teacher::MyListCourses[CourseIndex-1].SetCourseDeadLine(CourseDeadline);
}

void Teacher::AddToMyListCources(Course crs)
{
    Teacher::MyListCourses.push_back(crs);
}

void Teacher::AddPost(int CourseIndex)
{
    cout<<"Input Text : ";
    cin.ignore();
    string TextMessage;
    getline(cin,TextMessage,'\n');
    while(!ValidateText(TextMessage))
    {
        cout<<"Invalid Input\n";
        cout<<"Input Text : ";
        getline(cin,TextMessage,'\n');
    }
    Teacher::MyListCourses[CourseIndex-1].AddPost( Teacher::GetUserID(), Teacher::GetFullName(), TextMessage);
}

void Teacher::ViewPosts(int CourseIndex)
{
    while(true)
    {
        Teacher::MyListCourses[CourseIndex-1].ViewPosts();
        cout<<"\nPost Menu:\n1 - Comment on post\n2 - Back\nInput Number : ";
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
        cout<<"Input post number : ";
        int PostIndex;
        cin>>PostIndex;
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        cout<<"Input Text : ";
        cin.ignore();
        string TextMessage;
        getline(cin,TextMessage,'\n');
        while(!ValidateText(TextMessage))
        {
            cout<<"Invslid Input\n";
            cout<<"Input Text : ";
            getline(cin,TextMessage,'\n');
        }
        Teacher::MyListCourses[CourseIndex-1].ReplayOnPost( PostIndex, Teacher::GetUserID(), Teacher::GetFullName(), TextMessage);
    }
}

