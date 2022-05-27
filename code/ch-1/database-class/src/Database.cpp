#include <iostream>
#include <stdexcept>
#include "../inc/Database.hpp"

namespace Records {

    Employee& Database::addEmployee(const std::string& firstName,
                                    const std::string& lastName)
    {
        Employee theEmployee(firstName, lastName);
        theEmployee.setEmployeeNumber(mNextEmployeeNumber++);
        theEmployee.hire();
        mEmployees.push_back(theEmployee);
        return mEmployees[mEmployees.size()-1];
    }

    Employee& Database::getEmployee(int employeeNumber)
    {
        for (auto& employee : mEmployees) {
            if (employee.getEmployeeNumber() == employeeNumber) {
                return employee;
            }
        }
        throw std::logic_error("No employee found.");
    }

    Employee& Database::getEmployee(const std::string& firstName,
                                    const std::string& lastName)
    {
        for (auto& employee : mEmployees) {
            if (employee.getFirstName() == firstName) {
                if (employee.getLastName() == lastName) {
                    return employee;
                }
            }
        }
        throw std::logic_error("No employee found.");
    }

    void Database::displayAll() const
    {
        for (const auto& employee : mEmployees) {
            employee.display();
        }
    }

    void Database::displayCurrent() const
    {
        for (const auto& employee : mEmployees) {
            if (employee.isHired())
                employee.display();
        }
    }

    void Database::displayFormer() const
    {
        for (const auto& employee : mEmployees) {
            if (!employee.isHired())
                employee.display();
        }
    }
}