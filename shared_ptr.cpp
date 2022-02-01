//
//SharedPtr
//

#include <atomic>

#include "memory"
#include "utility"

template<typename T>
class SharedPtr {
private:
    T *object{nullptr};
    std::atomic_uint *count{nullptr};

public:
    SharedPtr() { count = new std::atomic_uint(0); }

    explicit SharedPtr(T *ptr) {
        object = ptr;
        count = new std::atomic_uint(0);
        *count += 1;
    }

    SharedPtr(const SharedPtr &r) {
        count = r.count;
        object = r.object;
        *count += 1;
    }

    SharedPtr(SharedPtr &&r) {
        count = std::move(r.count);
        object = std::move(r.object);
        *count += 1;
    }

    ~SharedPtr() {
        if (*count != 0) {
            --(*count);
        }
        if (*count == 0) {
            delete object;
            delete count;
        }
    }

    auto operator=(const SharedPtr &r) -> SharedPtr & {
        delete count;
        object = r.object;
        count = r.count;
        *count += 1;
        return *this;
    }

    auto operator=(SharedPtr &&r) -> SharedPtr & {
        delete count;
        object = std::move(r.object);
        count = std::move(r.count);
        *count += 1;
        return *this;
    }

    operator bool() const { return object != nullptr; }

    auto operator*() const -> T & { return *object; }

    auto operator->() const -> T * { return object; }

    auto get() -> T * { return object; }

    T *release() {
        T *newptr = object;
        (*count)--;
        delete object;
        return newptr;
    }

    void reset() {
        delete count;
        count = new std::atomic_uint(0);
        object = nullptr;
    }

    void reset(T *ptr = nullptr) {
        *count -= 1;
        delete count;
        count = new std::atomic_uint(0);
        object = ptr;
    }

    void swap(SharedPtr &r) {
        std::swap(object, r.object);
        std::swap(count, r.count);
    }
};
//2 способ
template <class T>
class shared_ptr {
    std::atomic<unsigned int>* quantity_;
    T* ptr_;
    void Deleter() {
        --(*quantity_);
        if (*quantity_ == 0) {
            if (ptr_ != nullptr) {
                delete ptr_;
                ptr_ = nullptr;
            }
            if (quantity_ != nullptr) {
                delete quantity_;
                quantity_ = nullptr;
            }
        }
    }

public:
    shared_ptr() {
        quantity_ = nullptr;
        ptr_ = nullptr;
    }

    ~shared_ptr() {
        Deleter();
    }

    /*shared_ptr(shared_ptr<T>&& rhs) {
        ptr_(std::move(rhs.ptr_));
        quantity_(std::move(rhs.quantity_));
    }*/

    shared_ptr<T>& operator=(shared_ptr<T>&& rhs) noexcept {
        std::swap(*this, rhs);
        rhs.Deleter();
        return *this;
    }

    shared_ptr<T>& operator=(const shared_ptr<T>& rhs) {
        if (ptr_ != nullptr) {
            delete ptr_;
            ptr_ = nullptr;
        }

        if (quantity_ != nullptr) {
            delete quantity_;
            quantity_ = nullptr;
        }

        ptr_ = rhs.ptr_;
        quantity_ = rhs.quantity_;
        ++(*quantity_);
        return *this;
    }

    shared_ptr(const shared_ptr<T>& rhs) {
        ptr_ = rhs.ptr_;
        quantity_ = rhs.quantity_;
        ++(*quantity_);
    }

    void reset(T* ptr = nullptr) {
        if (ptr_ != ptr) {
            Deleter();
            ptr_ = ptr;
            *quantity_ = 1;
        }
    }

    T* release() {
        return ptr_;
    }

    void swap(shared_ptr<T>& rhs) {
        if (this != &rhs) {
            std::swap(quantity_, rhs.quantity_);
            std::swap(ptr_, rhs.ptr_);
        }
    }

    T* operator->() {
        return ptr_;
    }

    T& operator*() {
        return *ptr_;
    }

    operator bool() const {
        return ptr_;
    }

    T* get() {
        return ptr_;
    }
};