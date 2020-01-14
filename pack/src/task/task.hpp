
#include <list>

class IBaseTask
{
public:
    int iTaskType;
    int iTaskStatus;

    CTime cUpdateTime;
    


    virtual bool isRealTime() = 0;
private:
    bool bRealTime;
    std::array<std::shared_ptr<IBaseSubTask>> m_subTaskArr;

    std::string m_TaskDesc;

private:
};

/* 构建型模式 */
class IBaseTaskType
{
public:
    virtual std::shared_ptr<IBaseTask> createTask() = 0;
    virtual int queryTask() = 0;
    virtual int resumeTask() = 0;
    virtual int pauseTask() = 0;
    virtual int deleteTask() = 0;

    virtual int recoverTask() = 0; 
    virtual int deleteSubTask() = 0;
};

/*处理通用的任务流程
 *1、提供任务的创建操作接口
 *2、提供任务的保活处理流程、异常处理流程
 *3、提供任务的调度、分发
 *4、包含内部线程
 *5、
 */
class CBaseTaskManager
{
public:
    int createTask();
    int deleteTask();

private:
    std::list<std::shared_ptr<IBaseTask>> m_listTask;

    std::array<std::shared_ptr<IBaseTaskType>> m_TaskTypeArr;

    std::thread m_thread;
};