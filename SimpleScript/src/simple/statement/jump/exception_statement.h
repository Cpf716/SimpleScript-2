//
//  exception_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/26/23.
//

#ifndef exception_statement_h
#define exception_statement_h

#include "exception.h"
#include "statement_t.h"

namespace simple {
    class exception_statement: public statement_t {
        //  MEMBER FIELDS
        
        string message;
    public:
        //  CONSTRUCTORS
        
        exception_statement(const string message) {
            if (message.empty())
                expect("expression");
            
            this->message = message;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        bool compare(const string val) const { return false; }
        
        string evaluate(interpreter* ss) {
            unsupported_error("evaluate()");
            
            return EMPTY;
        }
        
        string execute(interpreter* ss) {
            throw exception(decode(ss -> evaluate(message)));
            
            return EMPTY;
        }
        
        void set_break() { unsupported_error("set_break()"); }
        
        void set_continue() { unsupported_error("set_continue()"); }
        
        void set_parent(statement_t* parent) { }
        
        void set_return(const string result) { unsupported_error("set_return()"); }
        
        bool validate(interpreter* ss) const { return true; }
    };
}

#endif /* exception_statement_h */
