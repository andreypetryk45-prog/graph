#pragma once

#include <iostream>
#include <utility>
#include <stdexcept>
#include <cstdlib>
#include <new>

template <typename T>
class Vector
{
private:
    int m_size;
    int n_capacity;
    T *array;

public:
    class Iterator
    {
    private:
        int index;
        Vector<T> &vector;

    public:
        Iterator(int index, Vector<T> &vector) : index(index), vector(vector) {}
        Iterator &operator++()
        {
            index += 1;
            return *this;
        }
        T &operator*() const
        {
            return vector[index];
        }
        bool operator!=(const Iterator &other) const
        {
            return &this->vector != &other.vector || this->index != other.index;
            // two iterators are the same if they refering to the same object and have the same index
            // therefore || is used
        }
    };
    Iterator begin()
    {
        return Iterator(0, *this);
    }
    Iterator end()
    {
        return Iterator(size(), *this);
    }

    class Const_iterator
    {
    private:
        int index;
        const Vector<T> &vector;

    public:
        Const_iterator(int index, const Vector<T> &vector) : index(index), vector(vector) {}

        Const_iterator &operator++()
        {
            index += 1;
            return *this;
        }
        const T &operator*() const
        {
            return vector[index]; // is allowed, since there's const overloaded []
        }
        bool operator!=(const Const_iterator &other) const
        {
            bool same_index = this->index == other.index;
            if (&this->vector == &other.vector && same_index) // checks if both iterators refers to the same Vector object by comparing their address
            {
                return false; // one and the same object are the same, therefore false is returned
            }
            return true;
        }
    };
    Const_iterator begin() const
    {
        return Const_iterator(0, *this);
    }
    Const_iterator end() const
    {
        return Const_iterator(size(), *this); // using size(), since as soon as Iterator readches end() it should stop
    }

    Vector() : m_size(0), n_capacity(0), array(nullptr) {}
    Vector(const Vector &other)
    {
        this->n_capacity = other.n_capacity;
        this->m_size = other.m_size;

        // the most important part of the entire Abstract Data Type - Vector:
        //  we're allocating memory in one, whole and contiguous(послідовному) piece, which boosts the working speed
        // because of reducing Cache Missing
        this->array = (T *)std::malloc(other.n_capacity * sizeof(T)); // allocating memory for a vactor //вижіляємо памʼять по бітам

        for (int k = 0; k < other.m_size; k++)
        {
            new (&array[k]) T(other.array[k]); // конструюємо кожний елемент масиву
        }
    }
    Vector(Vector &&other) noexcept : array(nullptr), m_size(0), n_capacity(0) // a move-constructor for r-value object construction
    {
        std::swap(this->array, other.array);
        std::swap(this->m_size, other.m_size);
        std::swap(this->n_capacity, other.n_capacity);
    }

    Vector &operator=(const Vector &other)
    {
        if (this != &other) // in case they're the same vector
        {
            // creating a copy on the stack so that it will be deleted as soon as the function has ended
            Vector new_vector(other); // deep-copying the vector passing by an operator

            // now swaping them:
            std::swap(this->array, new_vector.array);
            std::swap(this->m_size, new_vector.m_size);
            std::swap(this->n_capacity, new_vector.n_capacity);
        }
        return *this; // returning the object itself, therefore an asterisk is used
    }
    Vector &operator=(Vector &&other) noexcept
    {
        if (this != &other)
        {
            this->clear();

            this->array = other.array;
            this->m_size = other.m_size;
            this->n_capacity = other.n_capacity;

            other.array = nullptr;
            other.m_size = 0;
            other.n_capacity = 0;
        }
        return *this;
    }

    bool is_empty() const
    {
        return m_size == 0;
    }
    int size() const
    {
        return m_size;
    }
    int capacity() const
    {
        return n_capacity;
    }
    bool contains(const T &value) const
    {
        if (is_empty())
        {
            return false;
        }
        for (const auto &element : *this)
        {
            if (element == value)
            {
                return true;
            }
        }
        return false;
    }
    void clear()
    {
        for (int j = 0; j < this->m_size; j++)
        {
            this->array[j].~T(); // звільняємо кодний елмент вектора
        }
        std::free(this->array); // звільняємо сиру памєять

        // reseting all atributes:
        this->array = nullptr;
        this->m_size = 0;
        this->n_capacity = 0;
    }
    void pop_back()
    {
        if (!is_empty())
        {
            array[m_size - 1].~T();
            --m_size;
            return;
        }
        throw std::out_of_range("Vector is empty!");
    }
    void pop_front()
    {
        if (is_empty())
        {
            throw std::out_of_range("Vector is empty!");
        }
        erase(0);
    }

    void resize()
    {
        // COPYING ELEMENTS AND DELETING OLD ARRAY'S ELEMENTS
        int new_capacity = (n_capacity == 0) ? 1 : 2 * n_capacity;
        T *new_array = (T *)std::malloc(new_capacity * sizeof(T)); // allocating memory
        for (int i = 0; i < m_size; i++)
        {
            new (&new_array[i]) T(std::move(array[i])); // new(&new_array[i]) - каже, де потрібно сконструктувати обʼєкт, типу Т,  std::move(array[i]) - говорить, що саме потрібно туди скопіювати
            array[i].~T();                              // invoking a deconstructor for the old array element
        }
        std::free(array); // freeing memory for the pointers

        // UPDATING INFO
        n_capacity = new_capacity; // updating the n_capacity value
        array = new_array;         // making array point to the newly created array
    }
    void push_back(const T &value)
    {
        T temp(value);
        if (m_size + 1 > n_capacity)
        {
            resize();
        }

        // T(std::move(element)) - конструктор типу Т
        new (&array[m_size]) T(std::move(temp)); // вставляємо новий елмент в адресу виділеної памʼяті, вик. конструктор
        ++m_size;
    }
    void push_front(const T &value)
    {
        T temp(value);
        if (m_size + 1 > n_capacity)
        {
            resize();
        }
        if (m_size == 0)
        {
            new (&array[0]) T(std::move(temp));
            ++m_size;
            return;
        }

        for (int i = m_size; i > 0; i--)
        {
            if (i == m_size)
            {
                new (&array[i]) T(std::move(array[i - 1]));
            }
            else
            {
                array[i] = std::move(array[i - 1]);
            }
        }
        array[0].~T();
        new (&array[0]) T(std::move(temp));
        ++m_size;
    }
    void remove(const T &value)
    { // removes all vector's elements with such a value
        if (is_empty())
        {
            return;
        }
        T temp(value);
        for (int i = 0; i < m_size;)
        {
            if (array[i] == temp)
            {
                erase(i); // the size has decreased, so  we do not need to increase 'i'
                // More importantly, the element now at the position 'i' hasn't been checked yet
                // because all elements to its reight have been shifted one position to the left
            }
            else
            {
                i++; // just moving forward, nothing's been changed
            }
        }
    }
    void erase(int index)
    {
        if (is_empty() || index < 0 || index >= m_size)
        {
            return;
        }
        for (int i = index; i < m_size - 1; i++)
        {                                       // using move-assignment operator
            array[i] = std::move(array[i + 1]); // moving the next object redarding i to i-th cell
        }
        array[m_size - 1].~T(); // deconstructing the last vector's element
        --m_size;

        // we can rewrite it using move-constructor: //though it's memory wasting, since we create what already exists
        //  for(int i = index; i < size - 1; i++){
        //      array[i].~T(); // destroying the current i-th vector's element
        //      //in the cell that's just been destroyed constructing new vector's element, using move-constructor
        //      new(&array[i]) T(std::move(array[i + 1]));
        //      //since we've already created an element (i + 1)th, we destroy it:
        //      array[i + 1].~T();
        //  }
        //  --size;
    }

    T &operator[](int index)
    { // передаємо посилання для прямої роботи зі створеним обʼєктом а не копією
        if (is_empty())
        {
            throw std::out_of_range("Vector is empty");
        }
        else
        {
            if (index < -1 || index >= m_size)
            {
                throw std::out_of_range("Index out of range!");
            }
        }
        if (index == -1)
        {
            index = m_size - 1;
        }

        return array[index];
    }
    const T &operator[](int index) const
    { // for displaying
        if (is_empty())
        {
            throw std::out_of_range("Vector is empty");
        }
        else
        {
            if (index < -1 || index >= m_size)
            {
                throw std::out_of_range("Index out of range!");
            }
        }
        if (index == -1)
        {
            index = m_size - 1;
        }

        return array[index];
    }
    bool operator==(const Vector &other) const
    {
        if (this->is_empty() && other.is_empty())
        {
            return true; // if they're empty, they're the equal
        }
        if (this->size() != other.size()) // if they aren't of the same size, they aren't equal
        {
            return false;
        }
        typename Vector<T>::Const_iterator it = this->begin();
        for (const T &element : other)
        {
            if (*it != element)
            {
                return false;
            }
            ++it;
        }
        return true;
    }
    friend std::ostream &operator<<(std::ostream &os, const Vector<T> &vector)
    {
        os << "[";
        for (int i = 0; i < vector.size(); i++)
        {
            os << vector[i];
            if (i < vector.size() - 1)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    ~Vector()
    {
        clear();
    }
};
