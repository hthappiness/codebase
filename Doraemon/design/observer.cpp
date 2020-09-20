
/* 观察者，当目标发生改变时需要通知观察者对象，调用其update接口 */
class Observer
{
public:
	int update();
};

/* 目标，知道其观察者 */
class Subject
{
public:
	int attach(Observer*);

	int detach(Observer*);

	int notify()
	{
		for (auto& item : m_observerList)
		{
			item->second->update();
		}
	}
private:
	std::map<id, Observer*> m_observerList;
};

class ConcreteObserver : public Observer
{
	/* 与subject状态保持一致 */
	enum class observerState
	{

	};
private:
	std::weak_ptr<Subject> m_subject;
};

class ConcreteSubject : public Subject
{
	/* 将有关状态存入各ConcreteObserver对象 */
	enum class subjectState
	{

	};
};