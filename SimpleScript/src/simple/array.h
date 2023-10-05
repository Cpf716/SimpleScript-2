//
//  array.h
//  SimpleScript
//
//  Created by Corey Ferguson on 1/31/23.
//

#ifndef array_h
#define array_h

#include <iostream>

namespace ss {
    template <typename T>
    class array {
        //  MEMBER FIELDS
        
        std::size_t _capacity, _size = 0;
        T* _data = NULL;
    public:
        //  CONSTRUCTORS
        
        array(const ss::array<T>& data) {
            _capacity = data.capacity();
            _data = new T[capacity()];
            
            for (std::size_t i = 0; i < data.size(); ++i)
                _data[i] = data._data[i];
            
            _size = data.size();
        }
        
        array() { _data = new T[_capacity = 1]; }
        
        array(const std::size_t capacity) { _data = new T[_capacity = capacity]; }
        
        ~array() { delete[] _data; }
        
        //  OPERATORS
        
        T& operator[](const std::size_t index) {
            if (index >= size())
                throw std::out_of_range(std::to_string(index));
            
            return _data[index];
        }
        
        //  MEMBER FUNCTIONS
        
        std::size_t capacity() const { return _capacity; };
        
        void clear() {
            delete[] _data;
            
            _data = new T[_capacity = 1];
            _size = 0;
        }
        
        void ensure_capacity(const std::size_t new_capacity) {
            if (capacity() >= new_capacity)
                return;
            
            T* tmp = new T[_capacity = new_capacity];
            
            for (std::size_t i = 0; i < size(); ++i)
                tmp[i] = _data[i];
            
            delete[] _data;
            
            _data = tmp;
        }
        
        int index_of(const T item) const {
            int i = 0;
            while (i < size() && _data[i] != item)
                ++i;
            
            return i == size() ? -1 : i;
        }
        
        void insert(const std::size_t index, const T item) {
            if (index > size())
                throw std::out_of_range(std::to_string(index));
            
            if (size() == capacity())
                ensure_capacity(capacity() ? capacity() * 2 : 1);
            
            _data[_size] = item;
            
            for (std::size_t i = _size++; i > index; --i)
                std::swap(_data[i], _data[i - 1]);
        }
        
        int last_index_of(const T item) const {
            int i = (int)size() - 1;
            while (i >= 0 && _data[i] != item)
                --i;
            
            return i;
        }
        
        void push(const T item) {
            if (size() == capacity())
                ensure_capacity(capacity() ? capacity() * 2 : 1);
            
            _data[_size++] = item;
        }
        
        void remove(const size_t index) {
            if (index >= size())
                throw std::out_of_range(std::to_string(index));
            
            for (std::size_t i = index; i < size() - 1; ++i)
                std::swap(_data[i], _data[i + 1]);
            
            --_size;
        }
        
        void resize(const std::size_t new_size) {
            ensure_capacity(new_size);
            
            while (size() < new_size)
                push("");
            
            while (size() > new_size)
                remove(new_size);
        }
        
        void shrink_to_fit() {
            T* tmp = new T[size()];
            
            for (std::size_t i = 0; i < size(); ++i)
                tmp[i] = _data[i];
            
            delete[] _data;
            
            _data = tmp;
            _capacity = size();
        }
        
        std::size_t size() const { return _size; };
    };
}

#endif /* array_h */
