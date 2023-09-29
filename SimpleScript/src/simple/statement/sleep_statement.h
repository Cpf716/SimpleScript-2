//
//  sleep_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 9/28/23.
//

#ifndef sleep_statement_h
#define sleep_statement_h

#include "statement_t.h"

namespace simple {
    class sleep_statement: public statement_t {
        string expression;
        
    public:
        //  CONSTRUCTORS
        
        sleep_statement(const string expression) {
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
            string res = ss->evaluate(expression);
            
            if (simple::is_array(res))
                type_error("array", "int");
            
            if (is_string(res))
                type_error("string", "int");
            
            double num = stod(res);
            
            if (!is_int(num))
                type_error("double", "int");
            
            if (num < 0)
                range_error(rtrim(num));
            
            this_thread::sleep_for(milliseconds((long)num));
            
            return EMPTY;
        }
        
        void set_break() { unsupported_error("set_break()"); }
        
        void set_continue() { unsupported_error("set_continue()"); }
        
        void set_parent(statement_t* parent) { }
        
        void set_return(const string result) { unsupported_error("set_return()"); }
        
        bool validate(interpreter* ss) const { return false; }
    };
}

#endif /* sleep_statement_h */
