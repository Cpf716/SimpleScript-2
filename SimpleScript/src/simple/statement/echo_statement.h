//
//  echo_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/18/23.
//

#ifndef echo_statement_h
#define echo_statement_h

#include "statement_t.h"

namespace simple {
    class echo_statement: public statement_t {
        //  MEMBER FIELDS
        
        string expression;
    public:
        //  CONSTRUCTORS
        
        echo_statement(const string expression) {
            if (expression.empty())
                expect("expression");
            
            this->expression = expression;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        bool compare(const string val) const { return false; }
        
        string evaluate(interpreter* ss) {
            unsupported_error("evaluate()");
            return EMPTY;
        }
        
        string execute(interpreter* ss) {
            string result = ss->evaluate(expression);
            
            if (simple::is_array(result))
                type_error("array", "string");
            
            cout << (result.empty() ? "null" : decode(result));
            
            return EMPTY;
        }
        
        void set_break() { unsupported_error("set_break()"); }
        
        void set_continue() { unsupported_error("set_continue()"); }
        
        void set_parent(statement_t* parent) { }
        
        void set_return(const string result) { unsupported_error("set_return()"); }
        
        bool validate(interpreter* ss) const { return false; }
    };
}

#endif /* echo_statement_h */
