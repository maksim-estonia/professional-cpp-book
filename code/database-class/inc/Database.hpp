#pragma once
#include <iostream>
#include <vector>
#include "../inc/Employee.hpp"

namespace Records {
    const int kFirstEmployeeNumber = 1000;

    class Database
    {
    private:
        std::vector<Employee> mEmployees;
        int mNextEmployeeNumber = kFirstEmployeeNumber;
    public:
        Employee& addEmployee(const std::string& firstName,
                              const std::string& lastName);
        Employee& getEmployee(int employeeNumber);
        Employee& getEmployee(const std::string& firstName,
                              const std::string& lastName);
        
        void displayAll() const;
        void displayCurrent() const;
        void displayFormer() const;
    };
    
}