/*
  Name: 通过对象地址访问虚函数
  Description: 
  1、由于指向虚函数表的vtbl在对象的首位置，且虚函数表保存的指向各虚函数的指针，
     若把虚函数当作一个数组，则需要一个二级指针指向该数组。 
     通过虚函数表调用虚函数，调用数序为print,job,~Person，和函数定义顺序相同。
  2、输出结果：
   sizeof(Person) = 12
   id = 10 age = 20 //print 
   Person job       //job 
   ~Person          //~Person 
   4//说明虚函数表中还有其他数据，暂不知道是啥。。虚函数表以NULL结尾 
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
               cout << "Person job" << endl;
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

typedef void (*Fun)();
int main()
{
    cout << "sizeof(Person) = " << sizeof(Person) << endl;
    Person person;
    
    /*int i = 0;
    Fun fun;
    while(i < 3)
    {
            //(int*)&person 取出person的地址 转换成int*类型的指针，//对象的地址，指向对象， 
            // (int*) *(int*)(&person) 提取该指针指向的内容,       //将指向的第一个地址vtbl的地址转化成指针 
            // 将指向的内容转换成int*类型的指针，即vtbl，
            //通过vtbl++,并转换成Fun*类型的函数指针，遍历虚函数表的内容。 
             
              fun = (Fun)*( ((int*)*(int*)(&person)) + i);
              fun();
              i++;
    }
    */ 
     
     //int **p = (int **)(&person); 
     //int **vtbl = (int **)*p; 获取虚函数表的地址 
    int **vtbl = (int**) *(int**)(&person);
     int i;
     for(i = 0; i < 3 && vtbl != NULL; ++i)
     {
               Fun fun = (Fun)(*vtbl);//获取虚函数表中函数的地址。 
               fun();
               vtbl++;
     }
     
     
     i = 0;
     vtbl = (int**) *(int**)(&person);
     while(*vtbl)
     {
                 ++vtbl;
                 ++i;
     }
     cout << i << endl;
     
     system("pause");
     return 0;
}
