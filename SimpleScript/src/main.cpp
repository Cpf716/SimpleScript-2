//
//  main.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 4/18/23.
//

#include "file.h"

using namespace ss;

interpreter ssu;

void signal_handler(int signum) {
    if (!ssu.signal(signum))
        return;
    
    logger_write("^C\n");
    logger_close();
    
    exit(signum);
}

int main(int argc, char* argv[]) {
    //  register signal callbacks
    signal(SIGINT, signal_handler);
    //  signal(SIGKILL, signal_handler);
    
    logger_write("Building...\n");
    
    string filename = argc == 1 ? BASE_DIR + "main.txt" : argv[1];
    
    time_point<steady_clock> beg;
    
    beg = steady_clock::now();
    
    //  initialize filepath tree
    //  this prevents recursive file inclusion
    node<string>* root = new node<string>(EMPTY, NULL);
    
    class file* target = new class file(filename, root, &ssu);
     
    root->close();
    
    ssu.set_function(target);
    
    time_point<steady_clock> end;
    
    end = steady_clock::now();
        
    logger_write("Done in " + to_string(duration<double>(end - beg).count()) + "s.\n");
    logger_write("Running...\n");
    
    ostringstream ss;
    
    ss << target->name() << "(";
    
    if (argc >= 3) {
        for (size_t i = 2; i < argc - 1; i += 1)
            ss << (is_double(argv[i]) ? rtrim(stod(argv[i])) : encode(argv[i])) << ",";
        
        ss << (is_double(argv[argc - 1]) ? rtrim(stod(argv[argc - 1])) : encode(argv[argc - 1]));
    }
    
    ss << ")";
    
    beg = steady_clock::now();
    
    try {
        statement(ss.str()).execute(&ssu);
        
    } catch (ss::error e) {
        logger_write("ERROR : " + string(e.what()) + "\n");
        
        ssu.print_stack_trace();
        
        throw e;
    }
    
    end = steady_clock::now();
    
    logger_write("Done in " + to_string(duration<double>(end - beg).count()) + "s.\n");
    logger_close();
}
