//
//  continue_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/20/23.
//

#ifndef continue_statement_h
#define continue_statement_h

#include "statement_t.h"

namespace simple {
    class continue_statement: public statement_t {
        //  MEMBER FIELDS
        
        statement_t* parent = NULL;
    public:
        //  CONSTRUCTORS
        
        continue_statement() { }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        bool compare(const string val) const { return false; }
        
        string evaluate(interpreter* ss) {
            unsupported_error("evaluate()");
            return EMPTY;
        }
        
        string execute(interpreter* ss) {
            set_continue();
            return EMPTY;
        }
        
        void set_break() { unsupported_error("set_break()"); }
        
        void set_continue() { parent -> set_continue(); }
        
        void set_parent(statement_t* parent) { this -> parent = parent; }
        
        void set_return(const string result) { unsupported_error("set_return()"); }
        
        bool validate(interpreter* ss) const { return true; }
    };
}

#endif /* continue_statement_h */
