//scoped_ptr
#include <memory>

template<class T>
class scoped_ptr {
private:
    T *ptr;
public:
    scoped_ptr(T *value) { ptr = value; };

    ~scoped_ptr() { delete ptr; }

    T *operator->() { return ptr; }

    T &operator*() { return *ptr; }

    T *get() { return ptr; }
};

template<typename T, typename... Args>
scoped_ptr<T> make_scoped(Args &&... args) {
    return scoped_ptr<T>(new T(std::forward<Args>(args)...));
}
//Возвращает ссылку rvalue на argifarg, не являющуюся ссылкой на значение lvalue.

//Если argэто ссылка на значение lvalue, функция возвращает argбез изменения ее типа.
// функция make_unigue на вход принимает r-value сколько-то аргументов args

#include <memory> // 2 способо

template<class T>
class scoped_ptr {
public:
    explicit scoped_ptr(T *value = nullptr);

    ~scoped_ptr();

    T *operator->();

    T &operator*();

    T *get() const;

private:
    T *ptr;
};

template<class T>
scoped_ptr<T>::scoped_ptr(T *value) : ptr(value) {}

template<class T>
scoped_ptr<T>::~scoped_ptr() {
    delete ptr;
}

template<class T>
T *scoped_ptr<T>::operator->() {
    return ptr;
}

template<class T>
T &scoped_ptr<T>::operator*() {
    return *ptr;
}

template<class T>
T *scoped_ptr<T>::get() const {
    return ptr;
}

template<typename T, typename... Args>
scoped_ptr<T> make_scoped(Args &&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}