//
//  node.h
//  SimpleScript
//
//  Created by Corey Ferguson on 7/25/23.
//

#ifndef node_h
#define node_h

#include "array.h"

namespace ss {
    template <typename T>
    class node {
        T _data;
        node* _parent = NULL;
        ss::array<node*> children;
    public:
        node(const T data, node* parent) {
            _data = data;
            _parent = parent;
            
            if (parent != NULL)
                parent->children.push(this);
        }
        
        T data() const { return _data; }
        
        node* parent() const { return _parent; }
        
        void close() {
            for (size_t i = 0; i < children.size(); ++i)
                children[i]->close();
            
            delete this;
        }
    };
}

#endif /* node_h */
