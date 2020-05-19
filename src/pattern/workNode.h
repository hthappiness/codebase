

#include <memory>

class CWorkNode
{
    using pfEndHandler = std::function<void ()>;
public:
    CWorkNode(pfEndHandler = nullptr);
    virtual void process(Doraemon::base::UnifiedMap&) = 0;
    virtual void setFlag(bool bFlag) = 0;
    void finish(Doraemon::base::UnifiedMap& data)
    {
        if( nullptr == m_next ) /* last node */
        {
            m_handler(data);
        }
        else /* next node */
        {
            m_next->process(data);
        }
    }
    void setNext(std::shared_ptr<CWorkNode> next){m_next = next;}
    void processNext();
private:
    std::shared_ptr<CWorkNode> m_next;
    pfEndHandler m_handler;
    bool m_workFlag;
};

class CWorkChain
{
public:
    explicit CWorkChain(std::shared_ptr<CWorkNode>& startNode)
    {
        m_startNode = startNode；
    }
    ~CWorkChain();

    int start();

    int destroy();

    /* called in consume */
    int inData();
private:
    std::shared_ptr<CWorkNode> m_startNode;
    bool m_closed;

    /* statistic infomation about  */
    int m_iCnt;
};

class CDecryptNode : public CWorkNode
{
public:
    virtual void process(Doraemon::base::UnifiedMap&);
};

class CDecodeNode : public CWorkNode
{
public:
    virtual void process(Doraemon::base::UnifiedMap&);
};

class CVencNode : public CWorkNode
{
public:
    virtual void process(Doraemon::base::UnifiedMap&);
};

class CEncryptNode : public CWorkNode
{
public:
    virtual void process(Doraemon::base::UnifiedMap&);
};

class CWmWorkChain : public CWorkChain
{
public:
    static std::shared_ptr<CWmWorkChain> create(int iFlag)
    {
        auto spDecryptNode = std::make_shared<CDecryptNode>();
        auto spDecodeNode = std::make_shared<CDecodeNode>();
        spDecryptNode->setNext(spDecodeNode);
        auto spVencNode = std::make_shared<CVencNode>();
        spDecodeNode->setNext(spVencNode);
        auto spEncryptNode = std::make_shared<CEncryptNode>();
        spVencNode->setNext(spEncryptNode);

        auto spWmWorkChain = std::make_shared<CWmWorkChain>(spDecryptNode);

        return spWmWorkChain;
    }
};