#include "Student.h"

Student::Student(int ID, string FullName, string UserName, string Email, string Password, int AccountType
                 , vector<Message>ListMyMessages, vector<Message>ListRecivedMessages, vector<Course>MyListCourses)
    :Person( ID, FullName, UserName, Email, Password, AccountType, ListMyMessages, ListRecivedMessages)
{
    this->MyListCourses=MyListCourses;
}

void Student::GetCoursePassword(int CourseIndex)
{
    Student::MyListCourses[CourseIndex-1].GetCoursePassword(GetUserID());
}

vector<Course> Student::GetUnregisterdCourses()
{
    vector<Course> UnregisterdCourses;
    for(auto course1 : Course::AllCourses)
    {
        bool Registered=0;
        for(auto course2 : Student::MyListCourses)
            if(course1==course2)Registered=1;

        if(!Registered)UnregisterdCourses.push_back(course1);
    }
    return UnregisterdCourses;
}

void Student::RegisterOnCourse()
{
    while(true)
    {
        vector<Course>UnregisterdCourses = GetUnregisterdCourses();
        cout<<"\n";
        int Index=1;
        for(auto course : UnregisterdCourses)
        {
            cout<<Index<<" - ";
            course.DisplayCourseInfo();
            cout<<"\n";
            Index++;
        }
        cout<<"\nRegister Menu :\n1 - Register\n2 - Back\nInput Number : ";
        int Chois;
        cin>>Chois;
        cout<<"\n";
        if(cin.fail() || Chois <1 || Chois>1)
        {
            if(!cin.fail() && Chois==2) return ;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        int CourseIndex;
        cout<<"Input Course Number : ";
        cin>>CourseIndex;
        if(cin.fail() || CourseIndex < 1 || CourseIndex > (int)UnregisterdCourses.size())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            // Check if student Compatible with required courses
            auto RequiredCourses=UnregisterdCourses[CourseIndex-1].GetListOfRequiredCourses();
            bool Compatible=1;
            vector<int> HaveToRegeister;
            for(int  i = 0 ; i <(int) RequiredCourses.size() ; i++)
            {
                bool ok=0;
                for(int j=0; j < (int)Student::MyListCourses.size() ; j++)
                {
                    if(RequiredCourses[i]==Student::MyListCourses[j].GetCourseID())
                    {
                        ok=1;
                        break;
                    }
                }
                if(!ok)
                {
                    Compatible=0;
                    HaveToRegeister.push_back(RequiredCourses[i]);
                }
            }

            if(Compatible && UnregisterdCourses[CourseIndex-1].Register(Student::GetUserID()))
            {
                Student::MyListCourses.push_back(UnregisterdCourses[CourseIndex-1]);

                string Query="INSERT INTO \"requests and enrolled\" (\"user id\" , \"course id\" , \"account type\" ) \
                VALUES ( "+to_string(GetUserID())+" , "+to_string(UnregisterdCourses[CourseIndex-1].GetCourseID())+" , 1 ) ; ";
                MakeDMLQuery(Query.c_str());

                cout<<"Registerd Successfully :) \n";
            }
            else
            {
                cout<<"Register Faild :(\n\n";
                if(HaveToRegeister.size()>0)
                {
                    cout<<"You have to be registered in these courses :\n";
                    for(auto CourseCode : HaveToRegeister)
                    {
                        cout<<"Course Code : "<<CourseCode<<"\n";
                    }
                    cout<<"\n";
                }
            }

        }
    }
}

void Student::AddAssignmentSolution(int CourseIndex)
{
    while(true)
    {

        Student::MyListCourses[CourseIndex-1].ViewAllAssignments();
        cout<<"\n\n";
        cout<<"1 - Answere\n2 - Back\nInput Number: ";
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois>1 || Chois <1)
        {
            if(!cin.fail() && Chois ==2 )return ;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
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
            string Solution;
            cin.ignore();
            cout<<"Input Your Solution : ";
            getline(cin,Solution,'\n');
            while(!ValidateText(Solution))
            {
                cout<<"Invalid Inpu\n";
                cout<<"Input Your Solution : ";
                getline(cin,Solution,'\n');
            }
            if(Student::MyListCourses[CourseIndex-1].SetSolution( Student::GetUserID(), Student::GetFullName(), Solution, AssignmentIndex))
            {
                cout<<"Answere Submited Successfuly :)\n";
            }
        }
    }
}

void Student::ShowGradeReport(int CourseIndex)
{
    Student::MyListCourses[CourseIndex-1].ShowGradeReportForStudent(Student::GetUserID());
}

void Student::UnregisterFromCourse(int CourseIndex)
{
    while(true)
    {
        //print Courses
        int Chois;
        cout<<"1 - Confirm Unregister\n2 - Back\nInput Number : ";
        cin>>Chois;
        if(cin.fail() || Chois>1 || Chois<1)
        {
            if(!cin.fail() && Chois==2)return ;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            Student::MyListCourses[CourseIndex-1].UnregisterStudentFromCourse(Student::GetUserID());
            auto it =Student::MyListCourses.begin();
            Student::QuesryUnregisterFromCourse_DB(CourseIndex);
            CourseIndex--;
            Student::MyListCourses.erase(it+CourseIndex);
            cout<<"Unregistered Successfully\n";
            break;
        }
    }
}

void Student::QuesryUnregisterFromCourse_DB(int CourseIndex)
{
    MakeDMLQuery("DELETE FROM \"requests and enrolled\" WHERE \"user id\" = "+to_string(GetUserID())+
                 " AND \"course id\" = "+to_string(MyListCourses[CourseIndex-1].GetCourseID())+" ; ");
}

void Student::ViewCourseMenu()
{
    cout<<"\nCourse Menu :\n1 - View assignment\n\
2 - Unregister from course\n\
3 - View posts\n\
4 - Add post\n\
5 - Show grade report\n\
6 - Back\nInput Number : ";
//register
}

void Student::ViewCourse()
{
    while(true)
    {
        int Index=1;
        cout<<"\n";
        for(auto course: Student::MyListCourses)
        {
            cout<<Index<<" - ";
            course.DisplayCourseInfo();
            cout<<"\n";
            Index++;
        }
        Student::ViewCourseMenu();
        int Chois;
        cin>>Chois;
        if(cin.fail() || Chois>5 || Chois<1)
        {
            if(!cin.fail() && Chois==6)return ;
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        int CourseIndex;
        cout<<"Input Course Number : ";
        cin>>CourseIndex;
        if(cin.fail() || CourseIndex<1 || CourseIndex >(int) Student::MyListCourses.size())
        {
            cin.clear();
            cin.ignore(1000,'\n');
            cout<<"Invalid Input\n";
            continue;
        }
        if(Chois==1)
        {
            Student::ViewAssignment(CourseIndex);
        }
        else if(Chois==2)
        {
            Student::UnregisterFromCourse(CourseIndex);
        }
        else if(Chois==3)
        {
            Student::ViewPosts(CourseIndex);
        }
        else if(Chois==4)
        {
            Student::AddPost(CourseIndex);
        }
        else if(Chois==5)
        {
            Student::ShowGradeReport(CourseIndex);
        }
    }
}

void Student::ViewAssignmentMenu()
{
    cout<<"\nAssignment Menu :\n\
1 - Add Solution\n\
2 - Back\n\
Input Number : ";
}

void Student::ViewAssignment(int CourseIndex)
{
    while(true)
    {
        Student::MyListCourses[CourseIndex-1].ShowAssignmentStatusForStudent(Student::GetUserID());
        Student::ViewAssignmentMenu();
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
        Student::AddAssignmentSolution(CourseIndex);
    }

}

void Student::ViewPosts(int CourseIndex)
{
    while(true)
    {
        Student::MyListCourses[CourseIndex-1].ViewPosts();
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
            cout<<"Invalid Input\n";
            cout<<"Input Text : ";
            getline(cin,TextMessage,'\n');
        }
        Student::MyListCourses[CourseIndex-1].ReplayOnPost( PostIndex, Student::GetUserID(), Student::GetFullName(), TextMessage);
    }
}

void Student::AddPost(int CourseIndex)
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
    Student::MyListCourses[CourseIndex-1].AddPost( Student::GetUserID(), Student::GetFullName(), TextMessage);
}

