
#include <string>
#include <iostream>

#include "json11.hpp"

int deserialize(std::string& str)
{
    std::string err;
    auto jsonRoot = json11::Json::parse(str, err);
    if( !jsonRoot.is_object() )
    {
        std::cout << "failed shape." << std::endl;
        return -1;
    }

    auto ret = jsonRoot.has_shape(json11::Json::shape{
        {"testObject", json11::Json::OBJECT},
        {"testArray", json11::Json::ARRAY}
    }, err);
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

    return 0;
}

int main()
{
    std::string str;

    serialize(str);

    deserialize(str);

    return 0;
}

//g++ --std=c++11 json11.cpp test.cpp -o testJson