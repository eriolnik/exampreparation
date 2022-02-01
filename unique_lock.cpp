//
// unique_lock (guard)
//

#include <mutex>
#include <system_error>

template <class Mutex>

class unique_lock {
private:
    Mutex *m = nullptr;
    bool own{false};

public:
    unique_lock() = default;

    ~unique_lock() {
        if (own) m->unlock();
        delete m;
    }

    unique_lock(unique_lock &&other) noexcept {
        if (!other.m.try_lock()) {
            m = std::move(other.m);
            m->lock();
        } else
            m = std::move(other.m);
        other.own = false;
        own = true;
    }

    explicit unique_lock(Mutex &mut) {
        m = mut;
        m->lock();
        own = true;
    }

    unique_lock(Mutex &mut, std::defer_lock_t) noexcept {
        m = mut;
        own = false;
    }
/*adopt_lock_t это грубо говоря флаг который указывает lock_guard`у,
    что передаваемый mutex уже захвачен и не нужно вызывать вызыватьт метод lock,
    в противном случае будет повторный захват mutex`а и неопределенное поведение*/
    unique_lock(Mutex &mut, std::adopt_lock_t) {
        m = mut;
        own = true;
    }

    void lock() {
        if (m == nullptr) throw std::system_error{};
        if (own) throw std::system_error{};
        m->lock();
        own = true;
    }

    void unlock() {
        if (m == nullptr) throw std::system_error{};
        if (!own) throw std::system_error{};
        m->unlock();
        own = false;
    }

    unique_lock<Mutex> &operator=(unique_lock<Mutex> &&ul) noexcept {
        m = std::move(ul.m);
        own = ul.own;
    }

    unique_lock<Mutex> &operator=(const unique_lock<Mutex> &ul) = delete;

    unique_lock(const unique_lock &) = delete;
};