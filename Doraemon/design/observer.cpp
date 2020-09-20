
/* �۲��ߣ���Ŀ�귢���ı�ʱ��Ҫ֪ͨ�۲��߶��󣬵�����update�ӿ� */
class Observer
{
public:
	int update();
};

/* Ŀ�֪꣬����۲��� */
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
	/* ��subject״̬����һ�� */
	enum class observerState
	{

	};
private:
	std::weak_ptr<Subject> m_subject;
};

class ConcreteSubject : public Subject
{
	/* ���й�״̬�����ConcreteObserver���� */
	enum class subjectState
	{

	};
};