#include <vector>
#include <string>

#include <boost/array.hpp>

// 
class IBaseConf{
public:
    virtual int reloadConf() = 0;
    virtual int loadConf() = 0;

private:
    std::string m_modName;
    std::string m_path;
    std::string m_confFileName;
};
/* alt + shift + a */
//ctrl + a
class CConfMangaer
{
public:
    int addNewConf(std::shared_ptr<IBaseConf>);

    int reloadConf();
private:
    std::vector<std::shared_ptr<IBaseConf>> m_ConfList;
};

struct ConfInfoTest
{

};

class CConfTest :public IBaseConf
{
public:
    CConfTest();
    ~CConfTest();

    virtual int loadConf();
    virutal int reloadConf();

    std::string& readFileIntoString();
public:
    ConfInfoTest m_TestConf;
};