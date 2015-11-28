/*
  Name: 加入virtual成员函数的对象的内存分布
  Description: 
  1、C++中虚函数通过虚函数表实现，类的对象为每一个虚函数产生一个指针，
  放在虚函数表里，对象内会存储一个指向虚函数表的指针vtbl，vtbl的设定和重置都由每一个
  类的构造函数、析构函数和复制构造函数运算符自动完成。
  2、32位系统，指针占4个字节
  3、输出结果为： 
  sizeof(Person) = 12
  sizeof(person) = 12
  *p = 4463760 address = 0x22ff20
  *p = 10 address = 0x22ff24
  *p = 20 address = 0x22ff28
  4、结论：内存分布为
  |-----------------------------|
  |     Person类的对象          |     |--------------------------------|
  |                             |     | 虚函数表 存储指向各虚函数的指针|
  |   |-----------------|       |     |--------------------------------|
  |   |  vtbl(4个字节)--+-------+---->|print  | job  |   ~Person       |
  |   |-----------------|       |     |--------------------------------|
  |   |  int Id         |       |
  |   |-----------------|       |
  |   |  int Age        |       |
  |   |-----------------|       |
  |-----------------------------|

*/
#include <iostream>
#include <stdlib.h>

using namespace std;

class Person
{
public:
       Person():Id(10), Age(20){ ++sCount; }
       
       static int PersonCount()
       {
              return sCount;
       }
       
       virtual void print()
       {
            cout << "id = " << Id << " age = " << Age << endl;
       }
       
       virtual void job()
       {
               cout << "Person" << endl;
       }
       virtual ~Person()
       { 
               --sCount; 
               cout << "~Person" << endl;
       }
       
private:
        int Id;
        int Age;   
        static int sCount;        
};

int Person::sCount = 0;
int main()
{
    cout << "sizeof(Person) = " << sizeof(Person) << endl;
    Person person;
    cout << "sizeof(person) = " << sizeof(person) << endl;
    
    int *p = (int*)(&person);
    for(int i = 0; i < sizeof(person) / sizeof(int); ++i, ++p)
    {
            cout << "*p = " << *p << " address = " << p << endl;
    }
     
    system("pause");
    return 0;
}
