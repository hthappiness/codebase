
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
 
//pthread_key_t key; // 好吧，这个玩意究竟是干什么的呢？

class CPerThreadParamHndl
{
public:
	CBaseThreadParam()
	{
		pthread_key_create(&key, NULL);
	}
	
	~CBaseThreadParam()
	{
		pthread_key_delete(key);
	}
	
	void setThreadParam(void* data)
	{
		pthread_setspecific(key, &data);
	}
	
	void* getThreadParam()
	{
		return pthread_getspecific(key);
	}
private:
	pthread_key_t key;
};

class CThreadContextParam
{
public:
	int setPerThreadParam(void* data)
	{
		m_threadLocalHndl.setThreadParam(data);
	}
	
	void* getPerThreadParam()
	{
		return m_threadLocalHndl.getThreadParam();
	}
private:

	static CPerThreadParamHndl m_threadLocalHndl;
}