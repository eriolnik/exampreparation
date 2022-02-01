//
// safestack
//

#include <iostream>
#include <mutex>
#include <stack>

template<class T>

class SafeStack {

private:
    std::stack<T> st;
    std::mutex mut; //обозначаем mutex

public:

    ~SafeStack() = default;

    SafeStack() = default;

    void Pop() {
        std::lock_guard<std::mutex> locker(mut); //mut.lock();
        if (!st.empty()) {
            st.pop();
        }
        //mut.unlock(), тк lockguard сам заботится о лок и анлок
    }

    void Push(const T &value) {
        std::lock_guard<std::mutex> locker(mut); //mut.lock();
        st.push(value);
        //mut.unlock();
    }

    bool TryPop(T &value) {
        if (st.top() == value) {
            if (mut.try_lock()) {
                st.pop();
                mut.unlock();
                return true;
            } else return false;
        } else {
            return false;
        }
    }
};
