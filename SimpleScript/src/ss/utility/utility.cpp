//
//  utility.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 11/4/22.
//

#include "utility.h"
#include <cmath>

namespace ss {
    const std::string delimeters[] = { "!", "(", ")", ",", ".", ":", ";" };

    std::string toupper(const std::string str) {
        char data[str.length() + 1];
        
        strcpy(data, str.c_str());
        
        for (std::size_t i = 0; i < str.length(); ++i)
            if (data[i] >= 97 && data[i] < 123)
                data[i] -= 32;
        
        return std::string(data);
    }

    std::string tolower(const std::string str) {
        char data[str.length() + 1];
        
        strcpy(data, str.c_str());
        
        for (std::size_t i = 0; i < str.length(); ++i)
            if (data[i] >= 65 && data[i] < 91)
                data[i] += 32;
        
        return std::string(data);
    }

    bool is_symbol(const std::string str) {
        if (str.empty())
            return false;
        
        std::size_t s = 0;
        while (s < str.length() && str[s] != '`')
            ++s;
        
        std::size_t e;
        if (s == str.length()) {
            s = 0;
            e = str.length();
        } else {
            if (s)
                return false;
            
            e = ++s;
            while (e < str.length() && str[e] != '`')
                ++e;
            
            if (e != str.length() - 1)
                return false;
        }
        
        if (str[s] != '_' && !isalpha(str[s]))
            return false;
        
        std::size_t i = s + 1;
        while (i < e && (str[i] == '_' || isalnum(str[i])))
            ++i;
        
        return i == e;
    }

    std::string encode(const std::string value) {
        std::size_t i = 0;
        std::size_t n = value.length();
        
        char data[n++ * 2 + 3];
        
        strcpy(data, value.c_str());
        
        data[n] = '\"';
        
        for (i = n; i > 0; --i)
            std::swap(data[i], data[i - 1]);
        ++n;
        
        for (i = 1; i < n - 1; ++i) {
            if (data[i] == '\"') {
                data[n] = '\"';
                
                for (std::size_t j = n; j > i + 1; --j)
                    std::swap(data[j], data[j - 1]);
                
                ++i;
                ++n;
            }
        }
        
        data[n] = '\"';
        
        std::swap(data[n], data[n - 1]);
        
        ++n;
        
        return std::string(data);
    }

    std::string trim(const std::string str) {
        std::size_t n = str.length() + 1;
        char data[n];
        
        strcpy(data, str.c_str());
        
        while (n > 1 && isspace(data[n - 2])) {
            std::swap(data[n - 2], data[n - 1]);
            --n;
        }
        
        while (n > 1 && isspace(data[0])) {
            for (std::size_t i = 0; i < n - 1; ++i)
                std::swap(data[i], data[i + 1]);
            --n;
        }
            
        return std::string(data);
    }

    //  postcondition:  removes leading spaces
    std::string ltrim(const std::string str) {
        std::size_t l = str.length() + 1;
        char _str[l];
        strcpy(_str, str.c_str());
        
        while (l > 1 && isspace(_str[0])) {
            for (std::size_t i = 0; i < l - 1; ++i)
                std::swap(_str[i], _str[i + 1]);
            --l;
        }
        
        return std::string(_str);
    }

    //  postcondition:  removes trailing spaces
    std::string rtrim(const std::string str) {
        std::size_t l = str.length() + 1;
        char _str[l];
        strcpy(_str, str.c_str());
        
        while (l > 1 && isspace(_str[l - 2])) {
            std::swap(_str[l - 2], _str[l - 1]);
            --l;
        }
        
        return std::string(_str);
    }

    std::string rtrim(const double val) {
        std::string str = std::to_string(val);
        
        std::size_t n = str.length();
        while (n > 0 && str[n - 1] == '0')
            --n;
        
        if (str[n - 1] == '.')
            --n;
        
        return str.substr(0, n);
    }

    bool is_string(const std::string str) {
        std::size_t i = 0;
        while (i < str.length() && str[i] != '\"')
            ++i;
        
        return i != str.length();
    }

    std::size_t parse(std::string* dst, const std::string src) { return parse(dst, src, ","); }

    std::size_t parse(std::string* dst, const std::string src, const std::string pattern) {
        std::size_t n = split(dst, src, pattern);
        
        n = merge(n, dst, pattern);
        
        for (std::size_t i = 0; i < n; ++i)
            dst[i] = trim(dst[i]);
     
        return n;
    }

    bool is_double(const std::string str) {
        if (str.empty())
            return false;
        
        size_t i = 0;
        if (str[i] == '+' || str[i] == '-')
            ++i;
        
        size_t j = i;
        while (j < str.length() && str[j] != '.')
            ++j;
        
        size_t k = j == str.length() ? i : j;
        while (k < str.length() && str[k] != 'E' && str[k] != 'e')
            ++k;
        
        size_t l = j < k ? j : k, m = i;
        
        while (m < l && isdigit(str[m]))
            ++m;
        
        if (m != l)
            return false;
        
        size_t n = l - i;
        
        if (j != str.length()) {
            m = j + 1;
            while (m < k && isdigit(str[m]))
                ++m;
            
            if (m != k)
                return false;
            
            n += k - j - 1;
        }
        
        if (n == 0)
            return false;
        
        if (k != str.length()) {
            size_t l = k + 1;
            
            if (l == str.length() || (str[l] != '+' && str[l] != '-'))
                return false;
            
            ++l;
            
            if (l == str.length())
                return false;
            
            while (l < str.length() && isdigit(str[l]))
                ++l;
            
            if (l != str.length())
                return false;
        }
        
        return true;
    }

    std::string decode(const std::string value) {
        int l = 0;
        while (l < value.length() && value[l] != '\"')
            ++l;
        
        if (l == value.length())
            return value;
            
        std::size_t n = value.length() + 1;
        char dst[n];
        
        strcpy(dst, value.c_str());
        
        for (std::size_t j = l; j < n - 1; ++j)
            std::swap(dst[j], dst[j + 1]);
        --n;
        
        int i = l, r = -1;
        while (i < n - 1) {
            if (dst[i] == '\"') {
                r = i + 1;
                while (r < n - 1 && dst[r] == '\"')
                    ++r;
                
                if ((r - i) % 2)
                    break;
                
                i = r;
            } else
                ++i;
        }
                
        if (i == n - 1) {
            std::cout << "Missing terminating '\"' character: (" << value << ")\n";
            
            //  BEGIN
            
            for (int j = l; j < (int)n - 2; ++j) {
                std::size_t k = 0;
                while (k < 2 && dst[j + k] == ("\\n")[k])
                    ++k;
                
                if (k == 2) {
                    dst[j] = '\n';
                    
                    for (std::size_t m = j + 1; m < n - 1; ++m)
                        std::swap(dst[m], dst[m + 1]);
                    --n;
                }
            }
            
            for (int j = l; j < (int)n - 2; ++j) {
                std::size_t k = 0;
                while (k < 2 && dst[j + k] == ("\\t")[k])
                    ++k;
                
                if (k == 2) {
                    dst[j] = '\t';
                    
                    for (std::size_t m = j + 1; m < n - 1; ++m)
                        std::swap(dst[m], dst[m + 1]);
                    --n;
                }
            }
            
            //  END
            
            for (int j = l; j < (int)n - 1; ++j) {
                if (dst[j] == '\"') {
                    std::size_t k = j + 1;
                    while (k < n - 1 && dst[k] == '\"')
                        ++k;
                    
                    std::size_t m;
                    for (m = 0; m < (k - j) / 2; ++m) {
                        for (std::size_t p = j; p < n - 1; ++p)
                            std::swap(dst[p], dst[p + 1]);
                        --n;
                    }
                    
                    j += m;
                }
            }
        } else {
            for (std::size_t j = --r; j < n - 1; ++j)
                std::swap(dst[j], dst[j + 1]);
            --n;
            --r;
            
            //  BEGIN
                        
            for (int j = l; j < r; ++j) {
                int k = 0;
                while (k < 2 && dst[j + k] == ("\\n")[k])
                    ++k;
                
                if (k == 2) {
                    dst[j] = '\n';
                    
                    for (std::size_t m = j + 1; m < n - 1; ++m)
                        std::swap(dst[m], dst[m + 1]);
                    --n;
                    --r;
                }
            }
            
            for (int j = l; j < r; ++j) {
                std::size_t k = 0;
                while (k < 2 && dst[j + k] == ("\\t")[k])
                    ++k;
                
                if (k == 2) {
                    dst[j] = '\t';
                    
                    for (std::size_t m = j + 1; m < n - 1; ++m)
                        std::swap(dst[m], dst[m + 1]);
                    --n;
                }
            }
            
            //  END
                        
            for (int j = l; j < r; ++j) {
                if (dst[j] == '\"') {
                    std::size_t k = j + 1;
                    while (k < r && dst[k] == '\"')
                        ++k;
                    
                    std::size_t m;
                    for (m = 0; m < (k - j) / 2; ++m) {
                        for (std::size_t p = j; p < n - 1; ++p)
                            std::swap(dst[p], dst[p + 1]);
                        --n;
                        --r;
                    }
                    
                    j += m;
                }
            }
                          
            std::size_t j = r;
            while (j < n - 1) {
                if (dst[j] == '\"') {
                    for (std::size_t k = j; k < n - 1; ++k)
                        std::swap(dst[k], dst[k + 1]);
                    --n;
                } else
                    ++j;
            }
        }
        
        return std::string(dst);
    }

    std::string* resize(const std::size_t n, const std::size_t p, std::string* data) {
        const std::size_t q = p > n ? n : p;
        
        std::string* tmp = new std::string[p];
        
        for (std::size_t i = 0; i < q; ++i)
            tmp[i] = data[i];
        
        delete[] data;
        
        return tmp;
    }

    std::size_t merge(std::size_t n, std::string* data, const std::string pattern) {
        for (int i = 0; i < (int)n - 1; ++i) {
            std::size_t l = 0;
            while (l < data[i].length() && data[i][l] != '\"')
                ++l;
            
            if (l != data[i].length()) {
                bool f = true;
                
                std::size_t j = l + 1;
                while (j < data[i].length()) {
                    if (data[i][j] == '\"') {
                        std::size_t r = j + 1;
                        while (r < data[i].length() && data[i][r] == '\"')
                            ++r;
                        
                        if ((r - j) % 2 == 0)
                            j = r;
                        else {
                            f = false;
                            break;
                        }
                    } else
                        ++j;
                }
                
                if (f) {
                    bool g = true;
                    
                    while (g && i < n - 1) {
                        std::size_t j = 0;
                        while (j < data[i + 1].length()) {
                            if (data[i + 1][j] == '\"') {
                                std::size_t r = j + 1;
                                while (r < data[i + 1].length() && data[i + 1][r] == '\"')
                                    ++r;
                                
                                if ((r - j) % 2 == 0)
                                    j = r;
                                else {
                                    g = false;
                                    break;
                                }
                            } else
                                ++j;
                        }
                        
                        data[i] += pattern + data[i + 1];
                        
                        for (std::size_t k = i + 1; k < n - 1; ++k)
                            std::swap(data[k], data[k + 1]);
                        --n;
                    }
                }
            }
        }
        
        return n;
    }

    std::size_t split(std::string* dst, const std::string src, const std::string pattern) {
        std::size_t s = 0, n = 0;
        for (int e = 0; e <= (int)src.length() - (int)pattern.length(); ++e) {
            std::size_t i = 0;
            while (i < pattern.length() && src[e + i] == pattern[i])
                ++i;
            
            if (i == pattern.length()) {
                dst[n++] = src.substr(s, e - s);
                
                s = e + i;
            }
        }
        
        dst[n++] = src.substr(s);

        return n;
    }

    //  split, but preserves the pattern
    std::size_t tokenize(std::string* dst, const std::string src, const std::string pattern) {
        std::size_t s = 0, n = 0;    int e;
        for (e = 0; e <= (int)src.length() - (int)pattern.length(); ++e) {
            std::size_t i = 0;
            while (i < pattern.length() && src[e + i] == pattern[i])
                ++i;
            
            if (i == pattern.length()) {
                if (s != e)
                    dst[n++] = src.substr(s, e - s);
                
                dst[n++] = pattern;
                s = e + i;
            }
        }
        
        if (s != src.length())
            dst[n++] = src.substr(s);

        return n;
    }

    bool is_array(const std::string val) {
        std::string data[val.length() + 1];
        
        return parse(data, val) != 1;
    }

    std::string stringify(ss::array<std::string> data, std::size_t beg, std::size_t len) {
        std::stringstream ss;
        
        if (len) {
            std::size_t i, n = beg + len;
            for (i = beg; i < n - 1; ++i)
                ss << data[i] << ',';
            
            ss << data[i];
        }
        
        return ss.str();
    }

    std::string stringify(ss::array<std::string> data) { return stringify(data, 0, data.size()); }

    std::string stringify(ss::array<std::string> data, std::size_t beg) { return stringify(data, beg, data.size() - beg); }

    std::string stringify(const std::size_t n, std::string* data) {
        if (!n)
            return EMPTY;
        
        std::stringstream ss;
        
        if (n) {
            for (std::size_t i = 0; i < n - 1; ++i)
                ss << data[i] << ',';
            
            ss << data[n - 1];
        }
        
        return ss.str();
    }

    std::size_t tokens(std::string* dst, const std::string src) {
        std::size_t n = 0, s = 0;
        for (std::size_t e = 0; e < src.length(); ++e) {
            while (e < src.length() && isspace(src[e]))
                ++e;
            
            if (s != e)
                dst[n++] = src.substr(s, e - s);
            
            s = e;
            while (e < src.length() && !isspace(src[e]))
                ++e;
            
            if (s != e)
                dst[n++] = src.substr(s, e - s);
            
            s = e;
        }
        
        std::size_t i = 0;
        while (i < n) {
            std::size_t tokenc = 0;
            std::string tokenv[dst[i].length() + 1];
            
            s = 0;  std::size_t e = 0;
            while (e < dst[i].length()) {
                std::size_t j;
                for (j = 0; j < sizeof (delimeters) / sizeof(delimeters[0]); ++j) {
                    if (e + delimeters[j].length() > dst[i].length())
                        continue;
                    
                    std::size_t k = 0;
                    while (k < delimeters[j].length() && dst[i][e + k] == delimeters[j][k])
                        ++k;
                    
                    if (k == delimeters[j].length())
                        break;
                }
                
                if (j == sizeof (delimeters) / sizeof(delimeters[0]))
                    ++e;
                else {
                    if (s != e)
                        tokenv[tokenc++] = dst[i].substr(s, e - s);
                    
                    tokenv[tokenc++] = delimeters[j];
                    s = ++e;
                }
            }
            
            if (s != dst[i].length())
                tokenv[tokenc++] = dst[i].substr(s);
            
            dst[i] = tokenv[0];
            
            for (std::size_t j = 1; j < tokenc; ++j) {
                dst[n] = tokenv[j];
                
                for (std::size_t k = n;  k > i + j; --k)
                    std::swap(dst[k], dst[k - 1]);
                
                ++n;
            }
            
            i += tokenc;
        }
        
        n = merge(n, dst, EMPTY);
        
        int p = 0;
        for (i = 0; i < n; ++i) {
            if (dst[i] == "(")
                ++p;
            else if (dst[i] == ")") {
                if (!p)
                    throw error("Syntax error on token \")\", delete this token");
                    
                --p;
            }
        }
        
        if (p)
            throw error("Syntax error, insert \")\" to complete expr body");
        
        i = 0;
        while (i < n) {
            dst[i] = trim(dst[i]);
            
            if (dst[i].empty()) {
                for (std::size_t j = i; j < n - 1; ++j)
                    std::swap(dst[j], dst[j + 1]);
                
                --n;
            } else
                ++i;
        }
        
        return n;
    }

    bool is_dictionary(const std::string* data, const std::size_t n) {
        if (n % 2 == 0) {
            std::size_t i = 0;
            while (i < (std::size_t)floor(n / 2) && is_string(data[i * 2]) && data[i * 2].length() != 2)
                ++i;
            
            if (i == (std::size_t)floor(n / 2)) {
                for (i = 0; i < (std::size_t)floor(n / 2); ++i) {
                    std::size_t j = i + 1;
                    while (j < (std::size_t)floor(n / 2) && data[i * 2] != data[j * 2])
                        ++j;
                    
                    if (j != (std::size_t)floor(n / 2))
                        break;
                }
                
                return i == (std::size_t)floor(n / 2);
            }
        }
        
        return false;
    }

    bool is_dictionary(ss::array<std::string> data) {
        if (data.size() % 2 == 0) {
            std::size_t i = 0;
            while (i < (std::size_t)floor(data.size() / 2) && is_string(data[i * 2]) && data[i * 2].length() != 2)
                ++i;
            
            if (i == (std::size_t)floor(data.size() / 2)) {
                for (i = 0; i < (std::size_t)floor(data.size() / 2); ++i) {
                    std::size_t j = i + 1;
                    while (j < (std::size_t)floor(data.size() / 2) && data[i * 2] != data[j * 2])
                        ++j;
                    
                    if (j != (std::size_t)floor(data.size() / 2))
                        break;
                }
                
                return i == (std::size_t)floor(data.size() / 2);
            }
        }
        
        return false;
    }


    bool is_table(std::string* data, const std::size_t n) {
        if (n > 1 && (!data[0].empty() && !is_string(data[0]))) {
            double d = stod(data[0]);
            if (d - (int)d == 0 && d >= 1) {
                std::size_t c = (std::size_t)d;
                return (n - 1) % c == 0;
            }
        }
        
        return false;
    }

    bool is_table(ss::array<std::string> data) {
        if (data.size() > 1 && (!data[0].empty() && !is_string(data[0]))) {
            double d = stod(data[0]);
            if (d - (int)d == 0 && d >= 1) {
                std::size_t c = (std::size_t)d;
                return (data.size() - 1) % c == 0;
            }
        }
        
        return false;
    }

    std::string filename(const std::string filepath) {
        std::size_t ei = filepath.length();
        while (ei > 0 && filepath[ei - 1] != '.')
            --ei;
        
        if (!ei)
            ei = filepath.length();
        
        std::size_t si = ei;
        while (si > 0 && filepath[si - 1] != '/')
            --si;
        
        if (filepath[ei - 1] == '.')
            --ei;
        
        return filepath.substr(si, ei - si);
    }

    int read_csv(std::string* &dst, const std::string src) {
        assert(!src.empty());
        
        std::ifstream file;
        
        file.open(src);
        
        if (!file)
            return -1;
        
        dst = new std::string[1];
        int n = 0;
        
        std::string line;
        while (getline(file, line)) {
            if (is_pow(n, 2))
                dst = resize(n, n * 2, dst);
            
            dst[n++] = line;
        }
        
        file.close();
        
        std::size_t p = n;
        
        n = (int)merge(n, dst, "\n");
        
        std::size_t i = 0, m = 0;
        while (i < n) {
            std::string valuev[dst[i].length() + 1];
            std::size_t valuec = parse(valuev, dst[i]);
            
            if (valuec > m)
                m = valuec;
            
            if (is_pow(n, 2))
                dst = resize(n, n * 2, dst);
            
            dst[n] = std::to_string(valuec);
            
            for (std::size_t j = n; j > i; --j)
                std::swap(dst[j], dst[j - 1]);
            
            ++n;
            ++i;
            
            dst[i] = raw(valuev[0]);
            
            for (std::size_t j = 1; j < valuec; ++j) {
                if (is_pow(n, 2))
                    dst = resize(n, n * 2, dst);
                
                dst[n] = raw(valuev[j]);
                
                for (std::size_t k = n; k > i + j; --k)
                    std::swap(dst[k], dst[k - 1]);
                
                ++n;
            }
            
            i += valuec;
        }
        
        ++m;
        
        for (std::size_t i = 0; i < p; ++i) {
            std::size_t j = stoi(dst[i * m]);
            for (std::size_t k = 0; k < m - j - 1; ++k) {
                if (is_pow(n, 2))
                    dst = resize(n, n * 2, dst);
                
                dst[n] = EMPTY;
                
                for (std::size_t l = n; l > i * m + j + k + 1; --l)
                    std::swap(dst[l], dst[l - 1]);
                
                n++;
            }
        }
        
        dst[n] = std::to_string(m);
        
        for (std::size_t i = n; i > 0; --i)
            std::swap(dst[i], dst[i - 1]);
        
        ++n;
        
        if (!is_pow(n, 2))
            dst = resize(n, n, dst);
        
        return n;
    }

    std::string read_txt(const std::string filename) {
        std::ifstream file;
        
        file.open(filename);
        
        if (!file)
            return "undefined";
        
        std::ostringstream ss;
        
        ss << file.rdbuf();
        
        return ss.str();
    }

    int read_tsv(std::string* &dst, const std::string src) {
        assert(!src.empty());
        
        std::ifstream file;
        
        file.open(src);
        
        if (!file)
            return -1;
        
        dst = new std::string[1];
        int n = 0;
        
        std::string line;
        while (getline(file, line)) {
            if (is_pow(n, 2))
                dst = resize(n, n * 2, dst);
            
            dst[n++] = line;
        }
        
        n = (int)merge(n, dst, "\n");
        
        const std::size_t p = n;

        std::size_t i = 0, j = 0;
        while (i < n) {
            std::string valuev[dst[i].length() + 1];
            std::size_t valuec = parse(valuev, dst[i], "\t");

            if (valuec > j)
                j = valuec;

            if (is_pow(n, 2))
                dst = resize(n, n * 2, dst);

            dst[n] = std::to_string(valuec);

            for (std::size_t k = n; k > i; --k)
                std::swap(dst[k], dst[k - 1]);

            ++n;
            ++i;

            dst[i] = raw(valuev[0]);

            for (std::size_t k = 1; k < valuec; ++k) {
                if (is_pow(n, 2))
                    dst = resize(n, n * 2, dst);

                dst[n] = raw(valuev[k]);

                for (std::size_t l = n; l > i + k; --l)
                    std::swap(dst[l], dst[l - 1]);

                ++n;
            }

            i += valuec;
        }

        ++j;

        for (i = 0; i < p; ++i) {
            std::size_t k = stoi(dst[i * j]);
            for (std::size_t l = 0; l < j - k - 1; ++l) {
                if (is_pow(n, 2))
                    dst = resize(n, n * 2, dst);

                dst[n] = EMPTY;

                for (std::size_t m = n; m > i * j + k + l + 1; --m)
                    std::swap(dst[m], dst[m - 1]);

                ++n;
            }
        }

        dst[n] = std::to_string(j);

        for (i = n; i > 0; --i)
            std::swap(dst[i], dst[i - 1]);

        ++n;
        
        if (!is_pow(n, 2))
            dst = resize(n, n, dst);
        
        return n;
    }

    std::string raw(const std::string str) {
        if (str.empty())
            return str;
        
        if (is_string(str))
            return encode(decode(str));
        
        if (is_double(str))
            return rtrim(stod(str));
        
        return encode(str);
    }

    bool is_int(const double val) {
        return val - (long)val == 0;
    }

    bool is_pow(const std::size_t a, const std::size_t b) {
        if (!b)
            return !a || a == 1;
        
        if (!a)
            return false;
        
        return is_int(log(a) / log(b));
    }

    std::size_t pow2(const std::size_t val) {
        if (!val)
            return 1;
        
        return pow(2, ceil(log(val) / log(2)));
    }

    std::string uuid() {
        std::ostringstream os;
        
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<> uid;
        
        for (std::size_t i = 0; i < 8; ++i)
            os << std::hex << uid(gen) % 16;
        
        os << '-';
        
        for (std::size_t i = 0; i < 3; ++i) {
            for (std::size_t j = 0; j < 4; ++j)
                os << uid(gen) % 16;
            
            os << '-';
        }
        
        for (std::size_t i = 0; i < 12; ++i)
            os << std::hex << uid(gen) % 16;
        
        return os.str();
    }

    void write_csv(const std::string path, const std::size_t n, std::string* data) {
        assert(!path.empty());
        
        std::ofstream file;
        
        file.open(path);
        
        for (std::size_t i = 0; i < (n - 1) / stoi(data[0]); ++i) {
            for (std::size_t j = 0; j < stoi(data[i * stoi(data[0]) + 1]) - 1; ++j) {
                std::string value = data[i * stoi(data[0]) + j + 2];
                
                value = decode(value);
                
                std::size_t k = 0;
                while (k < value.length() && value[k] != '\"' && value[k] != ',')
                    ++k;
                
                if (k != value.length())
                    value = encode(value);
                
                file << value << ",";
            }
            
            std::string value = data[i * stoi(data[0]) + stoi(data[i * stoi(data[0]) + 1]) + 1];
            
            value = decode(value);
            
            std::size_t j = 0;
            while (j < value.length() && value[j] != '\"' && value[j] != ',')
                ++j;
            
            if (j != value.length())
                value = encode(value);
                
            file << value << "\n";
        }
        
        file.close();
    }
    void write_tsv(const std::string path, const std::size_t n, std::string* data) {
        assert(!path.empty());
        
        std::ofstream file;
        
        file.open(path);
        
        for (std::size_t i = 0; i < (n - 1) / stoi(data[0]); ++i) {
            for (std::size_t j = 0; j < stoi(data[i * stoi(data[0]) + 1]) - 1; ++j) {
                std::string value = data[i * stoi(data[0]) + j + 2];
                
                value = decode(value);
                
                std::size_t k = 0;
                while (k < value.length() && value[k] != '\t' && value[k] != '\"')
                    ++k;
                
                if (k != value.length())
                    value = encode(value);
                
                file << value << "\t";
            }
            
            std::string value = data[i * stoi(data[0]) + stoi(data[i * stoi(data[0]) + 1]) + 1];
            
            value = decode(value);
            
            std::size_t j = 0;
            while (j < value.length() && value[j] != '\t' && value[j] != '\"')
                ++j;
            
            if (j != value.length())
                value = encode(value);
                
            file << value << "\n";
        }
        
        file.close();
    }

    void write_txt(const std::string path, const std::string data) {
        std::ofstream file;
        
        file.open(path);
        
        file << data;
        
        file.close();
    }
}
