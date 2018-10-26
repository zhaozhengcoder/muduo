#include <iostream>
#include "MyMutexLock.h"
#include <string>
#include <memory>
#include <map>

using namespace std;

class STOCK
{
    private:
        string _name;
    public:
        STOCK(string name)
        {
            this->_name = name;
        }

        void print()
        {
            cout<<_name<<endl;
        }

        ~STOCK()
        {
            cout<<"[del stock obj ]"<<_name<<endl;
        }
};

class STOCK_FACTORY
{
private:
    MyMutexLock _mutex;
    map<string ,weak_ptr<STOCK>> _stock;
public:
    shared_ptr<STOCK> get_stock(const string & stockname)
    {
        if(_stock.find(stockname) != _stock.end())
        {
            //find 
            if(!_stock[stockname].expired())
            {
                shared_ptr<STOCK> ptr = _stock[stockname].lock();
                return ptr;
            }
            else
            {
                shared_ptr<STOCK> ptr(new STOCK(stockname));
                _stock[stockname] = ptr;
                return ptr;
            }
        }
        // 两个else的重复代码是可以合并的，但是为了逻辑清晰，就没有合并
        else
        {
            //not find
            shared_ptr<STOCK> ptr(new STOCK(stockname));
            _stock[stockname] = ptr;
            return ptr;
        }
    }
};

// muduo 上面的例子，实现一个对象池 
// 使用智能指针的一个很好的例子
int main()
{
    STOCK_FACTORY s;
    string stockname = "google";
    {
        shared_ptr<STOCK> ptr = s.get_stock(stockname);
        ptr->print();
    }

    cout<<"end"<<endl;

    {
        shared_ptr<STOCK> ptr = s.get_stock(stockname);
        ptr->print();
    }

    return 0;
}

//  g++ -g factory.cc -std=c++11