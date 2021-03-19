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
    friend class MakePerson;
    friend std::ostream& operator << (std::ostream& os, const Person& p) {
        return os << "Name: " << p.m_name
            << "\nAge: " << p.m_age
            << "\nSalary: " << p.m_salary
            << "\nNet worth: " << p.m_netWorth;
    }
};

class MakePerson {
    Person m_person;

public:
    MakePerson& withName(const std::string& name) { m_person.m_name = name; return *this; }
    MakePerson& withAge(int age) { m_person.m_age = age; return *this; }
    MakePerson& withSalary(double salary) { m_person.m_salary = salary; return *this; }
    MakePerson& withNetWorth(double netWorth) { m_person.m_netWorth = netWorth; return *this; }
    operator Person() { return m_person; } // can implicitly convert to Person
};

int main() {
    Person p = MakePerson()
        .withName("Ruse")
        .withAge(24)
        .withSalary(100.)
        .withNetWorth(100.);

    std::cout << p << "\n";

    return 0;
}
