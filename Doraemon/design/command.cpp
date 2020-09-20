
#include <iostream>

/* mt��ҵ���������� ��������չcommand���ͣ�������mt����ɫ���� */
/* ģʽ����ĳһ����̵Ļ�����ɫ�����������������ڲ����̣���չ�µĹ��� */

#define __interface class


__interface receiver
{
public:
	template<typename... Args>
	virtual int invoke(Args... arg) = 0;
};

class command
{
public:
	command(receiver* executor)
	{
		m_receiver = executor;
	}

	template<typename... Args>
	int execute(Args... arg)
	{
		return m_receiver->invoke(std::forward<Args>(arg)...);
	}
private:
	receiver* m_receiver;
};
/* ���ƺ�������std::function����ƣ���������ģʽ�����֣�
 * command������չ�����ֶ���Ȼ��ͬ����Ϊ����̬����������command��wrapper
 */

class UiItem
{
public:

protected:
	command m_command;
};

class Button : public UiItem
{
	int clicked()
	{
		int id = 1;
		m_command.execute("button", id, "test button");
	}
};

class ButtonReceiver : public receiver
{
public:
	class param
	{
	public:
		param()
	};
	template<typename... Args>
	virtual int invoke(Args... arg) override
	{
		
	}
};