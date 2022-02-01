//
// vector
//
#include <cstring>
#include <stdexcept>
template <class T>
class Vector
{
private:
    T* data;
    size_t size;
    size_t capacity;
public:
    Vector() : size(0), capacity(16) {  //по умолчанию
        data = new T[capacity];
    }
    ~Vector() { //деструктор
        delete[] data;
    }
    Vector(const Vector& vec) : size(vec.size), capacity(vec.capacity) { //конструктор копирования
        if (size == 0) {
            data = nullptr;
        } else {
            data = new T[capacity];
            std::memcpy(data, vec.data, sizeof(T)*size);
        }
    }

    Vector(Vector&& vec) : size(vec.size), capacity(vec.capacity), data(vec.data)  { //конструктор перемещения
        vec.data = nullptr;
        vec.size = 0;
        vec.capacity= 0;
    }

    Vector& operator=(Vector&& rhs) noexcept { //перемещения
        if (this != &rhs) {
            capacity = rhs.capacity;
            size = rhs.size;
            delete[] data;
            data = rhs.data;

            rhs.data = nullptr;
            rhs.size = 0;
            rhs.capacity = 0;
        }
        return *this;
    }


    Vector& operator=(const Vector& rhs) { //копирования
        if (this != &rhs) {
            capacity = rhs.capacity;
            size = rhs.size;
            delete[] data;
            data = new T[capacity];
            std::memcpy(data, rhs.data, sizeof(T) * size);
        }
        return *this;
    }

    T& operator[](const size_t index) { //operator[]
        if (size > index)
        {
            return data[index];
        } else {
            throw std::range_error("Out of range");
        }
    }
    const T& operator[](const size_t index) const // Метод вставки элемента в произвольное место в массиве
    {
        if (index >= size) {
            throw std::range_error("Out of range");
        } else {
            return data[index];
        }
    }

    void push_back(const T& value) { // Метод добавления элемента в конец (push_back)
        if (capacity == size) {
            if (size == 0) {
                T* newData = new T[16];
                data = newData;
                capacity = 16;
            } else {
                T* newData = new T[size * 2];
                capacity = size * 2;
                std::memcpy(newData, data, (size) * sizeof(T)); //Копирует байты между буферами.
                delete[] data;
                data = newData;
            }
        }
        data[size] = value;
        ++size;
    }

};
