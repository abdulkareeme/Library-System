#ifndef TEACHER_H
#define TEACHER_H
#include <vector>
#include <string>
#include "Course.h"
#include "Person.h"
using namespace std;

class Teacher : public Person
{
    protected:

        vector<Course> MyListCourses;

    private:

       bool Confirmed;

    public:

        Teacher(int ID , string FullName , string UserName, string Email , string Password, int AccountType
                , vector<Message>ListMyMessages , vector<Message>ListRecivedMessages
                , vector<Course>MyListCourses , bool Confirmed);

        bool IsConfirmed();

        void ViewSolutions(int CourseIndex ,int AssignmentIndex);

        void ShowAssignmentInformations(int CourseIndex, int AssignmentIndex);

        void ViewAssignment(int CourseIndex);

        void VeiwAssignmentsMenu();

        void CreatAssignment(int CourseIndex);

        void ViewCoursesMenu();

        void DisplayCourses();

        void ViewCourses();

        void AddToMyListCources(Course crs);

        void AddPost(int CourseIndex);

        void ViewPosts(int CourseIndex);

        void SetGrade(int CourseIndex , int AssignmentIndex);

        void ChangeCourseDeadLine(int CourseIndex);
};

#endif // TEACHER_H
