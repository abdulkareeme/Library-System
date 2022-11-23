#include "Assignment.h"

Assignment::Assignment(int CourseID, string AssignmentQuestion, float FullMark, Date AssignmentDeadLine)
{
    this->AssignmentDeadLine=AssignmentDeadLine;
    this->AssignmentQuestion=AssignmentQuestion;
    this->CourseID=CourseID;
    this->FullMark=FullMark;

    string Query="INSERT INTO assignment (\"course id\" , \"assignment question\" , \"full mark\" , \"deadline\") \
    VALUES ("+to_string(CourseID)+" , '"+AssignmentQuestion+"' , "+to_string(FullMark)+" , '"
                 +to_string(AssignmentDeadLine.Day)+'/'+to_string(AssignmentDeadLine.Mounth)+'/'+to_string(AssignmentDeadLine.Year)+"' ) ;";
    MakeDMLQuery(Query.c_str());

    sqlite3* DB;
    sqlite3_stmt * statment;
    sqlite3_open("Database/librarySystem.db",&DB);
    Query = "SELECT max(\"assignment id\") FROM assignment ; ";
    int result=MakeDQLQuery(choose(DB,&statment,Query.c_str()));
    if(result != SQLITE_OK)cout<<sqlite3_errmsg(DB)<<"\n";
    while((result=sqlite3_step(statment))==SQLITE_ROW)
    {
        this->AssignmentID=(int)sqlite3_column_int(statment,0);
    }
    sqlite3_close(DB);
}
Assignment::Assignment(int AssignmentID,int CourseID, string AssignmentQuestion, float FullMark, Date AssignmentDeadLine, map<int,SolutionInfo> ListSolutions)
{
    this->AssignmentDeadLine=AssignmentDeadLine;
    this->AssignmentQuestion=AssignmentQuestion;
    this->CourseID=CourseID;
    this->FullMark=FullMark;;
    this->ListSolutions=ListSolutions;
    this->AssignmentID=AssignmentID;
}
bool Assignment::SetSolution(int StudentID,string StudentName, string Solution)
{
    if(AssignmentDeadLine>GetCurentDate())
    {
        string Condition="\"user id\" = "+to_string(StudentID);

        if(ListSolutions.count(StudentID))
        {
            ListSolutions[StudentID].Grade=-1;
            ListSolutions[StudentID].Comment="No Comment";

            UpdateObjactOnDB("solution","grade","-1",Condition.c_str());
            UpdateObjactOnDB("solution","comment","'No Comment'",Condition.c_str());
            UpdateObjactOnDB("solution","\"text solution\"",Solution.c_str(),Condition.c_str());
        }
        else
        {
            ListSolutions[StudentID].StudentName=StudentName;
            string Query="INSERT INTO solution (\"assignemnt id\" , \"user id\" , \"full name\" , \"grade\" , comment , \"text solution\" )\
            VALUES ( "+to_string(AssignmentID)+" , "+to_string(StudentID)+" , '"+StudentName+"' , -1 , 'No Comment' , '"+Solution+"' ) ; ";
            MakeDMLQuery(Query.c_str());
        }
        ListSolutions[StudentID].Solution=Solution;
        return true;
    }
    else
    {
        cout<<"You can't submit solution in this assignment any more , Deadline of this assignment : ";
        AssignmentDeadLine.ShowDate();
        cout<<"\n";
        return false;
    }
}
void Assignment::SetGrade(int StudentID, float Grade, string Comment="No Comment")
{
    if(!ListSolutions.count(StudentID))
    {
        cout<<"Invalid Input\n";
        return;
    }
    if(Grade>FullMark ||Grade<0)
    {
        cout<<"Invalid Input : Grade must be in range [0.0,"<<FullMark<<"]\n";
        return ;
    }
    if(Comment=="")Comment="No Comment";
    ListSolutions[StudentID].Grade=Grade;
    ListSolutions[StudentID].Comment=Comment;

    string Condition="\"user id\" = "+to_string(StudentID);
    UpdateObjactOnDB("solution","grade",to_string(Grade),Condition.c_str());
    UpdateObjactOnDB("solution","comment","'No Comment'",Condition.c_str());
}
void Assignment::SetDeadLine(Date DeadLine)
{
    this->AssignmentDeadLine=DeadLine;

    string Value="'"+to_string(AssignmentDeadLine.Day)+'/'+to_string(AssignmentDeadLine.Mounth)+'/'+to_string(AssignmentDeadLine.Year)+"'";
    string Condition="\"assignment id\" = "+to_string(AssignmentID);
    UpdateObjactOnDB("assignment","deadline",Value,Condition.c_str());
}
void Assignment::ShowAssignmentStatusForStudent(int StudentID)
{
    cout<<"------------------------------------------------------------------------------\n";
    Assignment::ShowBasicInfo();
    cout<<"Status : ";
    if(!ListSolutions.count(StudentID))
    {
        cout<<"Not Submited Yet\n";
        cout<<"------------------------------------------------------------------------------\n";
        return ;
    }
    cout<<"Submited  ,  Solution : ";
    cout<<Assignment::ListSolutions[StudentID].Solution<<"\n";
    cout<<"Grade : ";
    if(Assignment::ListSolutions[StudentID].Grade==-1)
    {
        cout<<"Not corrected yet\n";
    }
    else
    {
        cout<<Assignment::ListSolutions[StudentID].Grade<<" / "<<FullMark<<"  ,  Rate : ";
        GetRate(StudentID);
        cout<<"\n";
        cout<<"Comment : "<<Assignment::ListSolutions[StudentID].Comment<<"\n";
    }
    cout<<"------------------------------------------------------------------------------\n";

}
void Assignment::GetRate(int StudentID)
{
    if(ListSolutions[StudentID].Grade==-1)
    {
        cout<<"Not corrected yet"<<endl;
        return;
    }
    float Percent=ListSolutions[StudentID].Grade*100/FullMark;
    if(Percent>=90)cout<<"A+";
    else if(Percent>=80)cout<<'A';
    else if(Percent>=75)cout<<"B+";
    else if(Percent>=70)cout<<'B';
    else if(Percent>=65)cout<<"C+";
    else if(Percent>=60)cout<<'C';
    else if(Percent>=50)cout<<'D';
    else cout<<'F';
}
void Assignment::DisplayAllInfo()
{
    cout<<"------------------------------------------------------------------------------\n";
    cout<<"Question : "<<AssignmentQuestion<<"\n";
    cout<<"Full mark : "<<FullMark<<"\n";
    cout<<"Dead line : ";
    AssignmentDeadLine.ShowDate();
    cout<<"\n";
    cout<<"Total solutions : "<<ListSolutions.size()<<"\n";
    for(auto [StudentID,Info] : ListSolutions)
    {
        cout<<"\t"<<"Student ID : "<<StudentID<<"  Student name : "<<Info.StudentName<<"   submit solution : "<<Info.Solution<<"\n";
        cout<<"\t\t";
        if(Info.Grade==-1)cout<<"Not corrected yet\n";
        else
        {
            cout<<"Grade : "<<Info.Grade<<" / "<<FullMark<<"  , Rate : ";
            GetRate(StudentID);
            cout<<" , Comment : "<<Info.Comment<<"\n";
        }
    }
    cout<<"------------------------------------------------------------------------------\n";
}
void Assignment::GetQuestion()
{
    cout<<Assignment::AssignmentQuestion;
}
void Assignment::GetDeadLine()
{
    Assignment::AssignmentDeadLine.ShowDate();
}
float Assignment::GetFullMark()
{
    return Assignment::FullMark;
}

void Assignment::ShowBasicInfo()
{
    cout<<"Assignment Question : "<<AssignmentQuestion<<"\n";
    cout<<"Full mark : "<<FullMark<<"  ,  ";
    cout<<"Dead line : ";
    AssignmentDeadLine.ShowDate();
    cout<<"\n";
}

float Assignment::ShowGradeReportForStudent(int StudentID)
{
    if(!ListSolutions.count(StudentID)) return -1 ; // Solution Not Submited Yet
    else
    {
        if(ListSolutions[StudentID].Grade>-1)return ListSolutions[StudentID].Grade; // Submited And Corrected
        return -0.5; // Submited And Not Corrected Yet
    }
}

float Assignment::GetGrade(int StudentID)
{
    return ListSolutions[StudentID].Grade;
}

void Assignment::ShowGradeReportForTeacher()
{
    int Success=0,Faild=0,NotCorrected=0;
    for(auto [StudentID, Info] : ListSolutions)
    {
        if(Info.Grade==-1)NotCorrected++;
        else if(Info.Grade<50)Faild++;
        else Success++;
    }
    cout<<"Total submitions : "<<ListSolutions.size()<<"\n";
    int PercentageSucces=ListSolutions.size() ==0 ? 0 : Success*100/ListSolutions.size() ;
    int PercentageFaild=ListSolutions.size() ==0 ? 0 : Faild*100/ListSolutions.size() ;
    cout<<"The number of successful student : "<<Success<<" , Percentage : "<<PercentageSucces<<" %\n";
    cout<<"The number of student who have faild : "<<Faild<<" , Percentage : "<<PercentageFaild<<" %\n";
    cout<<"Number of solutions not corrected : "<<NotCorrected<<"\n";
}

int Assignment::GetID()
{
    return AssignmentID;
}
