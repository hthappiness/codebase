

#include <memory>
#include <functional>

#include "Doraemon\base\noncopyable.h"

//如果想在该模式中添加更多的内容，就必须做更多的抽象，使之成为这个层次的内容

class CWorkNode
{
    using pfEndHandler = std::function<void ()>;
public:
    CWorkNode(pfEndHandler handler = nullptr);

    /* When finishing ， call finish interface*/
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
    
    void setNext(std::shared_ptr<CWorkNode>& next){m_next = next;}
    
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
        m_startNode = startNode;
    }
    ~CWorkChain();

    int start();

    int destroy();

    /* called in consume */
    int inData()
    {
        //m_startNode->process();
    }
private:
    std::shared_ptr<CWorkNode> m_startNode;
    bool m_closed;

    /* statistic information about  */
    int m_iCnt;
};

class CDecryptNode : public CWorkNode
{
public:
    virtual void process(Doraemon::base::UnifiedMap&);
};

class CWatermarkNode : public CWorkNode
{
public:
    virtual void process(Doraemon::base::UnifiedMap&);
};

class CEncryptNode : public CWorkNode
{
public:
    virtual void process(Doraemon::base::UnifiedMap&);
};


//CVideoChannel, 只用和WmWorkChain耦合
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