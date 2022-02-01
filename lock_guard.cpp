//
// lock_guard
//
#include <mutex>

template <class Mutex>
class lock_guard {
private:
    Mutex *mut;
    bool ownership;

public:
    explicit lock_guard(Mutex &m) {  //конструктор
        mut = m;
        m.lock();
        ownership = true;
    }

    lock_guard(Mutex &m, std::adopt_lock_t) {
        mut = m;
        ownership = true;
    }

    lock_guard(const lock_guard &l) = delete;
//Первый конструктор конструирует объект типа lock_guard и блокирует lock_guard.
//Если MTX не является рекурсивным мьютексом,
// он должен быть разблокирован при вызове этого конструктора.
//Второй конструктор не блокирует MTX. MTX должен быть заблокирован при вызове этого конструктора.
// Конструктор не выдает никаких исключений.
    ~lock_guard() {  //Снимает блокировку mutex, переданного в конструктор.
        ownership = false;
        mut->unlock();
    }

    lock_guard &operator=(const lock_guard &) = delete;
};

//2 способ

template <class Mutex>
class lock_guard {
    Mutex& mut;

public:
    explicit lock_guard(Mutex& mut)
            : mut(mut)
    {
        mut.lock();
    }
    lock_guard(Mutex& mut, std::adopt_lock_t t)
            : mut(mut)
    {}

    ~lock_guard()
    {
        mut.unlock();
    }

    lock_guard(const lock_guard&) = delete;
    lock_guard& operator=(const lock_guard&) = delete;
};