//
//  for_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/12/23.
//

#include "for_statement.h"

namespace simple {
    //  CONSTRUCTORS

    for_statement::for_statement(const string specificer, const size_t statementc, statement_t** statementv) {
        string tokenv[specificer.length() * 2 + 1];
        size_t tokenc = tokens(tokenv, specificer);
        
        size_t e = 0, s = e;   int p = 0;
        for (; e < tokenc; ++e) {
            if (tokenv[e] == "(")
                ++p;
            else if (tokenv[e] == ")")
                --p;
            else if (!p && tokenv[e] == ",") {
                for (size_t i = e; i > s + 1; --i) {
                    tokenv[s] += " " + tokenv[s + 1];
                    
                    for (size_t j = s + 1; j < tokenc - 1; ++j)
                        swap(tokenv[j], tokenv[j + 1]);
                    
                    --tokenc;    --e;
                }
                
                s = e + 1;
            }
        }
        
        for (size_t i = e; i > s + 1; --i) {
            tokenv[s] += " " + tokenv[s + 1];
            
            for (size_t j = s + 1; j < tokenc - 1; ++j)
                swap(tokenv[j], tokenv[j + 1]);
            
            --tokenc;
        }
        
        if (tokenc == 1) {
            tokenc = tokens(tokenv, tokenv[0]);
            
            if (tokenc < 3 || !is_symbol(tokenv[0]) || tolower(tokenv[1]) != "in")
                expect("',' in 'for' statement specifier");
            
            expressionv = new string[expressionc = 2];
            
            expressionv[0] = tokenv[0];
            expressionv[1] = tokenv[2];
            
            for (size_t i = 3; i < tokenc; ++i)
                expressionv[1] += " " + tokenv[i];
        } else {
            for (size_t i = 0, n = (size_t)floor(tokenc / 2) + 1; i < n; ++i) {
                if (tokenv[i * 2] == ",") {
                    tokenv[tokenc] = EMPTY;
                    
                    for (size_t j = tokenc; j > i * 2; --j)
                        swap(tokenv[j], tokenv[j - 1]);
                    
                    ++tokenc;
                }
            }
            
            if (tokenv[tokenc - 1] == ",")
                tokenv[tokenc++] = EMPTY;
            
            if (tokenc < 5)
                expect("',' in 'for' statement specifier");
            
            if (tokenc > 5)
                expect("';' after expression");
            
            expressionv = new string[expressionc = 3];
            
            for (size_t i = 0; i < 3; ++i)
                expressionv[i] = tokenv[i * 2];
        }
        
        if (statementc && is_clause(statementv[statementc - 1]))
            expect("expression");
        
        this -> statementc = statementc;
        this -> statementv = statementv;
    }

    void for_statement::close() {
        delete[] expressionv;
        
        for (size_t i = 0; i < statementc; ++i)
            statementv[i] -> close();
        
        delete[] statementv;
        
        if (valuev != NULL)
            delete[] valuev;
        
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool for_statement::compare(const string val) const { return false; }

    string for_statement::evaluate(interpreter* ss) {
        unsupported_error("evaluate()");
        
        return EMPTY;
    }

    string for_statement::execute(interpreter* ss) {
        should_break = false;
        
        string buid = ss -> backup();
        size_t valuec = 0;
        
        if (expressionc == 2) {
            if (ss -> is_defined(expressionv[0]))
                ss -> drop(expressionv[0]);
            
            string result = ss -> evaluate(expressionv[1]);
            
            valuev = new string[result.length() + 1];
            valuec = parse(valuev, result);
        } else {
            string tokenv[expressionv[0].length() + 1];
            size_t tokenc = tokens(tokenv, expressionv[0]);
            
            size_t i = 0;
            while (i < tokenc && tokenv[i] == "(")
                ++i;
            
            if (i < tokenc && tolower(tokenv[i]) == "const")
                ++i;
            
            if (i < tokenc && tolower(tokenv[i]) == "array")
                ++i;
            
            if (i < tokenc - 1 && is_symbol(tokenv[i]) && tokenv[i + 1] == "=" && ss -> is_defined(tokenv[i])) {
                valuev = new string[valuec = 1];
                valuev[0] = tokenv[i];
                
                ss -> drop(valuev[0]);
            }
            
            //  available to every iteration
            ss -> evaluate(expressionv[0]);
        }
        
        size_t index = 0;
        while (1) {
            string _buid = ss -> backup();
            
            if (expressionc == 2) {
                if (index == valuec) {
                    ss -> restore(_buid, true, 1, expressionv);
                    
                    break;
                }
                
                if (valuev[index].empty() || is_string(valuev[index]))
                    ss -> set_string(expressionv[0], valuev[index]);
                else
                    ss -> set_number(expressionv[0], stod(valuev[index]));
                
                ++index;
                
            } else if (!expressionv[1].empty() && !simple::evaluate(ss -> evaluate(expressionv[1]))) {
                //  available for one iteration
                ss -> restore(_buid);
                
                break;
            }
            
            should_continue = false;
            
            for (size_t j = 0; j < statementc; ++j) {
                statementv[j] -> execute(ss);
                
                if (should_break || should_continue)
                    break;
            }
            
            if (should_break) {
                if (expressionc == 2)
                    ss -> restore(_buid, true, 1, expressionv);
                else
                    ss -> restore(_buid);
                
                break;
            }
            
            if (expressionc == 2)
                ss -> restore(_buid, true, 1, expressionv);
            else {
                //  available once
                ss -> evaluate(expressionv[2]);
                ss -> restore(_buid);
            }
        }
        
        if (expressionc == 2 || !valuec)
            ss -> restore(buid);
        else
            ss -> restore(buid, true, 1, valuev);
        
        if (valuev != NULL) {
            delete[] valuev;
            
            valuev = NULL;
        }
        
        return EMPTY;
    }

    void for_statement::set_break() { should_break = true; }

    void for_statement::set_continue() { should_continue = true; }

    void for_statement::set_return(const string result) {
        should_break = true;
        parent -> set_return(result);
    }

    bool for_statement::validate(interpreter* ss) const {
        if (!statementc) {
            cout << "'for' statement has empty body\n";
            
            return false;
        }
        
        size_t i = 0;
        while (i < statementc - 1 && !statementv[i] -> validate(ss))
            ++i;
        
        if (i != statementc - 1)
            cout << "Unreachable code\n";
                
        if (statementv[i] -> validate(ss) &&
            (statementv[i] -> compare("break") ||
             statementv[i] -> compare("return")))
            cout << "'for' statement will execute at most once\n";
        
        return false;
    }
}
