
class CVoteRequest
{
public:
    int serialization();
public:
    int m_iTerm; //第几轮投票
    
};

class CVoteResponse
{

};

enum ENodeType{
        FS_NODE_DENTRY,
        FS_NODE_LEAF,
        FS_NODE_NOT_INIT
    };

class CBaseFsNode
{
public:
    CBaseFsNode(std::string& partPath, ENodeType eNodeType)
        :m_urlPartName(partPath),m_nodeType(eNodeType)
    {}
private:
    std::string m_urlPartName;
    ENodeType m_nodeType;
};

class CDentryNode :public CBaseFsNode
{
public:
    CDentryNode(std::string& partPath)
        :CBaseFsNode(partPath, FS_NODE_DENTRY)
    {}
    int addSubNode(CBaseFsNode*);
    int delSubNode(CBaseFsNode*);
private:
    std::vector<CBaseFsNode*> m_subNodeVec;
};

class CFileNode : public CBaseFsNode
{
    CFileNode(std::string& partPath)
        :CBaseFsNode(partPath, FS_NODE_LEAF)
    {}
};

class CRemoteFs
{
public:
    int addFile();
private:
    //输入url，输出url类型
    //返回0为成功，其余错误码
    int parseUrl(std::string&, std::vector<std::string>&);
    int checkUrlRepeated();
 private:
    CDentryNode* m_rootDentry;
};