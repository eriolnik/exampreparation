//
// stack lock free
//

#include <atomic>

template<typename T>
class stack {
public:
    struct node {
        T data;
        node *next;
    };

    void push(const T &value) {
        node *new_head = new node(value, head.load());

        // теперь установим new_node новым head, но если head
        // больше не является тем, что хранится в new_node->next
        // (другой поток мог вставить свой new_node прямо сейчас)
        // то присвоим новый head в new_node->next и повторим
        while (!head.compare_exchange_weak(new_head->next, new_head)) {};
    }

    bool TryPop(T &value) {  //Возвращает значение, указывающее,
        // есть ли объект в верхней части стека<T><T>,
        // если он присутствует, копирует его в resultпараметр и
        // удаляет его из стека<T><T>.
        if (head.data != nullptr) {
            value = head.load();
            node *nod = value;
            while (node &&
                   !head.compare_exchange_weak(node, node->next)
                    ) { }
            return true;
        } else return false;
    }

private:
    std::atomic<node *> head;
};