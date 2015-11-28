/*
  Name: 单一继承内存分布 
  Description: 
1、虚函数表中保存的是函数的指针，若把虚函数表当作一个数组，则要指向该数组需要个
   二级指针，即**vtbl, 获取函数指针的值(数组中元素的值)，可以通过vtbl[i] 获取。
    虚函数表中还保存着对象的类型信息，通常为了方便查找对象的类型信息，使用虚函数表
    中的索引(下标)为-1的位置保存对应的类型信息(即类std::type_info的对象)的地址，即
    保存在第一个虚函数的地址元素之前。 
2、输出结果 ：
    sizeof(Base) = 12
    sizeof(Derived) = 20
    vtble = 0x441d08
          [-1]: 4460500
          [0]: 0x410df0-->Base::fun1()//vtbl为第一个元素的地址 
          [1]: 0x410e6c-->Derived::fun2()
          [2]: 0x410e98-->Derived::fun3()
    vtbl[0]: 0x410df0
    vtbl[1]: 0x410e6c
    vtbl[2]: 0x410e98
    size_of_vble = 3//此虚函数表只有三个元素 
    对象中元素地址  元素值
    0x22ff20        4463880
    0x22ff24        20
    0x22ff28        30
    0x22ff2c        200
    0x22ff30        300
    0x22ff34        6168728 
3、结论 内存分布图 
  |-----------------------------|
  |     Derived类的对象         |     |-------------------------------------------|
  |                             |     | 虚函数表 存储指向各虚函数的指针           |
  |   |----------------------|  |     |-------------------------------------------|
  |   |  vtbl(4个字节)-------+--+---->|Base::fun1 | Derived::fun2 | Derived::fun3 |
  |   |----------------------|  |     |-------------------------------------------|
  |   | Base::base1(20)      |  |
  |   |----------------------|  |
  |   | Base::base2(30)      |  |
  |   |----------------------|  |
  |   |Derived::derived1(200)|  |
  |   |----------------------|  |
  |   |Derived::derived1(300)|  | 
  |   |----------------------|  |  
  |-----------------------------|
  1)vtbl位于对象的最前端
  2)非static的成员变量根据其继承顺序和声明顺序排在vtbl的后面
  3)派生类继承基类的所声明的虚函数，即基类的虚函数地址被复制到派生类的虚函数中相应位置。
  4)派生类中新加入的virtual函数跟在继承而来的virtual的后面。如fun3在fun1、fun2后面。
  5)若子类重写父类的virtual函数，则子类的虚函数表该virtual函数对应的项会更新为新函数的
  地址，如fun2被重新，虚函数表中fun2的地址为子类重新后的fun2的新地址。 
*/

#include <iostream>
#include <stdlib.h>
using namespace std;

class Base
{  
public:
       Base()
       {
             base1 = 20;
             base2 = 30;
       }
       virtual void fun1()
       {
               cout << "Base::fun1()" << endl;
       } 
       virtual void fun2()
       {
               cout << "Base::fun2()" << endl;
       }
private:
        int base1;
        int base2;                    
};

class Derived : public Base
{
public:
       Derived() : Base()
       {
                derived1 = 200;
                derived2 = 300;
       }
       virtual void fun2()  
       {
               cout << "Derived::fun2()" << endl;
       }    
       virtual void fun3()
       {
               cout << "Derived::fun3()" << endl;
       }
private:
        int derived1;
        int derived2;
};

typedef void (*Fun)();
int main()
{
    cout << "sizeof(Base) = " << sizeof(Base) << endl;
    cout << "sizeof(Derived) = " << sizeof(Derived) << endl;
    
    Derived drv;
    
     //查看虚函数表内容 
    int **vtbl = (int**)*(int**)(&drv);   
    cout << "vtble = " << vtbl << endl;
    cout << "\t[-1]: " << (long)vtbl[-1] << endl;
    for(int i = 0; vtbl[i] && i < 3; ++i)
    {
            cout << "\t[" << i << "]: " << vtbl[i] << "-->";
            Fun fun = (Fun)vtbl[i];
            fun();
    }
    int cnt=0;//测试虚函数表中数据地址个数
    while(*vtbl)
    {                
            cout << "vtbl[" << cnt << "]: " << *vtbl << endl;
            vtbl++;
            cnt++;
    }
    cout << "size_of_vble = " << cnt << endl;
    
    int *p = (int*)(&drv);
    
    cout << "对象中元素地址\t元素值" << endl;
    cout << p << "\t\t" << *p << endl;
    
    p++;
    cout << p << "\t\t" << *p << endl;
    
    p++;
    cout << p << "\t\t" << *p << endl;
    
    p++;
    cout << p << "\t\t" << *p << endl;
    p++;
    cout << p << "\t\t" << *p << endl;
    p++;
    cout << p << "\t\t" << *p << endl;
    system("pause"); 
    return 0;
} 
