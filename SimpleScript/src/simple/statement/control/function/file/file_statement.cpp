//
//  file_statement.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 6/11/23.
//

#include "file_statement.h"

namespace ss {
    //  CONSTRUCTORS

    file_statement::file_statement(const size_t n, string* src, const size_t functionc, function_t** functionv) {
        this->functionc = functionc;
        this->functionv = functionv;
        
        statementv = new statement_t*[n];
        statementc = build(statementv, src, 0, n);
        
        if (statementc && is_clause(statementv[statementc - 1]))
            expect_error("expression");
        
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->set_parent(this);
    }

    void file_statement::close() {
        delete[] functionv;
        
        for (size_t i = 0; i < statementc; ++i)
            statementv[i]->close();
        
        delete[] statementv;
        delete this;
    }

    //  MEMBER FUNCTIONS

    bool file_statement::analyze(interpreter* ssu) const {
        if (!statementc) return false;
        
        size_t i = 0;
        while (i < statementc - 1 && !statementv[i]->analyze(ssu))
            ++i;
        
        if (i != statementc - 1)
            cout << "Unreachable code\n";
        
        statementv[statementc - 1]->analyze(ssu);
        
        return false;
    }

    size_t file_statement::build(statement_t** dst, string* src, size_t si, size_t ei) {
        size_t i = si, s = 0;
        while (i < ei) {
            //  cout << src[i] << endl;
            
            string* tokenv = new string[src[i].length() + 1];
            size_t tokenc = tokens(tokenv, src[i]);
            
            if (tokenv[0] == "include") {
                delete[] tokenv;
                expect_error("expression");
            }
            
            if (tokenv[0] == "catch") {
                delete[] tokenv;
                
                if (tokenc > 2)
                    expect_error("';' after expression");
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "try") {
                        delete[] tokenv;
                        ++p;
                    } else {
                        if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "try") {
                            delete[] tokenv;
                            
                            --p;
                            
                            if (!p)
                                break;
                        } else if (p == 1) {
                            if (tokenv[0] == "catch") {
                                delete[] tokenv;
                                expect_error("'end try'");
                            }
                            
                            if (tokenv[0] == "finally") {
                                delete[] tokenv;
                                break;
                            }
                            
                            delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                }
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new catch_statement(ltrim(src[i].substr(5)), _s, _dst);
                i = k;
            } else if (tokenc > 1 && tokenv[0] == "do" && tokenv[1] == "while") {
                delete[] tokenv;
                
                size_t j;
                for (j = 0; j <= src[i].length() - 5; ++j) {
                    size_t k = 0;
                    while (k < 5 && src[i][j + k] == ("while")[k])
                        ++k;
                    
                    if (k == 5)
                        break;
                }
                
                j += 5;
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "while") {
                        delete[] tokenv;
                        ++p;
                    } else {
                        if (tokenc > 1) {
                            if (tokenv[0] == "do" && tokenv[1] == "while") {
                                delete[] tokenv;
                                ++p;
                            } else if (tokenv[0] == "end" && tokenv[1] == "while") {
                                delete[] tokenv;
                                
                                if (tokenc > 2)
                                    expect_error("';' after expression");
                                
                                --p;
                                
                                if (!p) break;
                            } else
                                delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                }
                
                if (k == ei) expect_error("'end while'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new do_while_statement(ltrim(src[i].substr(j)), _s, _dst);
                i = k + 1;
            } else if (tokenc > 1 && tokenv[0] == "else" && tokenv[1] == "if") {
                delete[] tokenv;
                
                size_t j;
                for (j = 0; j <= src[i].length() - 2; ++j) {
                    size_t k = 0;
                    while (k < 2 && src[i][j + k] == ("if")[k])
                        ++k;
                    
                    if (k == 2)
                        break;
                }
                
                j += 2;
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "if") {
                        delete[] tokenv;
                        ++p;
                    } else {
                        if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "if") {
                            delete[] tokenv;
                            
                            --p;
                            
                            if (!p)
                                break;
                        } else if (p == 1) {
                            if (tokenv[0] == "else") {
                                delete[] tokenv;
                                break;
                            } else
                                delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                    
                }
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new else_if_statement(ltrim(src[i].substr(j)), _s, _dst);
                i = k;
            } else if (tokenv[0] == "else") {
                delete[] tokenv;
                
                if (tokenc > 1)
                    expect_error("';' after expression");
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "if") {
                        delete[] tokenv;
                        ++p;
                    } else {
                        if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "if") {
                            delete[] tokenv;
                            
                            --p;
                            
                            if (!p)
                                break;
                        } else if (p == 1) {
                            if (tokenv[0] == "else") {
                                delete[] tokenv;
                                expect_error("'end if'");
                            }
                            
                            delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                    
                }
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new else_statement(_s, _dst);
                i = k;
            } else if (tokenv[0] == "finally") {
                delete[] tokenv;
                
                if (tokenc > 1)
                    expect_error("';' after expression");
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    if (tokenv[0] == "try")
                        ++p;
                    else {
                        tokenv = new string[src[k].length() + 1];
                        tokenc = tokens(tokenv, src[k]);
                        
                        if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "try") {
                            delete[] tokenv;
                            
                            --p;
                            
                            if (!p)
                                break;
                        } else if (p == 1) {
                            if (tokenv[0] == "catch" || tokenv[0] == "finally") {
                                delete[] tokenv;
                                expect_error("'end try'");
                            }
                            
                            delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                }
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new finally_statement(_s, _dst);
                i = k;
            } else if (tokenv[0] == "for") {
                delete[] tokenv;
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenc > 1 && tokenv[0] == "for") {
                        delete[] tokenv;
                        ++p;
                    } else if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "for") {
                        delete[] tokenv;
                        
                        if (tokenc > 2)
                            expect_error("';' after expression");
                        
                        --p;
                        
                        if (!p) break;
                    } else
                        delete[] tokenv;
                }
                
                if (k == ei) expect_error("'end for'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new for_statement(ltrim(src[i].substr(3)), _s, _dst);
                
                i = k + 1;
            } else if (tokenv[0] == "func") {
                delete[] tokenv;
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "func") {
                        delete[] tokenv;
                        ++p;
                    } else if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "func") {
                        delete[] tokenv;
                        if (tokenc > 2)
                            expect_error("';' after expression");
                        
                        --p;
                        
                        if (!p)
                            break;
                    } else
                        delete[] tokenv;
                }
                
                if (k == ei) expect_error("'end func'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new function_statement(ltrim(src[i].substr(4)), _s, _dst);
                i = k + 1;
            } else if (tokenv[0] == "if") {
                delete[] tokenv;
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "if")
                        ++p;
                    else if (tokenc >= 2 && tokenv[0] == "end" && tokenv[1] == "if") {
                        delete[] tokenv;
                        
                        if (tokenc > 2)
                            expect_error("';' after expression");
                        
                        --p;
                        
                        if (!p) break;
                    } else
                        delete[] tokenv;
                }
                
                if (k == ei) expect_error("'end if'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new if_statement(ltrim(src[i].substr(2)), _s, _dst);
                
                i = k + 1;
            } else if (tokenv[0] == "try") {
                delete[] tokenv;
                
                if (tokenc > 1)
                    expect_error("';' after expression");
                
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "try") {
                        delete[] tokenv;
                        ++p;
                    } else if (tokenc > 1 && tokenv[0] == "end" && tokenv[1] == "try") {
                        delete[] tokenv;
                        
                        if (tokenc > 2)
                            expect_error("';' after expression");
                        
                        --p;
                        
                        if (!p)
                            break;
                    } else
                        delete[] tokenv;
                }
                
                if (k == ei) expect_error("'end try'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new try_statement(_s, _dst);
                i = k + 1;
            } else if (tokenv[0] == "while") {
                delete[] tokenv;
        
                size_t k;   int p = 1;
                for (k = i + 1; k < ei; ++k) {
                    tokenv = new string[src[k].length() + 1];
                    tokenc = tokens(tokenv, src[k]);
                    
                    if (tokenv[0] == "while") {
                        delete[] tokenv;
                        ++p;
                    } else {
                        if (tokenc > 1) {
                            if (tokenv[0] == "do" && tokenv[1] == "while") {
                                delete[] tokenv;
                                ++p;
                            } else if (tokenv[0] == "end" && tokenv[1] == "while") {
                                delete[] tokenv;
                                
                                if (tokenc > 2)
                                    expect_error("';' after expression");
                                
                                --p;
                                
                                if (!p)
                                    break;
                            } else
                                delete[] tokenv;
                        } else
                            delete[] tokenv;
                    }
                }
                
                if (k == ei) expect_error("'end while'");
                
                statement_t** _dst = new statement_t*[k - i - 1];
                size_t _s = build(_dst, src, i + 1, k);
                
                dst[s++] = new while_statement(ltrim(src[i].substr(5)), _s, _dst);
                i = k + 1;
            } else {
                if (tokenv[0] == "assert") {
                    //  delete[] tokenv;
                    dst[s] = new assert_statement(ltrim(src[i].substr(6)));
                } else if (src[i] == "break") {
                    //  delete[] tokenv;
                    dst[s] = new break_statement();
                } else if (tokenv[0] == "consume") {
                    //  delete[] tokenv;
                    dst[s] = new consume_statement(ltrim(src[i].substr(8)));
                } else if (src[i] == "continue") {
                    //  delete[] tokenv;
                    dst[s] = new continue_statement();
                } else if (tokenv[0] == "echo") {
                    //  delete[] tokenv;
                    dst[s] = new echo_statement(ltrim(src[i].substr(4)));
                } else if (tokenv[0] == "return") {
                    //  delete[] tokenv;
                    dst[s] = new return_statement(ltrim(src[i].substr(6)));
                } else if (tokenv[0] == "sleep") {
                    //  delete[] tokenv;
                    dst[s] = new sleep_statement(ltrim(src[i].substr(5)));
                } else if (tokenv[0] == "throw") {
                    //  delete[] tokenv;
                    dst[s] = new exception_statement(ltrim(src[i].substr(5)));
                } else {
                    //  delete[] tokenv;
                    dst[s] = new statement(src[i]);
                }
                
                delete[] tokenv;
                
                ++i;
                ++s;
            }
        }
        
        return s;
    }

    bool file_statement::compare(const string val) const {
        unsupported_error("compare()");
        return false;
    }

    string file_statement::evaluate(interpreter* ssu) {
        unsupported_error("evaluate()");
        return EMPTY;
    }

    string file_statement::execute(interpreter* ssu) {
        analyze(ssu);
        
        should_return = false;
        
        for (size_t i = 0; i < statementc; ++i) {
            statementv[i]->execute(ssu);
            
            if (should_return)
                break;
        }
        
        return result;
    }

    void file_statement::set_break() { throw error("break cannot be used outside of a loop"); }

    void file_statement::set_continue() { throw error("continue cannot be used outside of a loop"); }

    void file_statement::set_parent(statement_t* parent) { unsupported_error("set_parent()"); }

    void file_statement::set_return(const string result) {
        this->result = result;
        this->should_return = true;
    }
}
