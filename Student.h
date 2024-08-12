#ifndef ASSIGN2_STUDENT_H
#define ASSIGN2_STUDENT_H

#include <string>

class Student {
public:
    // Constructor
    Student(const std::string& name, const std::string& studentId, const std::string& email);

    // Getters
    std::string getName() const;
    std::string getStudentId() const;
    std::string getEmail() const;

    // Display student information
    void display() const;

private:
    std::string name;
    std::string studentId;
    std::string email;
};

#endif // ASSIGN2_STUDENT_H
