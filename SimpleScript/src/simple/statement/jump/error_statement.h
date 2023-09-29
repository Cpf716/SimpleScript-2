//
//  error_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/26/23.
//

#ifndef error_statement_h
#define error_statement_h

#include "statement_t.h"

namespace simple {
    class error_statement: public statement_t {
        //  MEMBER FIELDS
        
        string message;
    public:
        //  CONSTRUCTORS
        
        error_statement(const string message) {
            if (message.empty())
                expect("expression");
            
            this -> message = message;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        bool compare(const string val) const { return false; }
        
        string evaluate(interpreter* ss) {
            unsupported_error("evaluate()");
            return EMPTY;
        }
        
        string execute(interpreter* ss) {
            throw error(decode(ss -> evaluate(message)));
            
            return EMPTY;
        }
        
        void set_break() { unsupported_error("set_break()"); }
        
        void set_continue() { unsupported_error("set_continue()"); }
        
        void set_parent(statement_t* parent) { }
        
        void set_return(const string result) { unsupported_error("set_return()"); }
        
        bool validate(interpreter* ss) const { return true; }
    };
}

#endif /* error_statement_h */
