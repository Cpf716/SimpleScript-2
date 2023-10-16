//
//  tuo.h
//  SimpleScript
//
//  Created by Corey Ferguson on 6/1/23.
//

#ifndef tuo_h
#define tuo_h

#include "operator_t.h"

namespace ss {
    class tuo: public operator_t {
        //  MEMBER FIELDS
        
        std::function<string(const string, const string, const string)> opr;
    public:
        //  CONSTRUCTORS
        
        tuo(const string opc, const std::function<string(const string, const string, const string)> opr) {
            set_opcode(opc);
            
            this->opr = opr;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        string apply(const string lhs, const string ctr, const string rhs) const {
            return opr(lhs, ctr, rhs);
        }
    };
}

#endif /* tuo_h */
