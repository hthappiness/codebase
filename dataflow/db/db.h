
//基于特定的数据库接口适配接口，定义接口
class CDb
{
protected:
	virtual int putData(string& key, string& value) = 0;
	virtual int getData(string& key, string& value) = 0;
}

class CDbWrapper
{
public:
	static CDbWrapper* createDb();
	int putData(string& key, string& value);
	int getData(string& key, string& value);
	int configDb();
private:
	CDb* m_dbImpl;  //leveldb + other
};