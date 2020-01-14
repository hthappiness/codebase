
class CRaftNode
{
public:
    CRaftNode(){
        m_NodeState = NODE_FOLLOWER;
    }
    ~CRaftNode();

    int processMsg(); //heartBeat, voteRequest, 

private:

     enum NodeState{
        NODE_FOLLOWER,
        NODE_CANDIDATE,
        NODE_LEADER
    };

    void setNodeState(NodeState state){ m_NodeState = state;}

private:
    int m_iVoteTerm;
    NodeState m_NodeState;
    CRandomTimer m_RTimer; //重置timer的时机；接收到消息，hearBeat、vote、

    CNetTransfer m_tranfer; //发送消息，接收消息
};