

#映射协议内容的相关概念，也就是定义数据对象

把不同层次的信息放在对应的对象；为了描述这个过程，zookeeper内部需要记录的状态---最清晰、最安全、最轻便（这都是价值）
SID（服务器的唯一标识）和ZXID（事务ID）

集群：
    cluser state : 广播模式， 恢复模式， 同步模式 ； LOOKING,  FOLLOWING, OBSERVING, LEADING
    raftServer：
        server id : myid
        zxid : 32:epoch  32:transaction id （对某个server来说，选择出了leader，epoch才会+1， 逻辑时钟的自增规则； 事务id是标识prososal的id）
        role state : leader, follower, observer

        判断投票给谁的规则---差异点： 先看数据zxid,数据zxid大者胜出;其次再判断leader Serverid----myid,leader Serverid大者胜出】

        method：
            变更状态
            发起投票vote
            接收投票
            处理和统计投票 ： 每次对收到的投票的处理，都是对(vote_sid, vote_zxid)和(self_sid, self_zxid)对比的过程，相关逻辑都在这几个变量里。  https://blog.csdn.net/tengxvincent/article/details/81094665
            改变服务器状态

文件系统： znode


投票消息 vote msg：
    serverId : myid
    Zxid 
    
    server的流程-----处理消息，根据自身的状态去处理投票消息

每台机器发出投票后，也会收到其他机器的投票，每台机器会根据一定规则来处理收到的其他机器的投票，并以此来决定是否需要变更自己的投票，这个规则也是整个Leader选举算法的核心所在（关键在于，确定怎样的规则才能更好地达到目的：这需要的是逻辑，归纳出规则，演绎出可能出现的情况，然后反馈到规则本身，以达到自洽的情况，这就是逻辑思维的核心体现。规则是大前提，演绎出各种可能的情况，用问题又去修正规则本身，这是反馈和完善的过程）

使用GRPC + c++完成一个这样的分布式数据共享功能，集群管理
软件工程要求：
1、制定迭代计划
2、代码要求
    善用设计模式
3、

./nodeManager --nodeId=1 --nodeSum=5 