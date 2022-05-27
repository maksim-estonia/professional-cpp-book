#include <iostream>
#include "Employee.hpp"

namespace Records {
    Employee::Employee(const std::string& firstName,
                       const std::string& lastName):
        mFirstName(firstName), mLastName(lastName)
        {}

    void Employee::promote(int raiseAmmount)
    {
        setSalary(getSalary() + raiseAmmount);
    }

    void Employee::demote(int demeritAmount)
    {
        setSalary(getSalary() - demeritAmount);
    }

    void Employee::hire()
    {
        mHired = true;
    }

    void Employee::fire()
    {
        mHired = false;
    }

    void Employee::display() const
    {
        std::cout << "Employee: " << getLastName() << ", " << getFirstName() << std::endl;
        std::cout << "-------" << std::endl;
        std::cout << (isHired() ?  "Current Employee" : "Former Employee") << std::endl;
        std::cout << "Employee Number: " << getEmployeeNumber() << std::endl;
        std::cout << "Salary: $" << getSalary() << std::endl;
        std::cout << std::endl;
    }

    void Employee::setFirstName(const std::string& firstName)
    {
        mFirstName = firstName;
    }

    const std::string& Employee::getFirstName() const
    {
        return mFirstName;
    }

    void Employee::setLastName(const std::string& lastName)
    {
        mLastName = lastName;
    }

    const std::string& Employee::getLastName() const
    {
        return mLastName;
    }

    int Employee::getEmployeeNumber() const
    {
        return mEmployeeNumber;
    }

    void Employee::setEmployeeNumber(int employeeNumber)
    {
        mEmployeeNumber = employeeNumber;
    }

    void Employee::setSalary(int newSalary)
    {
        mSalary = newSalary;
    }

    int Employee::getSalary() const
    {
        return mSalary;
    }

    bool Employee::isHired() const
    {
        return mHired;
    }

    Employee::~Employee()
    {}

}