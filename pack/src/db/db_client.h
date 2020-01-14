
#if 0
class CPgSql
{
    using retType=PGResult;
};

class CMySql
{
    using retType=xxxx;
}; 
#endif
class CPgSqlDao
{
     using retType = PGResult;
};

template<typename daoType>
class CDbClient{

    static std::shared_ptr<CDbClient> getInstance();

    template<typename daoType>
    class DbResult{
        daoType::retType getNextRow();

        int getInt(daoType::retType& result, long lRow, long lColumn);
        std::string getString(daoType::retType& result, long lRow, long lColumn);
        std::date getData(daoType::retType& result, long lRow, long lColumn);
    };

    CDbClient();
    ~CDbClient();

    int connect();
    int reconnect();

    DbResult execSQL(std::string sqlBuf){
        return impl->execSQL(sqlBuf);
    }

    static daoType* impl;
};

//template<typename daoType>
//daoType* CDbClient<daoType>::impl = new daoType();

template<typename daoType>
static daoType CDbClient<daoType>::impl = nullptr;

//client层不感知具体的数据库操作返回类型，在该层中其统一被命名为DbResult(这就是抽象)，根据模板参数确定实际的参数
//client层，用何种语言形式描述不同数据库的操作以及其返回值类型，这就是抽象
//本质在于，用何种抽象来联结不同的数据和方法，联结不同的概念