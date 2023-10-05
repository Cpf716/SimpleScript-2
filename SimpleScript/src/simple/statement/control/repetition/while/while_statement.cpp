//
//  while_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/12/23.
//

#include "while_statement.h"

namespace ss {
    //  CONSTRUCTORS

    while_statement::while_statement(const string expression, const size_t statementc, statement_t** statementv) {
        if (expression.empty())
            expect_error("expression");
        
        this->expression = expression;
        
        if (statementc && is_clause(statementv[statementc - 1]))
            expect_error("expression");
        
        this->statementc = statementc;
        this->statementv = statementv;
    }

    void while_statement::close() {
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool while_statement::analyze(interpreter* ssu) const {
        if (!statementc) {
            cout << "'while' statement has empty body\n";
            
            return false;
        }
        
        size_t i = 0;
        while (i < statementc - 1 && !statementv[i]->analyze(ssu))
            ++i;
        
        if (i != statementc - 1)
            cout << "Unreachable code\n";
                
        if (statementv[i]->analyze(ssu) &&
            (statementv[i]->compare("break") ||
             statementv[i]->compare("return")))
            cout << "'while' statement will execute at most once\n";
        
        return false;
    }

    string while_statement::evaluate(interpreter* ssu) {
        unsupported_error("evaluate()");
        return EMPTY;
    }

    string while_statement::execute(interpreter* ssu) {
        should_break = false;
        
        while (1) {
            const string buid = ssu->backup();
            
            if (!ss::evaluate(ssu->evaluate(expression))) {
                ssu->restore(buid);
                break;
            }
            
            should_continue = false;
            
            for (size_t i = 0; i < statementc; ++i) {
                statementv[i]->execute(ssu);
                
                if (should_break || should_continue)
                    break;
            }
            
            ssu->restore(buid);
            
            if (should_break)
                break;
        }
        
        return EMPTY;
    }

    bool while_statement::compare(const string val) const { return false; }

    void while_statement::set_break() { should_break = true; }

    void while_statement::set_continue() { should_continue = true; }

    void while_statement::set_return(const string result) {
        should_break = true;
        parent->set_return(result);
    }
}
