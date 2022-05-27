#include <iostream>
#include "Employee.hpp"

using namespace Records;

int main()
{
    std::cout << "Testing the Employee class." << std::endl;
    Employee emp;
    emp.setFirstName("John");
    emp.setLastName("Doe");
    emp.setEmployeeNumber(71);
    emp.setSalary(50000);
    emp.promote();
    emp.promote(50);
    emp.hire();
    emp.display();
    
    return EXIT_SUCCESS;
}