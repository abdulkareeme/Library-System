#include "Course.h"

vector<Message> Course::GetListCoursePosts()
{
    return ListCoursePosts;
}

vector<Assignment> Course::GetListOfAssignments()
{
    return ListOfAssignments;
}

vector<Course> Course::AllCourses= {};

int Course::GetIndex(int CourseID)
{
    for(int Index=0; Index< Course::AllCourses.size() ; Index++)
    {
        if(Course::AllCourses[Index].GetCourseID() == CourseID)return Index;
    }
}

Course::Course(int CourseID,string CourseName, vector<int>ListRegisterdStudentID,int CourseOwnerID,
               string CoursePassword,Date CourseDeadLine,vector<int> ListEnrolldTA_ID,
               vector<int>ListOfRequiredCoursesID,vector<Assignment>ListOfAssignments,vector<Message>ListCoursePosts )
{
    this->CourseID=CourseID;
    this->CourseName=CourseName;
    this->ListRegisterdStudentID=ListRegisterdStudentID;
    this->CourseOwnerID=CourseOwnerID;
    this->CoursePassword=CoursePassword;
    this->CourseDeadLine=CourseDeadLine;
    this->ListEnrolldTA_ID=ListEnrolldTA_ID;
    this->ListOfRequiredCoursesID=ListOfRequiredCoursesID;
    this->ListOfAssignments=ListOfAssignments;
    this->ListCoursePosts=ListCoursePosts;
}

Course::Course(string CourseName, vector<int> ListOfRequiredCoursesID,Date CourseDeadLine,int CourseOwnerID, string CoursePassword)
{
    this->CourseName=CourseName;
    this->ListOfRequiredCoursesID=ListOfRequiredCoursesID;
    this->CourseDeadLine=CourseDeadLine;
    this->CourseOwnerID=CourseOwnerID;
    this->CoursePassword=CoursePassword;

    string DeadLine=to_string(CourseDeadLine.Day)+'/'+to_string(CourseDeadLine.Mounth)+'/'+to_string(CourseDeadLine.Year);
    string Query= "INSERT INTO course (\"course name\" , \"owner id\" , password , \"dead line\" ) \
    VALUES ( '"+CourseName+"' , "+to_string(CourseOwnerID)+" , '"+ CoursePassword+"' , '"+DeadLine+"' ) ; ";
    MakeDMLQuery(Query.c_str());

    sqlite3* DB;
    sqlite3_stmt * statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    Query = "SELECT max(\"course id\") FROM course ; ";
    int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
    if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        this->CourseID=(int)sqlite3_column_int(statment,0);
    }
    sqlite3_close(DB);

    for(auto RequiredCourse : ListOfRequiredCoursesID)
    {
        Query = "INSERT INTO \"requirment courses\" (\"course id\" , \"required id\" ) \
        VALUES ( "+to_string(CourseID)+" , "+to_string(RequiredCourse)+" ) ;";
        MakeDMLQuery(Query.c_str());
    }
    Course::AllCourses.push_back(*this);
}

string Course::GetCoursePassword(int StudentID)
{
    bool Registerd=0;
    for(auto ID:ListRegisterdStudentID)
    {
        if(StudentID==ID)
        {
            Registerd=1;
            break;
        }
    }
    if(Registerd)return CoursePassword;
    cout<<"Request Denied :(\n";
    cout<<"You must be registerd in the course\n";
    cout<<"Register ? [Y/N] : ";
    char YES='N';
    cin>>YES;
    if(cin.fail())
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid input\n";
        YES='N';
    }
    else if(YES=='Y')Pay(StudentID);
    else if(YES=='N')return "";
}

void Course::Pay(int StudentID)
{
    cout<<"Type 'Pay' To confirm payment , otherwise type anything to cancel : ";
    string confirm;
    cin>>confirm;
    if(confirm=="Pay" || confirm=="'pay'" || confirm=="pay" || confirm=="'Pay'")
    {
        ListRegisterdStudentID.push_back(StudentID);
        int GlobalIndex = Course::GetIndex(GetCourseID());
        Course::AllCourses[GlobalIndex] = *this;
        cout<<"The password of this course : "<<GetCoursePassword(StudentID)<<"\n";
    }
}

Date Course::GetCourseDeadline()
{
    CourseDeadLine.ShowDate();
}

vector<int> Course::GetListOfRequiredCourses()
{
    return ListOfRequiredCoursesID;
}

int Course::GetCourseID()
{
    return CourseID;
}

void Course::ViewPosts()
{
    int Index=1;
    for(auto post:ListCoursePosts)
    {
        cout<<Index<<" - Post : \n";
        post.DisplayMessageFrom();
        Index++;
    }
}

void Course::AddPost(int SenderID, string SenderName, string TextMessage)
{
    Message mes = Message(SenderID,CourseID,SenderName,CourseName,TextMessage,1);
    ListCoursePosts.push_back(mes);
    int GlobalIndex = Course::GetIndex(GetCourseID());
    Course::AllCourses[GlobalIndex] = *this;
}

void Course::ReplayOnPost(int Index,int ReplayerID, string ReplayerName,string TextMessag)
{
    if(Index-1<ListCoursePosts.size() || Index-1>=0)
    {
        ListCoursePosts[Index-1].AddReplay(ReplayerID,ReplayerName,TextMessag);
        int GlobalIndex = Course::GetIndex(GetCourseID());
        Course::AllCourses[GlobalIndex] = *this;
    }
    else cout<<"Invalid input\n";
}

void Course::CreateAssignment(string AssignmentQuestion, float FullMark, Date AssignmentDeadline)
{
    Assignment assignment=Assignment(CourseID,AssignmentQuestion,FullMark,AssignmentDeadline);
    ListOfAssignments.push_back(assignment);
    int GlobalIndex = Course::GetIndex(GetCourseID());
    Course::AllCourses[GlobalIndex] = *this;
}

bool Course::Register(int StudentID)
{
    if(GetCurentDate()>CourseDeadLine)
    {
        cout<<"Sorry you can't register in this course any more , Deadline of this course : ";
        CourseDeadLine.ShowDate();
        cout<<"\n";
        return false;
    }
    Course::Pay(StudentID);
    return true;
}

void Course::ViewAllAssignments()
{
    for(int i=0; i<Course::ListOfAssignments.size(); i++)
    {
        cout<<i+1<<" - ";
        Course::ListOfAssignments[i].GetQuestion();
        cout<<"\n";
    }
}

void Course::ViewAssignmetInfo(int Index)
{
    if(Index-1 >=Course::ListOfAssignments.size() || Index-1 <0)
    {
        cout<<"Invalid input\n";
        return;
    }
    Course::ListOfAssignments[Index-1].DisplayAllInfo();
}

bool Course::SetSolution(int StudentID,string StudentName, string Solution,int Index)
{
    if(Index-1 >=Course::ListOfAssignments.size() || Index-1 <0)
    {
        cout<<"Invalid input\n";
        return 0;
    }
    if(CourseDeadLine<GetCurentDate())
    {
        cout<<"You can't submit solution in this course any more , Deadline of this course : ";
        CourseDeadLine.ShowDate();
        cout<<"\n";
        return 0;
    }
    bool Status= Course::ListOfAssignments[Index-1].SetSolution(StudentID,StudentName,Solution);
    int GlobalIndex = Course::GetIndex(GetCourseID());
    Course::AllCourses[GlobalIndex] = *this;
    return Status;
}

void Course::SetGrade(int Index,int StudentID, float Grade, string Comment="")
{
    if(Index-1 >=Course::ListOfAssignments.size() || Index-1 <0)
    {
        cout<<"Invalid input\n";
        return;
    }
    Course::ListOfAssignments[Index-1].SetGrade(StudentID,Grade,Comment);
    int GlobalIndex = Course::GetIndex(GetCourseID());
    Course::AllCourses[GlobalIndex] = *this;
}

void Course::SetAssignmentDeadLine(Date DeadLine,int Index)
{
    if(Index-1 >=Course::ListOfAssignments.size() || Index-1 <0)
    {
        cout<<"Invalid input\n";
        return;
    }
    Course::ListOfAssignments[Index-1].SetDeadLine(DeadLine);
    int GlobalIndex = Course::GetIndex(GetCourseID());
    Course::AllCourses[GlobalIndex] = *this;
}

void Course::SetCourseDeadLine(Date DeadLine)
{
    this->CourseDeadLine=DeadLine;
    int GlobalIndex = Course::GetIndex(GetCourseID());
    Course::AllCourses[GlobalIndex] = *this;

    string Value="'"+to_string(DeadLine.Day)+'/'+to_string(DeadLine.Mounth)+'/'+to_string(DeadLine.Year)+"'";
    string Condition="\"course id\" = "+to_string(CourseID);
    UpdateObjactOnDB("course","\"dead line\"",Value,Condition.c_str());
}

void Course::ShowGradeReportForStudent(int StudentID)
{
    int NumOfSubmitedAssignment=0;
    float SumOfGradeOfSubmitedAssignment=0.0;
    float SumOfFullMarkOfSubmitedAssignment=0.0;
    for(auto assignment : Course::ListOfAssignments)
    {
        float Result = assignment.ShowGradeReportForStudent(StudentID);
        if(Result>=0)
        {
            NumOfSubmitedAssignment++;
            SumOfFullMarkOfSubmitedAssignment+=assignment.GetFullMark();
            SumOfGradeOfSubmitedAssignment+=assignment.GetGrade(StudentID);
        }
        else if(Result==-0.5) NumOfSubmitedAssignment++;
    }
    cout<<"Total Assignment : "<<Course::ListOfAssignments.size()<<"\n";
    cout<<"Number of submited assignments : "<<NumOfSubmitedAssignment<<"\n";
    int Percentage= SumOfFullMarkOfSubmitedAssignment ==0 ? 0 : SumOfGradeOfSubmitedAssignment*100/SumOfFullMarkOfSubmitedAssignment ;
    cout<<"Percentage (only for corrected assignment) : "<<Percentage<<" %\n";
}

void Course::ShowGradeReportForTeacher()
{
    cout<<"Total Assignment : "<<Course::ListOfAssignments.size()<<"\n";
    for(auto assignment : Course::ListOfAssignments)
    {
        cout<<"------------------------------------------------------------------------------\n";
        assignment.ShowBasicInfo();
        cout<<"Total Student : "<<Course::ListRegisterdStudentID.size()<<"\n";
        assignment.ShowGradeReportForTeacher();
        cout<<"------------------------------------------------------------------------------\n";
    }
}

void Course::GetRate(int StudentID,int Index)
{
    if(Index-1 >=Course::ListOfAssignments.size() || Index-1 <0)
    {
        cout<<"Invalid input\n";
        return;
    }
    Course::ListOfAssignments[Index-1].GetRate(StudentID);
}

void Course::GetAssignmentDeadLine(int Index)
{
    if(Index-1 >=Course::ListOfAssignments.size() || Index-1 <0)
    {
        cout<<"Invalid input\n";
        return;
    }
    Course::ListOfAssignments[Index-1].GetDeadLine();
}

void Course::GetAssignmentFullMark(int Index)
{
    if(Index >Course::ListOfAssignments.size() || Index <1)
    {
        cout<<"Invalid input\n";
        return;
    }
    cout<<Course::ListOfAssignments[Index-1].GetFullMark();
}

void Course::GetAssignmentQuestion(int Index)
{
    if(Index-1 >=Course::ListOfAssignments.size() || Index-1 <0)
    {
        cout<<"Invalid input\n";
        return;
    }
    Course::ListOfAssignments[Index-1].GetQuestion();
}

void Course::DisplayCourseInfo()
{
    cout<<"Course code : SRC"<<Course::CourseID<<"   Course name : "<<Course::CourseName<<"   Course owner : "<<Course::GetCourseOwner()<<"   Number of registerd sutdent : "<<Course::ListRegisterdStudentID.size()<<
    "   Course Deadline : ";Course::CourseDeadLine.ShowDate();
}

string Course::GetCourseOwner()
{
    string CourseOwnerName;
    string Query="SELECT \"full name\" FROM user WHERE \"user id\" = " + to_string(Course::CourseOwnerID) + " ;";
    sqlite3* DB;
    sqlite3_stmt* statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
    if(result != SQLITE_OK)
    {
        cout<<"There is no user has id "<<Course::CourseOwnerID<<"\n";
        return "";
    }
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        CourseOwnerName=(char*)sqlite3_column_text(statment,0);
    }
    return CourseOwnerName;
}

void Course::AddTA(int TA_ID)
{
    Course::ListEnrolldTA_ID.push_back(TA_ID);
    int GlobalIndex = Course::GetIndex(GetCourseID());
    Course::AllCourses[GlobalIndex] = *this;
}

bool Course::operator==(Course other)
{
    return this->CourseID == other.CourseID;
}

void Course::UnregisterStudentFromCourse(int StudentID)
{
    int Index=0;
    for(auto ID : ListRegisterdStudentID)
    {
        if(ID==StudentID)break;
        Index++;
    }
    auto it=ListRegisterdStudentID.begin();
    ListRegisterdStudentID.erase(it+Index);
    int GlobalIndex = Course::GetIndex(GetCourseID());
    Course::AllCourses[GlobalIndex] = *this;

    string Query = "DELETE FROM \"requests and enrolled\" WHERE \"uesr id\" = "+to_string(StudentID)+" AND \"course id\" = "+to_string(CourseID)+" ; ";
    MakeDMLQuery(Query.c_str());
}

void Course::ShowAssignmentStatusForStudent(int StudentID)
{
    int Index=1;
    for(auto assignment : Course::ListOfAssignments)
    {
        cout<<Index<<" - \n";
        assignment.ShowAssignmentStatusForStudent(StudentID);
    }
}
