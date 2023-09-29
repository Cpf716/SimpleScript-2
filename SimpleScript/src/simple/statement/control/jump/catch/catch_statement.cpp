//
//  catch_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/17/23.
//

#include "catch_statement.h"

namespace simple {
    //  CONSTRUCTORS

    catch_statement::catch_statement(const string symbol, const size_t statementc, statement_t** statementv) {
        if (!is_symbol(symbol))
            expect("symbol");
        
        this -> symbol = symbol;
        
        if (statementc && (statementv[statementc - 1] -> compare("else") ||
            statementv[statementc - 1] -> compare("elseif")))
            expect("expression");
        
        this -> statementc = statementc;
        this -> statementv = statementv;
    }

    void catch_statement::close() {
        for (size_t i = 0; i < statementc; ++i)
            statementv[i] -> close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool catch_statement::compare(const string val) const { return val == "catch"; }

    string catch_statement::evaluate(interpreter* ss) { return encode(symbol); }

    string catch_statement::execute(interpreter* ss) {
        should_break = false;
        
        for (size_t i = 0; i < statementc; ++i) {
            statementv[i] -> execute(ss);
            
            if (should_break)
                break;
        }
        
        return EMPTY;
    }

    void catch_statement::set_break() {
        should_break = true;
        parent -> set_break();
    }

    void catch_statement::set_continue() {
        should_break = true;
        parent -> set_continue();
    }

    void catch_statement::set_return(const string result) {
        should_break = true;
        parent -> set_return(result);
    }

    bool catch_statement::validate(interpreter* ss) const {
        if (!statementc) return false;
        
        size_t i = 0;
        while (i < statementc - 1 && !statementv[i] -> validate(ss))
            ++i;
        
        if (i != statementc - 1)
            cout << "Unreachable code\n";
        
        statementv[statementc - 1] -> validate(ss);
        
        return false;
    }
}
