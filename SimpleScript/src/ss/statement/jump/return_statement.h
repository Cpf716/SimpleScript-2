//
//  return_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/20/23.
//

#ifndef return_statement_h
#define return_statement_h

#include "statement_t.h"

namespace ss {
    class return_statement: public statement_t {
        //  MEMBER FIELDS
        
        string expression;
        
        statement_t* parent = NULL;
    public:
        //  CONSTRUCTORS
        
        return_statement(string result) {
            expression = trim(result).empty() ? encode("undefined") : result;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        bool analyze(interpreter* ssu) const { return true; }
        
        bool compare(const string val) const { return val == "return"; }
        
        string evaluate(interpreter* ssu) {
            unsupported_error("evaluate()");
            return EMPTY;
        }
        
        string execute(interpreter* ssu) {
            set_return(expression.empty() ? EMPTY : ssu->evaluate(expression));
            return EMPTY;
        }
        
        void set_break() { unsupported_error("set_break()"); }
        
        void set_continue() { unsupported_error("set_continue()"); }
        
        void set_parent(statement_t* parent) { this->parent = parent; }
        
        void set_return(const string result) { parent->set_return(result); }
    };
}

#endif /* return_statement_h */
