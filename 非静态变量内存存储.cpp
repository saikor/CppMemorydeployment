#include <iostream>
#include <stdlib.h>

using namespace std;

class Person
{
public:
       Person():Id(10), Age(20){}
       void print()
       {
            cout << "id = " << Id << " age = " << Age << endl;
       }
private:
        int Id;
        int Age;           
};

int main()
{
    Person p1;
    cout << "sizeof(p1) = " << sizeof(p1) << endl;
    int *p = (int*)(&p1);
    
    cout << "p.id = " << *p << " address = " << p << endl;
    
    p++;
    cout << "p.age = " << *p << " address = " << p << endl;
    
    
    Person p2;
    cout << "sizeof(p2) = " << sizeof(p2) << endl;
    p = (int*)(&p2);
    
    cout << "p.id = " << *p << " address = " << p << endl;
    
    p++;
    cout << "p.age = " << *p << " address = " << p << endl;
    system("pause");
    return 0;
}
