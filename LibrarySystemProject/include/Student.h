#ifndef STUDENT_H
#define STUDENT_H
#include "Person.h"
#include "Course.h"
using namespace std;

class Student : public Person
{
    private:

      vector<Course>MyListCourses;

    public:
        //think about not set AccountType here and with doctor and TA and set it by default

        Student(int ID , string FullName , string UserName, string Email , string Password , int AccountType
                  , vector<Message>ListMyMessages , vector<Message>ListRecivedMessages , vector<Course>MyListCourses);

        void GetCoursePassword(int CourseIndex);

        void RegisterOnCourse();

        void AddAssignmentSolution(int CourseIndex);

        void ShowGradeReport(int CourseIndex);

        void UnregisterFromCourse(int CourseIndex);

        void QuesryUnregisterFromCourse_DB(int CourseIndex);

        vector<Course> GetUnregisterdCourses();

        void ViewCourseMenu();

        void ViewCourse();

        void ViewAssignmentMenu();

        void ViewAssignment(int CourseIndex);

        void ViewPosts(int CourseIndex);

        void AddPost(int CourseIndex);
};

#endif // STUDENT_H
