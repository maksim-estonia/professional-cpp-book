#include <iostream>
#include "../inc/Database.hpp"

using namespace Records;

int main()
{
    Database myDB;
    Employee& emp1 = myDB.addEmployee("Greg", "Wallis");
    emp1.fire();

    Employee& emp2 = myDB.addEmployee("Marc", "White");
    emp2.setSalary(100000);

    Employee& emp3 = myDB.addEmployee("John", "Doe");
    emp3.setSalary(10000);
    emp3.promote();

    std::cout << "all employees: " << std::endl << std::endl;
    myDB.displayAll();

    std::cout << "current employees: " << std::endl << std::endl;
    myDB.displayCurrent();

    std::cout << "former employees: " << std::endl << std::endl;
    myDB.displayFormer();
}