//
//  else_if_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/17/23.
//

#include "else_if_statement.h"

namespace simple {
    //  CONSTRUCTORS

    else_if_statement::else_if_statement(const string expression, const size_t statementc, statement_t** statementv) {
        if (expression.empty())
            expect("expression");
        
        this -> expression = expression;
        
        if (statementc && (statementv[statementc - 1] -> compare("catch") ||
            statementv[statementc - 1] -> compare("finally")))
            expect("expression");
        
        this -> statementc = statementc;
        this -> statementv = statementv;
    }

    void else_if_statement::close() {
        for (size_t i = 0; i < statementc; ++i)
            statementv[i] -> close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool else_if_statement::compare(const string val) const { return val == "elseif"; }

    string else_if_statement::evaluate(interpreter* ss) { return ss -> evaluate(expression); }

    string else_if_statement::execute(interpreter* ss) {
        should_break = false;
        
        for (size_t i = 0; i < statementc; ++i) {
            statementv[i] -> execute(ss);
            
            if (should_break)
                break;
        }
            
        return EMPTY;
    }

    void else_if_statement::set_break() {
        should_break = true;
        parent -> set_break();
    }

    void else_if_statement::set_continue() {
        should_break = true;
        parent -> set_continue();
    }

    void else_if_statement::set_return(const string result) {
        should_break = true;
        parent -> set_return(result);
    }

    bool else_if_statement::validate(interpreter* ss) const {
        if (!statementc) {
            cout << "'else if' statement has empty body\n";
            
            return false;
        }
        
        size_t i = 0;
        while (i < statementc - 1 && !statementv[i] -> validate(ss))
            ++i;
        
        if (i != statementc - 1)
            cout << "Unreachable code\n";
        
        statementv[statementc - 1] -> validate(ss);
        
        return false;
    }
}
