#include <list>
#include <mutex>
#include <condition_variable>
#include <chrono>

template <typename T>
class BlockQueue
{
private:
    std::list<T *> list;
    int size = 0;
    int limit = 0;
    std::mutex mu;
    std::condition_variable cond;

public:
    int add(T &t, int waitTime)
    {
        std::unique_lock<std::mutex> lck(mu);
        while (size == limit)
        {
            if (waitTime < 0)
            {
                cond.wait(lck);
            }
            else
            {
                cond.wait_for(lck, std::chrono::milliseconds(waitTime));
                if (size == limit)
                {
                    return -1;
                }
            }
        }
        list.insert(&t);
        size++;
        cond.notify_one();
        return 1;
    }

    T *pop(int waitTime)
    {
        std::unique_lock<std::mutex> lck(mu);
        while (size == 0)
        {
            if (waitTime < 0)
            {
                cond.wait(lck);
            }
            else
            {
                cond.wait_for(lck, std::chrono::milliseconds(waitTime));
                if (size == 0)
                {
                    return nullptr;
                }
            }
        }
        T *t = list.front();
        list.pop_front();
        size--;
        cond.notify_one();
        return t;
    }

    int getSize()
    {
        return size;
    }
};