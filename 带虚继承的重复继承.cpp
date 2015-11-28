/*
  Name: 带虚继承的重复继承  
  Description: 
1、输出结果
 vtble = 0x442d6c
        [-1]: 4464588
        [0]: 0x410ff0-->Derived::fun1
        [1]: 0x41101c-->Derived::funD
 *p = 100
 vtble = 0x442d88
        [-1]: 4464588
        [0]: 0x43df78-->Derived::fun1
        [1]: 0x410eac-->Base::funA
        [2]: 0x410f1c-->Base1::funB
 *p = 1
 *p = 10
 vtble = 0x442da8
        [-1]: 4464588
        [0]: 0x43df78-->Derived::fun1
        [1]: 0x410eac-->Base::funA
        [2]: 0x410f9c-->Base2::funC
 *p = 1
 *p = 20
2、类关系
                    Base--fun1, funA
                  /      \
  fun1, funB---Base1     Base2--fun1,funC
          virtual \        / virtual
                 Derived--fun1,funD
3、内存分布
  |-----------------------------|
  |     Derived类的对象         |     |----------------------------------------------|
  |                             |     | 虚函数表 存储指向各虚函数的指针              |
  |   |----------------------|  |     |----------------------------------------------|
  |   |       vtbl-----------+--+---->|Derived::fun1 | Base1::funB |Derived::funD  | | 
  |   |----------------------|  |     |----------------------------------------------|
  |   |   Base1::base1(10)   |  |
  |   |----------------------|  |     |----------------------------|
  |   |       vtbl-----------+--+---->|Derived::fun1 | Base2::funC |
  |   |----------------------|  |     |----------------------------|
  |   | Base2::base2(20)     |  |
  |   |----------------------|  |
  |   |Derived::derived(100) |  |
  |   |----------------------|  |     |---------------------------|
  |   |       vtbl-----------+--+---->|Derived::fun1 | Base::funA |
  |   |----------------------|  |     |---------------------------|
  |   | Base::base(1)        |  |
  |   |----------------------|  |
  |                             |  
  |-----------------------------| 
4、结论：
 1)使用虚继承后，在派生类的对象中只存在一份的Base子对象，从而避免了二义性。
   由于是多重继承，且有一个虚基类（Base），所以Derived类拥有三个虚函数表，其对象存在三个vptr。
   如上图所示，第一个虚函数表是由于多重继承而与第一基类（Base1）共享的主表，
   第二个虚函数表是与其他基类（Base2）有关的次表，第三个是虚基类的虚函数表。          
 2)类Derived的成员与Base1中的成员排列顺序相同，首先是以声明顺序排列其普通基类的成员，
 接着是派生类的成员，最后是虚基类的成员。
 3)派生自虚基类的派生类的虚函数表中，也不含有虚基类中的virtual函数，
 派生类重写的virtual函数会在所有虚函数表中得到更新。
 4)在类Derived的对象中，Base（虚基类）子对象部分为共享区域，而其他部分为不变区域。
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

class Base1 : virtual public Base
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

class Base2 : virtual public Base
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
    
    cout << "vtbl = " << vtbl << endl;
    cout << "\t[-1]: " << (int)vtbl[-1] << endl;
    for(int i = 0; i < 3 && vtbl[i]; ++i)
    {
            cout << "\t[" << i << "]: " << vtbl[i] << "-->";
            Fun fun = (Fun)(vtbl[i]);
            fun();
    }
    p++;
    cout << "*p = " << *p << endl;//p=&Base::base
    
    p++;
    vtbl = (int**)(*p);//p=vtbl 
    cout << "vtbl = " << vtbl << endl;
    cout << "\t[-1]: " << (int)vtbl[-1] << endl;
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
    vtbl = (int**)(*p);//p=vtbl 
    cout << "vtble = " << vtbl << endl;
    cout << "\t[-1]: " << (int)vtbl[-1] << endl;
    for(int i = 0; i < 4 && vtbl[i]; ++i)
    {
            cout << "\t[" << i << "]: " << vtbl[i] << "-->";
            Fun fun = (Fun)(vtbl[i]);
            fun();
    }
    
    p++;
    cout << "*p = " << *p << endl;    
    
    system("pause");
    return 0;
}
