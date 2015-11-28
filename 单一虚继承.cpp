/*
  Name: 单一虚继承 
  Description: 
1、输出结果
 vtble = 0x441ccc
        [-1]: 4460420
        [0]: 0x410d10-->Base1::fun1
        [1]: 0x410d3c-->Base1::funB
 *p = 10
 vtble = 0x441ce4
        [-1]: 4460420
        [0]: 0x43dc7c-->Base1::fun1
        [1]: 0x410ccc-->Base::funA
 *p = 1
2、类关系
               Base--fun1,funA
               |
               |virtual
               |
               Base1--fun1,funB
3、内存分布
  |-----------------------------|
  |     Base1类的对象           |     |--------------------------------------------|
  |                             |     | 虚函数表 存储指向各虚函数的指针            |
  |   |----------------------|  |     |--------------------------------------------|
  |   |       vtbl-----------+--+---->|Base1::fun1 | Base1::funB                   | 
  |   |----------------------|  |     |--------------------------------------------|
  |   | Base1::base1(10)     |  |
  |   |----------------------|  |     |-----------------------------|
  |   |       vtbl-----------+--+---->|Base1::fun1 | Base::funA     |
  |   |----------------------|  |     |-----------------------------|
  |   | Base::base(1)        |  |
  |   |----------------------|  |
  |                             |  
  |-----------------------------|
4、结论
 一、与单一继承相比，内存布局明显不同： 
   1)成员的顺序问题。在普通的单一继承中，基类的成员位于派生类的成员之前。
    而在单一虚继承中，首先是派生类的成员，然后是虚基类的成员。
   2)vptr的个数问题。在普通的单一继承中，派生类只有一个虚函数表，所以其对象只有一个vtbl。
    而在单一虚继承中，派生类的虚函数表有n个（n为虚基类的个数）额外的虚数函数表，
    即总有n+1个虚函数表。
   3)派生自虚基类的派生类的虚函数表中，并不含有虚基类中的virtual函数，
    但是派生类重写的virtual函数会在所有虚函数表中得到更新
 二、一个类如果内含一个或多个虚基类对象，像Base1那样，将会被分割为两部分：
 一个不变区域和一个共享区域。
 不变区域中的数据，不管后续如何变化，总是拥有固定的偏移量（从对象的开头算起），
 所以这一部分可以被直接存取。共享区域所对应的就是虚基类对象。
*/

#include <iostream>
#include <stdlib.h>
using namespace std;
/*
class BaseC 
{
public:
       BaseC()
       {
             basec = 200;
       }
       virtual void fun1()
       {
               cout << "BaseC::fun1" << endl;
       }
       
       virtual void funC()
       {
               cout << "BaseC::funC" << endl;
       }
//protected:
private:            
          int basec;       
};
*/
class Base
{
public:
       Base()
       {
             base = 1;
       }
       virtual void fun1()
       {
               cout << "Base::fun1" << endl;
       }
       
       virtual void funA()
       {
               cout << "Base::funA" << endl;
       }
//protected:
private:            
          int base;       
};

class Base1 : virtual public Base//, virtual public BaseC
{
public:
       Base1() : Base()
       {
               base1 = 10;
       }
       virtual void fun1()
       {
               cout << "Base1::fun1" << endl;
       }
       virtual void funB()
       {
               cout << "Base1::funB" << endl;
       }
private:
        int base1;       
};

typedef void(*Fun)();
int main()
{
    Base1 base1;
    int *p = (int*)(&base1);
    int **vtbl = (int**)(*p);//p=vtbl 
    cout << "vtble = " << vtbl << endl;
    cout << "\t[-1]: " << (int)vtbl[-1] << endl;
    for(int i = 0; i < 3 && vtbl[i]; ++i)
    {
            cout << "\t[" << i << "]: " << vtbl[i] << "-->";
            Fun fun = (Fun)(vtbl[i]);
            fun();
    }
    
    p++;
    cout << "*p = " << *p << endl;//p=&Base1::base1 
        
    p++;
    vtbl = (int**)(*p);//p=vtbl 
    cout << "vtble = " << vtbl << endl;
    cout << "\t[-1]: " << (int)vtbl[-1] << endl;
    for(int i = 0; i < 3 && vtbl[i]; ++i)
    {
            cout << "\t[" << i << "]: " << vtbl[i] << "-->";
            Fun fun = (Fun)(vtbl[i]);
            fun();
    }
    
    p++;
    cout << "*p = " << *p << endl;//p=&Base::base
    /*
    p++;
    vtbl = (int**)(*p);//p=vtbl 
    cout << "vtble = " << vtbl << endl;
    cout << "\t[-1]: " << (int)vtbl[-1] << endl;
    for(int i = 0; i < 3 && vtbl[i]; ++i)
    {
            cout << "\t[" << i << "]: " << vtbl[i] << "-->";
            Fun fun = (Fun)(vtbl[i]);
            fun();
    }
    
    p++;
    cout << "*p = " << *p << endl;//p=&Base::base
    */
    system("pause");
    return 0;
}
