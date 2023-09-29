//
//  statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 4/18/23.
//

#ifndef statement_h
#define statement_h

#include "statement_t.h"

namespace simple {
    class statement: public statement_t {
        //  MEMBER FIELDS
        
        string expression;
    public:
        //  CONSTRUCTORS
        
        statement(const string expression) {
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
            ss -> evaluate(expression);
             
            return EMPTY;
        }
        
        void set_break() { unsupported_error("set_break()"); }
        
        void set_continue() { unsupported_error("set_continue()"); }
        
        void set_parent(statement_t* parent) { }
        
        void set_return(const string result) { unsupported_error("set_return()"); }
        
        bool validate(interpreter* ss) const {
            if (!ss -> is_mutating(expression))
                cout << "Expression result unused: (" << expression << ")\n";
            
            return false;
        }
    };
}

#endif /* statement_h */
