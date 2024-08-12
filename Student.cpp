#include "Student.h"
#include <iostream>

// Constructor
Student::Student(const std::string& name, const std::string& studentId, const std::string& email)
    : name(name), studentId(studentId), email(email) {}

// Getters
std::string Student::getName() const {
    return name;
}

std::string Student::getStudentId() const {
    return studentId;
}

std::string Student::getEmail() const {
    return email;
}

// Display student information
void Student::display() const {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Student ID: " << studentId << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << std::endl;
}
