#include <string>

namespace Records {

    class Employee
    {
        const int kDefaultStartingSalary = 30000;
        
    private:
        std::string mFirstName;
        std::string mLastName;
        int mEmployeeNumber = -1;
        int mSalary = kDefaultStartingSalary;
        bool mHired = false;
    public:
        Employee(/* args */) = default;
        Employee(const std::string& firstName,
                const std::string& lastName);
        void promote(int raiseAmount = 1000);
        void demote(int demeritAmount = 1000);
        void hire();    // Hires and rehires the employee
        void fire();    // Dismisses the employee
        void display() const;   // Outputs employee info to console

        // Getters and setters
        void setFirstName(const std::string& firstName);
        const std::string& getFirstName() const;

        void setLastName(const std::string& lastName);
        const std::string& getLastName() const;

        void setEmployeeNumber(int employeeNumber);
        int getEmployeeNumber() const;

        void setSalary(int newSalary);
        int getSalary() const;

        bool isHired() const; 

        ~Employee();
    };
    
}


