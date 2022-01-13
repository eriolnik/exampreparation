//safequeue

#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template<class T>
class SafeQueue {
private:
    std::queue<T> q;
    std::mutex m;

public:

    SafeQueue() = default;

    ~SafeQueue() = default;

    void Pop() { //удаление последнего
        std::lock_guard<std::mutex> locker(m); //потокобезопасная зона
        q.pop();
    }

    void Push(const T &value) { //добавление элемента первый зашел первый вышел
        std::lock_guard<std::mutex> locker(m); //захват мютекса   потокобезопасная зона
        q.push(value);
    }

    bool TryPop(T &value) { //реализация проверки потокобезопасности
        if (q.front() == value) {
            if (m.try_lock()) {
                q.pop();
                m.unlock();
                return true;
            } else {
                return false;
            }
        } else return false;
    }

    size_t Size() {
        return q.size();
    }
};
