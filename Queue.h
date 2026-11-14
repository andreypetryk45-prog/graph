#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <stdexcept>
#include <cstdlib>
#include <new>

template <typename T>
class Queue
{
private:
    T *array;                 // array where elemments are stored
    unsigned int m_size;        // cuurent number of elements within Queue
    unsigned int capacity;    // current capacity,  needs to dynamically increase an array
    unsigned int front_index; // index of the first element, it needs because QUEUE is implemented with a circular array

    void grow_capacity() // uses the same principle as in Vector
    {
        unsigned int new_capacity = (capacity == 0) ? 10 : 2 * capacity;
        T *temp_array = static_cast<T *>(std::malloc(new_capacity * sizeof(T)));
        if (temp_array == nullptr)
        {
            throw std::bad_alloc();
        }
        unsigned int constructed = 0;
        try
        {
            for (unsigned int i = 0; i < this->m_size; i++) // move-copying elements of *this
            {
                unsigned int index = (this->front_index + i) % this->capacity;
                new (&(temp_array[i])) T(std::move_if_noexcept(this->array[index])); // using std::move_if_noexcept to guarantes if there occures an exception the elements from this->array won't be moved, which guarantees data-safety
                constructed++;
            }

            for (unsigned int i = 0; i < this->m_size; i++) // destroying elements of this
            {
                unsigned int index = (this->front_index + i) % this->capacity; // to destroy only those objects that have been constucted
                this->array[index].~T();
            }
            this->front_index = 0;
            this->capacity = new_capacity;
            std::swap(this->array, temp_array);
            std::free(temp_array);
        }
        catch (...)
        {
            for (int i = 0; i < constructed; i++)
            {
                temp_array[i].~T();
            }
            std::free(temp_array);
            throw;
        }
    }
    void swap(Queue &other) noexcept
    {
        // creating a copy:
        T *temp_array = this->array;
        unsigned int temp_size = this->m_size;
        unsigned int temp_capacity = this->capacity;
        unsigned int temp_front = this->front_index;

        // copy other in this:
        this->array = other.array;
        this->m_size = other.m_size;
        this->capacity = other.capacity;
        this->front_index = other.front_index;

        // copy old this in oter:
        other.array = temp_array;
        other.m_size = temp_size;
        other.front_index = temp_front;
        other.capacity = temp_capacity;
    }
    unsigned int get_capacity() const
    {
        return this->capacity;
    }

public:
    Queue() : array(nullptr), m_size(0), front_index(0), capacity(0) {}                // default constructor
    Queue(const Queue &other) : array(nullptr), m_size(0), front_index(0), capacity(0) // copy constructor
    {
        if (other.capacity == 0) // if other is empty
        {
            this->array = nullptr;
            this->capacity = 0;
            this->front_index = 0;
            this->m_size = 0;
            return;
        }

        this->array = static_cast<T *>(std::malloc(other.capacity * sizeof(T))); // allocating raw memory stored in one row
        this->capacity = other.capacity;

        if (this->array == nullptr) // an error occured while byte-memory allocation
        {
            throw std::bad_alloc();
        }

        try
        {
            for (int i = 0; i < other.m_size; i++) // moving aloong only those elements that have been constructed
            {
                int index = (other.front_index + i) % other.capacity;
                new (&(this->array[i])) T(other.array[index]);
                ++this->m_size; // increacing it here in case bad_aloc()
            }
            this->front_index = 0; // new front is stored in the beginning of the array
        }
        catch (...) // an error occured while element construction
        {
            this->clear();
            throw; // throws the same exception: std::bad_aloc()
        }
    }
    Queue(Queue &&other) noexcept : // move constructor
                                    array(nullptr), m_size(0), front_index(0), capacity(0)
    {
        std::swap(this->array, other.array);
        std::swap(this->m_size, other.m_size);
        std::swap(this->front_index, other.front_index);
        std::swap(this->capacity, other.capacity);
    }
    Queue(const unsigned int capacity) : array(nullptr), m_size(0), front_index(0), capacity(0) // a parameterized constructor, to create a queue with a given capacity
    {
        if (capacity == 0)
        {
            return;
        }
        this->array = static_cast<T *>(std::malloc(capacity * sizeof(T)));
        if (this->array == nullptr)
        {
            throw std::bad_alloc();
        }
        this->capacity = capacity;
    }

    Queue<T> &operator=(const Queue<T> &other)
    {
        Queue<T> temp(other); // using our cope-constructor to not write the same code, 'temp' is created on the stack
        // which means that it'll be deleted as soon as the function's scope will end

        this->swap(temp);
        return *this;
    }
    Queue<T> &operator=(Queue<T> &&other) noexcept // move assignment operator
    {
        if (this == &other) // if they're the same object, return *this
        {
            return *this;
        }
        swap(other);
        return *this;
    }

    bool is_empty() const
    {
        return this->m_size == 0;
    }
    void clear()
    {
        if (capacity == 0)
        { // it's already empty
            return;
        }
        for (int i = 0; i < m_size; i++)
        {
            int index = (front_index + i) % capacity;
            array[index].~T(); // destroying objects stored in the array;
        }
        free(array);
        array = nullptr;

        m_size = 0;
        front_index = 0;
        capacity = 0;
    }
    unsigned int size() const
    {
        return this->m_size;
    }

    const T &front() const
    {
        if (is_empty())
        {
            throw std::out_of_range("Queue is empty!");
        }
        return array[front_index];
    }
    const T &back() const
    {
        if (is_empty())
        {
            throw std::out_of_range("Queue is empty!");
        }
        return array[(front_index + m_size - 1) % capacity];
    }

    void enqueue(const T &element) // addition of a new element in the end of the queue
    {
        const T temp_element(element);
        // In Queue, a new element is added in its end, and is deleted from its beginning -> FIFO
        if (this->m_size == this->capacity) // expanding array capacity if there's a need to
        {
            grow_capacity();
        }
        unsigned int index = (this->front_index + this->m_size) % this->capacity; // (front + number of existing elements in Queue) % the capacity of the array
        new (&(this->array[index])) T(temp_element);                              // if there occurs an exception while T(element) construction, its constructor will throw an exception therefore there won't be an element added
        ++this->m_size;
    }
    void dequeue() // deletion happens from the front
    {
        if (is_empty())
        {
            throw std::out_of_range("Queue is empty!");
        }
        array[front_index].~T();
        --m_size;
        if (m_size == 0)
        {
            front_index = 0; // to simplify the order within a queue
            return;
        }
        front_index = (front_index + 1) % capacity;
    }

    friend std::ostream &operator<<(std::ostream &os, const Queue<T> &queue)
    {
        if (queue.is_empty())
        {
            os << "[]";
            return os;
        }
        os << "[";
        for (unsigned int i = 0; i < queue.size(); i++)
        {
            unsigned int index = (queue.front_index + i) % queue.get_capacity();
            os << queue.array[index];
            if (i + 1 != queue.m_size)
            {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }
    ~Queue()
    {
        clear();
    }
};
