#include "conf.hpp"
#include "json11.hpp"

#include <boost/filesystem.hpp>

int CConfTest::readFileIntoString()
{
    ifstream iFile(m_confFileName);
    ostringstream StrJson;

    char ch;

    while(strJson && iFile.get(ch))
    {
        strJson.put(ch);
    }

    iFile.close();

    return strJson.str();

}

int CConfTest::loadConf()
{
    if( !boost::filesystem::exist(m_confFileName) )
    {
        return -1;
    }

    if( boost::ends_with(m_path, "/") )
    {
        return -1;
    }

    /* read file*/
    auto strConf = readFileIntoString(m_confFileName);

    /* parse json */
    std::string err;
    auto jsonObj = json11::Json::JsonParse(strConf, err);
    if( !err.empty() )
    {
        std::cout<<err<<std::endl;
        return -1;
    }

    std::string path = jsonObj["testName"].string_value();

    return 0;
}