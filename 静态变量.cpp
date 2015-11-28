 /*
  Name: 加入静态变量的对象的内存分布
  Description: 
 1、说明静态变量存储在对象之外。 static成员函数也放在类的对象之外
 2、输出结果：和非静态变量打印结果相同，
  sizeof(Person) = 8
  sizeof(p1) = 8
  p.id = 10 address = 0x22ff20   
  p.age = 20 address = 0x22ff24

  sizeof(p2) = 8
  p.id = 10 address = 0x22ff00
  p.age = 20 address = 0x22ff04
 3、内存分布图
  |-----------------------------|
  |     Person类的对象          |           |-----------------| 
  |   |-----------------|       |           | Person::sCouont |
  |   |  int Id         |       |           |-----------------|
  |   |-----------------|       |
  |   |  int Age        |       |           |---------------------| 
  |   |-----------------|       |           | Person::PersonCount |
  |-----------------------------|           |---------------------| 
  
*/

#include <iostream>
#include <stdlib.h>

using namespace std;

class Person
{
public:
       Person():Id(10), Age(20){ ++count; }
       ~Person(){ --count; }
       void print()
       {
            cout << "id = " << Id << " age = " << Age << endl;
       }
       
       static int PersonCount()
       {
              return count;
       }
private:
        int Id;
        int Age;   
        static int count;        
};

int Person::count = 0;
int main()
{
    cout << "sizeof(Person) = " << sizeof(Person) << endl;
    
    Person p1;
    cout << "sizeof(p1) = " << sizeof(p1) << endl;
    int *p = (int*)(&p1);
    
    cout << "p.id = " << *p << " address = " << p << endl;
    
    p++;
    cout << "p.age = " << *p << " address = " << p << endl;
    
    cout << endl;
    
    Person p2;
    cout << "sizeof(p2) = " << sizeof(p2) << endl;
    p = (int*)(&p2);
    
    cout << "p.id = " << *p << " address = " << p << endl;
    
    p++;
    cout << "p.age = " << *p << " address = " << p << endl;
    system("pause");
    return 0;
}
