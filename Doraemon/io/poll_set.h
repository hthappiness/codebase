#pragma once

#include <mutex>

class CPollset;
class CPollsetWorker;

class CPollSetInterface
{
public:
    virtual void global_init(void) = 0;
    virtual void global_shutdown(void);
    virtual void init(CPollset *pollset, std::mutex **mu);
    virtual void shutdown(CPollset *pollset, grpc_closure *closure);
    virtual void destroy(CPollset *pollset);
    virtual int work(CPollset *pollset, CPollsetWorker **worker,
                                grpc_millis deadline);
    virtual int kick(CPollset *pollset,
                                CPollsetWorker *specific_worker);
    virtual size_t pollset_size(void);
};