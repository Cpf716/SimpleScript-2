//
//  file_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 10/14/23.
//

#include "file_statement.h"

namespace ss {
    //  CONSTRUCTORS

    file_statement::file_statement(const size_t statementc, statement_t** statementv) {
        this->statementc = statementc;
        this->statementv = statementv;
        
        if (statementc && is_clause(statementv[statementc - 1]))
            expect_error("expression");
        
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->set_parent(this);
    }

    void file_statement::close() {
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool file_statement::analyze(interpreter* ssu) const {
        if (!statementc)
            return false;
        
        size_t i = 0;
        while (i < statementc - 1 && !statementv[i]->analyze(ssu))
            ++i;
        
        if (i != statementc - 1)
            logger_write("Unreachable code\n");
        
        statementv[statementc - 1]->analyze(ssu);
        
        return false;
    }

    bool file_statement::compare(const string value) const {
        unsupported_error("compare()");
        return false;
    }

    string file_statement::evaluate(interpreter* ssu) {
        unsupported_error("evaluate()");
        return EMPTY;
    }

    string file_statement::execute(interpreter* ssu) {
        analyze(ssu);
        
        result = encode("undefined");
        should_return = false;
        
        for (size_t i = 0; i < statementc; ++i) {
            statementv[i]->execute(ssu);
            
            if (should_return)
                break;
        }
        
        return result;
    }

    void file_statement::set_break() {
        throw error("break cannot be used outside of a loop");
    }

    void file_statement::set_continue() {
        throw error("continue cannot be used outside of a loop");
    }

    void file_statement::set_parent(statement_t* parent) {
        unsupported_error("set_parent()");
    }

    void file_statement::set_return(const string result) {
        this->result = result;
        this->should_return = true;
    }
}
