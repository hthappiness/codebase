
#define DF_REGISTER_TYPE(name_, type_)\
static constexpr struct name_##_T{\
    using type = type_;\
    static constexpr char const* const name = #name_;\
}name_

/* 
constexpr struct MsgID_T{
    using type = unsigned long;
    static constexpr char const* const name = "MsgID"
}MsgID

Key::type, Key::name

*/

DF_REGISTER_TYPE(MsgID, unsigned long);