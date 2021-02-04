// Simple builder class:
// We set the builder as a friend class and remove the need for any public constructors.
// Especially useful when constructing objects which take many parameters. 
// No need to remember the order of the parameters in the constructor!

#include <iostream>

class Person {	
    std::string m_name{ "" };
    int         m_age{ 0 };
    double      m_salary{ 0. };
    double      m_netWorth{ 0. };

    Person() = default;

public:
    // Need this so the PersonBuilder can set the Person's private data without (messy) setters
    // Also to gain access to the private constructor
    friend class PersonBuilder;

    void printInfo() {
        std::cout << "Age: " << m_age << "\nName: " << m_name << "\nSalary: " << m_salary << "\nNet worth: " << m_netWorth << "\n";
    }
};

class PersonBuilder {
    Person m_person;

public:
    PersonBuilder& setName(const std::string& name) { m_person.m_name = name; return *this; }
    PersonBuilder& setAge(int age) { m_person.m_age = age; return *this; }
    PersonBuilder& setSalary(double salary) { m_person.m_salary = salary; return *this; }
    PersonBuilder& setNetWorth(double netWorth) { m_person.m_netWorth = netWorth; return *this; }
    Person build() { return m_person; }
};

int main() {
    PersonBuilder builder;
    Person p = builder
        .setName("Ruse")
        .setAge(24)
        .setSalary(100.)
        .setNetWorth(100.)
        .build();

    p.printInfo();

    return 0;
}
    
class Test {
    
};