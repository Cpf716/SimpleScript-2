//  Author: Corey Ferguson
//  Date:   September 28, 2023
//  File:   operator_t.h
//

#ifndef operator_t_h
#define operator_t_h

#include <iostream>

using namespace std;

namespace ss {
    class operator_t {
        //  MEMBER FIELDS
        
        string opc;
    protected:
        //  MEMBER FUNCTIONS
        
        void set_opcode(const string opc) { this->opc = opc; }
    public:
        //  CONSTRUCTORS
        
        virtual void close() = 0;
        
        //  MEMBER FUNCTIONS
        
        string opcode() const { return opc; }
    };
}

#endif
