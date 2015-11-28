/*
  Name: 非虚继承的重复继承  
  Description: 
1、输出结果
 vtble = 0x441d54
        [-1]: 4460492
        [0]: 0x410ed0-->Derived::fun1
        [1]: 0x410d8c-->Base::funA
        [2]: 0x410dfc-->Base1::funB
        [3]: 0x410efc-->Derived::funD
 *p = 1
 *p = 10
 vtble = 0x441d6c
        [-1]: 4460492
        [0]: 0x43de00-->Derived::fun1
        [1]: 0x410d8c-->Base::funA
        [2]: 0x410e7c-->Base2::funC
 *p = 1
 *p = 20
 *p = 100 
2、类关系
                    Base--fun1, funA
                  /      \
  fun1, funB---Base1     Base2--fun1,funC
                \        / 
                 Derived--fun1,funD
3、内存分布
  |-----------------------------|
  |     Derived类的对象         |     |---------------------------------------------------------|
  |                             |     | 虚函数表 存储指向各虚函数的指针                         |
  |   |----------------------|  |     |---------------------------------------------------------|
  |   |       vtbl-----------+--+---->|Derived::fun1 | Base::funA | Base1::funB | Derived::funD |
  |   |----------------------|  |     |---------------------------------------------------------|
  |   | Base::base(1)        |  |
  |   |----------------------|  |
  |   | Base1::base1(10)        |  |
  |   |----------------------|  |     |------------------------------------------|
  |   |       vtbl-----------+--+---->|Derived::fun1 | Base::funA | Base2::funC  |
  |   |----------------------|  |     |------------------------------------------|
  |   | Base::base(1)        |  |
  |   |----------------------|  |
  |   | Base2::base2(20)     |  |
  |   |----------------------|  |
  |   |Derived::derived(100) |  |
  |   |----------------------|  |  
  |-----------------------------| 
4、结论:
 1)重复继承后，位于继承层次顶端的父类Base分别被子类Base1和Base2继承，并被Derived继承，
 所以在Derived的对象中，存在Base1的对象成员，又存在Base2的对象成员，且这两对象都Base的对象，
 所以Base对象成员(base)在Derived中存在两份。
 2)二义性的原因，由于在子类的对象中，存在两份父类的成员，当通过继承Base类的其他函数(且这些函数又被继承)
 可以修改base时，就会产生歧义，因为derived对象中存在两份base变量，所以编译器不能判断究竟要
 修改哪一个成员变量？ 所以在访问Base中的成员时，需要加上域作用符来明确说明是哪一个如：
 Base1::base = 2; 
  
*/
#include <iostream>
#include <stdlib.h>
using namespace std;

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

class Base1 : public Base
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

class Base2 : public Base
{
public:
       Base2() : Base()
       {
               base2 = 20;
       }
       virtual void fun1()
       {
               cout << "Base2::fun1" << endl;
       }
       virtual void funC()
       {
               cout << "Base2::funC" << endl;
       }
private:
        int base2;       
};


class Derived : public Base1, public Base2
{
public:
       Derived() : Base1(), Base2()
       {
                 derived = 100;
       }
       virtual void fun1()
       {
               cout << "Derived::fun1" << endl;
       }
       virtual void funD()
       {
               cout << "Derived::funD" << endl;
       }
private:
        int derived;
};

typedef void (*Fun)();
int main()
{
    Derived derived;
    int *p = (int*)(&derived);
    int **vtbl = (int**)(*p);//p=vtbl 
    cout << "vtble = " << vtbl << endl;
    cout << "\t[-1]: " << (int)vtbl[-1] << endl;
    for(int i = 0; i < 4 && vtbl[i]; ++i)
    {
            cout << "\t[" << i << "]: " << vtbl[i] << "-->";
            Fun fun = (Fun)(vtbl[i]);
            fun();
    }
    
    p++;
    cout << "*p = " << *p << endl;//p=&Base::base
    p++;
    cout << "*p = " << *p << endl;//p=&Base1::base1
    
    
    p++;
    vtbl = (int**)(*p);//p=vtbl 
    cout << "vtble = " << vtbl << endl;
    cout << "\t[-1]: " << (long)vtbl[-1] << endl;
    for(int i = 0; i < 4 && vtbl[i]; ++i)
    {
            cout << "\t[" << i << "]: " << vtbl[i] << "-->";
            Fun fun = (Fun)(vtbl[i]);
            fun();
    }
    
    p++;
    cout << "*p = " << *p << endl;
    p++;
    cout << "*p = " << *p << endl;
    p++;
    cout << "*p = " << *p << endl;
    
    system("pause");
    return 0;
}
