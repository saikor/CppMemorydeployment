/*
  Name: 多重继承内存分布  
  Description: 
1、输出结果：
 vtble = 0x441d1c
        [-1]: 4460476
        [0]: 0x410e70-->Derived::fun1
        [1]: 0x410dbc-->Base1::fun2
        [2]: 0x410e9c-->Derived::fun4
 *p = 10
 vtble = 0x441d30
        [-1]: 4460476
        [0]: 0x43dda0-->Derived::fun1
        [1]: 0x410e2c-->Base2::fun3
 *p = 20
 *p = 100
 vtbl[0]: 0x410e70
 vtbl[1]: 0x410dbc
 vtbl[2]: 0x410e9c
 vtbl[3]: 0xfffffff8
 vtbl[4]: 0x440fbc
 vtbl[5]: 0x43dda0
 vtbl[6]: 0x410e2c
 size_of_vble = 7
2、类关系
fun1, fun2---Base1     Base2--fun1,fun3
                \        / 
                 Derived--fun1,fun4
3、内存分布：
  |-----------------------------|
  |     Derived类的对象         |     |--------------------------------------------|
  |                             |     | 虚函数表 存储指向各虚函数的指针            |
  |   |----------------------|  |     |--------------------------------------------|
  |   |       vtbl-----------+--+---->|Derived::fun1 | Base1::fun2 | Derived::fun4 |
  |   |----------------------|  |     |--------------------------------------------|
  |   | Base1::base1(10)     |  |
  |   |----------------------|  |     |-----------------------------|
  |   |       vtbl-----------+--+---->|Derived::fun1 | Base2::fun3  |
  |   |----------------------|  |     |-----------------------------|
  |   | Base2::base2(20)     |  |
  |   |----------------------|  |
  |   |Derived::derived1(100)|  |
  |   |----------------------|  |  
  |-----------------------------| 
4、说明：
 1)在多重继承中，一个子类拥有n-1张额外的虚函数表，n表示继承的基类个数。也就是
 一个派生类会有n个虚函数表，其中一个为主表，与第一个基类(如Base1)共享，其他表
 次表，与其他基类无关。
 2)子类新声明的virtual函数，放在主表中。
 3)每一个父类的子对象在子类的对象保持原样性，并一次按声明次序排列。 
 4)若子类重写virtual函数，则其所有父类中相同签名的virtual都会被改写。如fun1，
 则两个虚函数表中fun1函数的项都会更新为子类重写的函数的新地址。目的是为了解决
 父类类型的指针指向同一个子类实例，能够调用到实际的函数。
*/

#include <iostream>
#include <stdlib.h>

using namespace std;

class Base1
{
public:
       Base1()
       {
             base1 = 10; 
       }
       virtual void fun1()
       {
               cout << "Base1::fun1" << endl;
       }
       virtual void fun2()
       {
               cout << "Base1::fun2" << endl;
       }
private:
    int base1;                 
};

class Base2
{
public:
       Base2()
       {
              base2 = 20;
       }
       virtual void fun1()
       {
               cout << "Base2::fun1" << endl;
       }
       virtual void fun3()
       {
               cout << "Base2::fun3" << endl;
       }
private:
        int base2;       
};

class Derived : public Base1, public Base2
{
public:
       Derived() : Base1(), Base2()
       {
                 derived1 = 100;
       }
       virtual void fun1()
       {
               cout << "Derived::fun1" << endl;
       }
       virtual void fun4()
       {
               cout << "Derived::fun4" << endl;
       }       
private:
        int derived1;       
};

typedef void(*Fun)();
int main()
{
    Derived derived;
    
    int *p = (int*)(&derived);
    int **vtbl = (int**)(*p);//p=vtbl
    cout << "vtble = " << vtbl << endl;
    cout << "\t[-1]: " << (long)vtbl[-1] << endl;
    for(int i = 0; vtbl[i] && i < 3; ++i)
    {
            cout << "\t[" << i << "]: " << vtbl[i] << "-->";
            Fun fun = (Fun)vtbl[i];
            fun();
    }
            
    p++;
    cout << "*p = " << *p << endl;//p=10
    
    p++;
    vtbl = (int**) (*p);//p=vtbl
    cout << "vtble = " << vtbl << endl;
    cout << "\t[-1]: " << (long)vtbl[-1] << endl;
    for(int i = 0; vtbl[i] && i < 3; ++i)
    {
            cout << "\t[" << i << "]: " << vtbl[i] << "-->";
            Fun fun = (Fun)vtbl[i];
            fun();
    }
    
    p++;
    cout << "*p = " << *p << endl;//p=20
    p++;
    cout << "*p = " << *p << endl;//p=100
    
    int cnt=0;//测试虚函数表中数据地址个数
    vtbl = (int**) *(int**)(&derived);
    while(*vtbl)
    {                
            cout << "vtbl[" << cnt << "]: " << *vtbl << endl;
            vtbl++;
            cnt++;
    }
    cout << "size_of_vble = " << cnt << endl;
    
    
    system("pause");
    return 0;
}
