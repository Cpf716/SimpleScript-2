//
//  uuo.h
//  SimpleScript
//
//  Created by Corey Ferguson on 11/8/22.
//

#ifndef uuo_h
#define uuo_h

#include "operator_t.h"

using namespace std;

namespace simple {
    class uuo: public operator_t {
        //  MEMBER FIELDS
        
        std::function<string(const string)> operation;
    public:
        //  CONSTRUCTORS
        
        uuo(const string opcode, const std::function<string(const string)> operation) {
            set_opcode(opcode);
            this -> operation = operation;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        string apply(const string rhs) const {
            return operation(rhs);
        }
    };
}

#endif /* uuo_h */
