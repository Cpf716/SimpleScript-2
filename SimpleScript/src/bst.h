//  Author: Corey Ferguson
//  Date:   September 22, 2023
//  File:   bst.h
//

#include <iostream>

template <typename T>
class bst {
    //  MEMBER FIELDS
    
    T _data;
public:
    //  MEMBER FIELDS
    
    bst* left = NULL;
    bst* right = NULL;
    bst* parent = NULL;
    
    //  CONSTRUCTORS
    
    bst(T data) { _data = data; }
    
    //  MEMBER FUNCTIONS
    
    T data() const { return _data; }
    
    void close() {
        if (left != NULL)
            left -> close();
        
        if (right != NULL)
            right -> close();
        
        delete this;
    }
};

//  NON-MEMBER FUNCTIONS

template <typename T>
bst<std::pair<T, int>>* build(T* data, int s, int e, bst<std::pair<T, int>>* parent = NULL) {
    int l = floor((e - s) / 2);
    
    bst<std::pair<T, int>>* _parent = new bst<std::pair<T, int>>(std::pair<T, int>(data[s + l], s + l));
    
    _parent -> parent = parent;
    
    if (l != 0)
        _parent -> left = build(data, s, s + l, _parent);
    
    if (s + l != e - 1)
        _parent -> right = build(data, s + l + 1, e, _parent);
    
    return _parent;
}

template <typename T>
int index_of(bst<std::pair<T, int>>* cursor, const T value) {
    if (cursor -> data().first == value)
        return cursor -> data().second;
    
    if (cursor -> data().first > value) {
        if (cursor -> left == NULL)
            return -1;
        
        return index_of(cursor -> left, value);
    }
        
    if (cursor -> data().first < value) {
        if (cursor -> right == NULL)
            return -1;
        
        return index_of(cursor -> right, value);
    }
    
    return -1;
}
