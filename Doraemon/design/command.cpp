
#include <iostream>

/* mt的业务，命令描述 ；可以扩展command类型，以增加mt的特色功能 */
/* 模式描述某一类过程的基本角色，便于清晰把握其内部流程，扩展新的功能 */

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
/* 类似函数对象，std::function的设计，就是命令模式的体现；
 * command可以扩展出多种对象，然后不同的行为，多态的体现依靠command的wrapper
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