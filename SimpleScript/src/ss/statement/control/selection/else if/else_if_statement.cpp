//
//  else_if_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/17/23.
//

#include "else_if_statement.h"

namespace ss {
    //  CONSTRUCTORS

    else_if_statement::else_if_statement(const string expression, const size_t statementc, statement_t** statementv) {
        if (expression.empty())
            expect_error("expression");
        
        this->expression = expression;
        
        if (statementc && (statementv[statementc - 1]->compare("catch") ||
            statementv[statementc - 1]->compare("finally")))
            expect_error("expression");
        
        this->statementc = statementc;
        this->statementv = statementv;
    }

    void else_if_statement::close() {
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool else_if_statement::analyze(interpreter* ssu) const {
        if (!statementc) {
            logger_write("'else if' statement has empty body\n");
            
            return false;
        }
        
        size_t i = 0;
        while (i < statementc - 1 && !statementv[i]->analyze(ssu))
            ++i;
        
        if (i != statementc - 1)
            logger_write("Unreachable code\n");
        
        statementv[statementc - 1]->analyze(ssu);
        
        return false;
    }

    bool else_if_statement::compare(const string val) const { return val == "elseif"; }

    string else_if_statement::evaluate(interpreter* ssu) { return ssu->evaluate(expression); }

    string else_if_statement::execute(interpreter* ssu) {
        should_break = false;
        
        for (size_t i = 0; i < statementc; ++i) {
            statementv[i]->execute(ssu);
            
            if (should_break)
                break;
        }
            
        return EMPTY;
    }

    void else_if_statement::set_break() {
        should_break = true;
        parent->set_break();
    }

    void else_if_statement::set_continue() {
        should_break = true;
        parent->set_continue();
    }

    void else_if_statement::set_return(const string result) {
        should_break = true;
        parent->set_return(result);
    }
}
