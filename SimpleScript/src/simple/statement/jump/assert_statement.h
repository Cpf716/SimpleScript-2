//
//  assert_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 6/3/23.
//

#ifndef assert_statement_h
#define assert_statement_h

#include "statement_t.h"

namespace simple {
    class assert_statement: public statement_t {
        //  MEMBER FIELDS
        
        string expression;
    public:
        //  CONSTRUCTORS
        
        assert_statement(const string expression) {
            if (expression.empty())
                expect("expression");
            
            this -> expression = expression;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        bool compare(const string val) const { return false; }
        
        string evaluate(interpreter* ss) {
            unsupported_error("evaluate()");
            return EMPTY;
        }
        
        string execute(interpreter* ss) {
            if (!simple::evaluate(ss -> evaluate(expression)))
                throw error("Assertion failed: (" + expression + ")");
                
            return EMPTY;
        }
        
        void set_break() { unsupported_error("set_break()"); }
        
        void set_continue() { unsupported_error("set_continue()"); }
        
        void set_parent(statement_t* parent) { }
        
        void set_return(const string result) { unsupported_error("set_return()"); }
        
        bool validate(interpreter* ss) const { return false; }
    };
}

#endif /* assert_statement_h */
