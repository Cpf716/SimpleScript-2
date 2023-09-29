//
//  consume_statement.h
//  SimpleScript
//
//  Created by Corey Ferguson on 6/5/23.
//

#ifndef consume_statement_h
#define consume_statement_h

#include "statement_t.h"

namespace simple {
    class consume_statement: public statement_t {
        //  MEMBER FIELDS
        
        string symbol;
    public:
        //  CONSTRUCTORS
        
        consume_statement(const string symbol) {
            if (!is_symbol(symbol))
                expect("symbol");
            
            this -> symbol = symbol;
        }
        
        void close() { delete this; }
        
        //  MEMBER FUNCTIONS
        
        bool compare(const string val) const { return false; }
        
        string evaluate(interpreter* ss) {
            unsupported_error("evaluate()");
            return EMPTY;
        }
        
        string execute(interpreter* ss) {
            ss -> consume(symbol);
            
            return EMPTY;
        }
        
        void set_break() { unsupported_error("set_break()"); }
        
        void set_continue() { unsupported_error("set_continue()"); }
        
        void set_parent(statement_t* parent) { }
        
        void set_return(const string result) { unsupported_error("set_return()"); }
        
        bool validate(interpreter* ss) const { return false; }
    };
}

#endif /* consume_statement_h */
