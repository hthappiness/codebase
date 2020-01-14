
enum DataState
{
    ITEM_UNCOMMITED,
    ITEM_COMMITED
};

class CDataRecord
{
private:
    unsigned int m_uiUnCommitedCnt;
};

class CDataItem
{
public:
    int setState(DataState state);
private:
    DataState m_dateState;
};

class CJsonDataItem :public CDataItem
{

};