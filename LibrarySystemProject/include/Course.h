#ifndef COURSE_H
#define COURSE_H
#include <vector>
#include <string>
#include "Date.h"
#include "Message.h"
#include "Assignment.h"
#include <iostream>
#include "SqlSystem.h"
using namespace std;

class Course
{
    private:

        int CourseID; //Remember to fill this atrbute from data base

        string CourseName;

        int CourseOwnerID;

        string CoursePassword;

        Date CourseDeadLine;

        vector<int> ListRegisterdStudentID;

        vector<int> ListEnrolldTA_ID;

        vector<int> ListOfRequiredCoursesID; // don't forget to check if student has all courses requirment before register

        vector<Assignment> ListOfAssignments;

        vector<Message> ListCoursePosts;

    public:

        vector<Message> GetListCoursePosts();

        vector<Assignment> GetListOfAssignments();

        static vector<Course> AllCourses; // Remember to build this element by database And push courses to it in constructer

        static int GetIndex(int CourseID);

        Course(int CourseID ,string CourseName, vector<int>ListRegisterdStudentID,int CourseOwnerID,
                string CoursePassword,Date CourseDeadLine,vector<int> ListEnrolldTA_ID,
                    vector<int>ListOfRequiredCoursesID,vector<Assignment>ListOfAssignments,vector<Message>ListCoursePosts );

        Course(string CourseName, vector<int> ListOfRequiredCoursesID,Date CourseDeadLine,int CourseOwnerID, string CoursePassword);

        string GetCoursePassword(int StudentID);

        void Pay(int StudentID);

        Date GetCourseDeadline();

        vector<int> GetListOfRequiredCourses();

        int GetCourseID();

        void ViewPosts();

        void AddPost(int SenderID, string SenderName, string TextMessage);

        void ReplayOnPost(int Index,int ReplayerID, string ReplayerName,string TextMessag);

        void CreateAssignment(string AssignmentQuestion, float FullMark, Date AssignmentDeadline);

        bool Register(int StudentID);

        void ViewAllAssignments();

        void ViewAssignmetInfo(int Index);

        bool SetSolution(int StudentID,string StudentName, string Solution,int Index);

        void SetGrade(int Index, int StudentID, float Grade, string Comment);

        void SetAssignmentDeadLine(Date DeadLine,int Index);

        void SetCourseDeadLine(Date DealLine);

        void ShowGradeReportForStudent(int StudentID);

        void ShowGradeReportForTeacher();

        void GetRate(int StudentID,int Index);

        void GetAssignmentDeadLine(int Index);

        void GetAssignmentFullMark(int Index);

        void GetAssignmentQuestion(int Index);

        void DisplayCourseInfo();

        string GetCourseOwner();

        void AddTA(int TA_ID);

        bool operator==(Course other);

        void UnregisterStudentFromCourse(int StudentID);

        void ShowAssignmentStatusForStudent(int StudentID);

        void DeleteCourse(int CourseID);
        //Remember to delete course and it's assigments in datebase
};

#endif // COURSE_H
