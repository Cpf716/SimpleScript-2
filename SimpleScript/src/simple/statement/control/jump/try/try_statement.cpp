//
//  try_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/17/23.
//

#include "try_statement.h"

namespace simple {
    //  CONSTRUCTORS

    try_statement::try_statement(const size_t statementc, statement_t** statementv) {
        index = 0;
        while (index < statementc && !statementv[index] -> compare("catch"))
            ++index;
        
        if (index == statementc)
            expect("'catch'");
        
        if (index == statementc - 1) {
            if (statementc == 1)
                cout << "'try' statement has empty body\n";
            
        } else if (statementc == 2)
            cout << "'try' statement has empty body\n";
        
        this -> statementc = statementc;
        this -> statementv = statementv;
    }

    void try_statement::close() {
        for (size_t i = 0; i < statementc; ++i)
            statementv[i] -> close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool try_statement::compare(const string val) const { return false; }

    string try_statement::evaluate(interpreter* ss) {
        unsupported_error("evaluate()");
        return EMPTY;
    }

    string try_statement::execute(interpreter* ss) {
        try {
            should_break = false;
            
            string buid = ss -> backup();
            
            for (size_t i = 0; i < index; ++i) {
                statementv[i] -> execute(ss);
                
                if (should_break)
                    break;
            }
            
            ss -> restore(buid);
            
        } catch (exception& e) {
            size_t i = statementc - 1;
            if (statementv[i] -> compare("finally"))
                --i;
            
            string buid = ss -> backup();
            string symbol = decode(statementv[i] -> evaluate(ss));
            
            if (ss -> is_defined(symbol))
                ss -> drop(symbol);
            
            ss -> set_string(symbol, encode(e.what()));
            
            statementv[i] -> execute(ss);
            
            ss -> drop(symbol);
            
            string symbolv[1];
            
            symbolv[0] = symbol;
            
            ss -> restore(buid, true, 1, symbolv);
            
            if (i != statementc - 1)
                statementv[statementc - 1] -> execute(ss);
        }
        
        return EMPTY;
    }

    void try_statement::set_break() {
        should_break = true;
        parent -> set_break();
    }

    void try_statement::set_continue() {
        should_break = true;
        parent -> set_continue();
    }

    void try_statement::set_return(const string result) {
        should_break = true;
        parent -> set_return(result);
    }

    bool try_statement::validate(interpreter* ss) const {
        if (index) {
            size_t i = 0;
            while (i < index - 1 && !statementv[i] -> validate(ss))
                ++i;
            
            if (i != index - 1)
                cout << "Unreachable code\n";
            
            statementv[index - 1] -> validate(ss);
        }
        
        for (size_t i = index; i < statementc; ++i)
            statementv[i] -> validate(ss);
        
        return false;
    }
}
