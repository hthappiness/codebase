#include<iostream>
#include<vector>
#include<list>
 
using namespace std;
 
class SliderWidget
{
public:
    SliderWidget();
    {
        std::cout<<"Slider Widget created"<<std::endl;
    }
     
};
 
class BoxWidget
{
public:
    BoxWidget()
    {
        std::cout<<"Box Widget created"<<std::endl;
    }
};
 
template <class T>
class OpNewCreator{
public:
    static T* create()
    {
        return new T;
    }
protected:
    ~OpNewCreator(){}
};
 
template <class T>
class MallocCreator
{
public:
    static T* create()
    {
        void *buf=std::malloc(sizeof(T));
        if(!buf) return 0;
        return new(buf) T;
    }
protected:
    ~MallocCreator(){}
};
 
template <class T>
class PrototypeCreator
{
public:
    PrototypeCreator(T * pObj=0):pPrototype(pObj)
    {
    }
     
    T* create()
    {
        return pPrototype?pPrototype->clone():0;
    }
     
    T* getPrototype(){return pPrototype;}
    void setPrototype(T* pObj){pPrototype=pObj;}
     
protected:
    ~PrototypeCreator(){}
     
private:
    T* pPrototype;
}
 
template<class T>
class ContainerVec
{
public:
    void push(T* widget)
    {
        mVecContainer.push_back(widget);
    }
     
    ~ContainerVec(){}
     
private:
    std::vector<T*> mVecContainer;
};
 
template<class T>
class COntainerList
{
public:
    void push(T* widget)
    {
        mListContainer.insert(widget);
    }
     
    ~containerList(){}
private:
    std::list<T*> mListContainer;
};
 
template<class T,
         template<class> class CreationPolicy=MallocCreator,
         template<class> class Container=ContainerVec>
class WidgetManager:public CreationPolicy<T>  //模板的模板
{
public:
    typedef CreationPolicy<T> BaseClass;
    T* create()
    {
        T* tmp=BaseClass::create();
        mContainer.push_back(tmp);
        return tmp;
    }      
private:
    Container<T> mContainer;
}
 
typedef WidgetManager<BoxWidget,OpNewCreator,ContainerVec> BoxWidgetManager;
typedef WidgetManager<SliderWidget,OpNewCreator,ContainerList> SliderWidgetManager;
 
int main()
{
    BoxWidgetManager boxWidgetManager;
    BoxWidget* boxWidet=boxWidgetManager.create();
    cout<<typeid(BoxWidgetManager).name()<<endl;
     
    system("pause");
}