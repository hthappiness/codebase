/*********************************
 * process the input 
 * abstract syntax tree
 * ******************************/
enum SyntaxType
{
    TERMINAL,
    UNTERMINAL,
};

enum ForSyntaxType
{
    ITEM_IF,
    ITEM_FOR,
    ITEM_OPEN_BRACE,  //左括号
    ITEM_CLOSE_BRACE, //右括号
    ITEM_EXP,
    ITEM_COMMA,      //逗号
    ITEM_SEMICOLON,  //分号
    ITEM_OTHER,
}

//generate forma
//关于产生式的计算，FIRST集合

//input processing code
// lookhead


class CAst
{

};