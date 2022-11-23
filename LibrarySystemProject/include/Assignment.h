#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H
#include "Date.h"
#include <iostream>
#include <map>
#include "SqlSystem.h"
using namespace std;

struct SolutionInfo
{
    string StudentName,Solution;
    float Grade=-1;
    string Comment="No commnent";
};

class Assignment
{
    private:

        int CourseID;

        int AssignmentID;

        string AssignmentQuestion;

        float FullMark;

        Date AssignmentDeadLine;

        map<int,SolutionInfo> ListSolutions;

    public:

        Assignment(int CourseID, string AssignmentQuestion , float FullMark, Date AssignmentDeadLine);

        Assignment(int AssignmentID, int CourseID, string AssignmentQuestion , float FullMark, Date AssignmentDeadLine, map<int,SolutionInfo> ListSolutions);

        bool SetSolution(int StudentID,string StudentName, string solution);

        void SetGrade(int StudentID, float Grade, string Comment);

        void SetDeadLine(Date DeadLine);

        float ShowGradeReportForStudent(int StudentID);

        void ShowGradeReportForTeacher();

        void GetRate(int StudentID);

        void DisplayAllInfo();

        void GetQuestion();

        void GetDeadLine();

        float GetFullMark();

        float GetGrade(int Student);

        void ShowBasicInfo();

        void ShowAssignmentStatusForStudent(int StudentID);

        int GetID();
};

#endif // ASSIGNMENT_H
