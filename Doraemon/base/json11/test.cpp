
#include <string>
#include <iostream>
#include <fstream>

#include <unordered_map>

#include "json11.hpp"

/* 自定义对象 */
class CTestJson
{
public:
    CTestJson(std::string str = "testJson", int id = 255)
    {
        name_ = str;
        id_ = id;
    }
    //json11::Json to_json() 
    // compiler error:passing ‘const CTestJson’ as ‘this’ argument of 
    // ‘json11::Json CTestJson::to_json()’ discards qualifiers [-fpermissive] 去除限定符
    json11::Json to_json() const
    {
        return json11::Json{
            json11::Json::object{
                {"name", name_},
                {"id", id_}
            }
        };
    }
private:
    std::string name_;
    int         id_;
};

int deserialize(const std::string& str)
{
    std::string err;
    auto jsonRoot = json11::Json::parse(str, err);
    if( !jsonRoot.is_object() )
    {
        std::cout << "failed shape." << std::endl;
        return -1;
    }

    // using initialize_list, shape的定义是初始化列表，所以使用{},({})
    //c++98编译失败
    //map<int,std::string> _map{{1,"lxg"},{2,"the answer"},{3,"hello world."}};    ({ {}, {}, {}})
    auto ret = jsonRoot.has_shape(json11::Json::shape({
        {"testObject", json11::Json::OBJECT},
        {"testArray", json11::Json::ARRAY}
    }), err);
    if( false == ret)
    {
        std::cout << "failed shape." << std::endl;
        return -1;
    }

    auto obj = jsonRoot["testObject"].object_items();
    
    std::string name = obj["name"].string_value();
    int score = obj["score"].int_value();

    std::cout << name << " : " << score << std::endl;

    for (auto& objArray : jsonRoot["testArray"].array_items())
    {
        std::string name = objArray["name"].string_value();
        int score = objArray["score"].int_value();
        std::cout << name << " : " << score << std::endl;
    }

    auto varInt = jsonRoot["testInt"].int_value();
    std::cout << "int var = " << varInt << std::endl;
    
    return 0;
}

int serialize(std::string& str)
{
    json11::Json::object obj1 {
        {"name", "hting"},
        {"score", 99}
    };

    // array is the alias of std::vector<Json>
    //typedef std::map<std::string, Json> object;
    json11::Json::array obj2;
    obj2.emplace_back(json11::Json::object{
        {"name", "lixi"},
        {"score", 34} 
    }); // 初始化列表
    obj2.emplace_back(json11::Json::object{
        {"name", "guboyuan"},
        {"score", 244} 
    });

    // constructure function
    json11::Json root = json11::Json::object{
        {"testInt", 2222},
        {"testObject", obj1},
        {"testArray", obj2}
    };

    str = root.dump();

    std::cout << "dump result:" << str << std::endl;
#if 1
    //一个单独的string
    auto obj = json11::Json{"hello world"};
    auto test = obj.dump();

    std::cout << "dump result 2:" << test << std::endl; // "hello world"
#endif

#if 1
    //一个单独的string
    CTestJson jsonClass("xixixi", 890);
    //auto obj3  = json11::Json(jsonClass);  // explicit, both ok
    json11::Json obj3 = jsonClass;   // implicit,根据to_json构造
    auto test3 = obj3.dump();

    std::cout << "dump result 3:" << test3 << std::endl; // "hello world"
#endif
    return 0;
}

int persistize(std::string& jsonStr)
{
    // output , output to file
    std::ofstream fileStream("/home/hting/student.json");
    if( fileStream.is_open() )
    {
        fileStream << jsonStr << std::endl;

        fileStream.close();
    }

    return 0;
}

int readFile()
{
    std::ifstream fileStream("/home/hting/student.json");
    if( fileStream.is_open() )
    {
        std::string file;
        #if 1
        // 流操作,读取的数据大小是以一定格式进行的
        while( !fileStream.eof())
        {
            fileStream >> file ;
            std::cout << file << std::endl;
        }
       #else

        while( !fileStream.eof())
        {
            //getline 按行读取 \n
            std::getline(fileStream, file);
            std::cout << file << std::endl;
        }
        #endif

        fileStream.close();
    }

}

int test()
{
    json11::Json::object obj1 {
        {"name", "hting"},
        {"score", 99}
    };

    json11::Json::array arr{"1", 2, obj1};

    for(const auto& item : arr)
    {
        std::cout << "result:" <<static_cast<int>(item.type()) << std::endl;
    }

    std::unordered_map<std::string, int> unMap = {
        {"aaaa", 1},
        {"bbbb", 2},
        {"cccc", 3}
    };

    //explicit construct
    json11::Json mapJson = unMap;
    std::cout << "------end------: " << mapJson.dump() << std::endl;
}

int main()
{
    test();
#if 1
    std::string str;

    serialize(str);

    deserialize(str);

    persistize(str);

    readFile();
#endif
    return 0;
}

//g++ --std=c++11 json11.cpp test.cpp -o testJson