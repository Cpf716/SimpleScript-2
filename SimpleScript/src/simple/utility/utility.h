//
//  utility.h
//  SimpleScript
//
//  Created by Corey Ferguson on 11/1/22.
//

#ifndef utility_h
#define utility_h

#include "array.h"
#include "error.h"
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

namespace simple {
    //  NON-MEMBER FIELDS

    const static std::string BASE_DIR = "/Users/<user>/SimpleScript/SimpleScript/ssl/public/";
    const static std::string EMPTY = "";

    std::string decode(const std::string value);

    //  encode comma-separated value
    std::string encode(const std::string value);

    std::string filename(const std::string filepath);

    bool is_array(const std::string val);

    bool is_dictionary(const std::string* data, const std::size_t n);

    bool is_dictionary(simple::array<std::string> data);

    bool is_double(const std::string str);

    bool is_int(const double val);

    bool is_pow(const std::size_t a, const std::size_t b);

    bool is_string(const std::string str);

    bool is_symbol(const std::string str);

    bool is_table(std::string* data, const std::size_t n);

    bool is_table(simple::array<std::string> data);

    std::string ltrim(const std::string str);

    std::string rtrim(const double val);

    std::string rtrim(const std::string str);

    std::string trim(const std::string str);

    std::size_t merge(std::size_t n, std::string* data, const std::string pattern);

    //  parse comma-separated values
    std::size_t parse(std::string* dst, const std::string src);

    std::size_t parse(std::string* dst, const std::string src, const std::string pattern);

    std::size_t pow2(const std::size_t val);

    std::string raw(const std::string str);

    int read_csv(std::string* &dst, const std::string src);

    int read_tsv(std::string* &dst, const std::string src);

    std::string read_txt(const std::string filename);

    std::string* resize(const std::size_t n, const std::size_t p, std::string* data);

    std::size_t split(std::string* dst, const std::string src, const std::string pattern);

    std::string stringify(simple::array<std::string> data);

    std::string stringify(simple::array<std::string> data, std::size_t si);

    std::string stringify(simple::array<std::string> data, std::size_t si, std::size_t ei);

    std::string stringify(const std::size_t n, std::string* data);

    std::string tolower(const std::string str);

    std::string toupper(const std::string str);

    std::size_t tokens(std::string* dst, const std::string src);

    std::size_t tokenize(std::string* dst, const std::string src, const std::string pattern);

    std::string uuid();

    void write_csv(const std::string path, const std::size_t n, std::string* data);

    void write_tsv(const std::string path, const std::size_t n, std::string* data);

    void write_txt(const std::string path, const std::string data);
}

#endif /* commoon_h */
