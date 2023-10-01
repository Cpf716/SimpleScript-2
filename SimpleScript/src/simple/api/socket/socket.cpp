//
//  socket.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 9/21/23.
//

#include "socket.h"

namespace simple {
    namespace api {
        //  CONSTRUCTORS

        socket::socket(const int fildes) {
            val.push_back(fildes);
            fla.push_back(true);
        }

        //  MEMBER FUNCTIONS

        bool socket::is_client() const {
            return (!add.size() && !parval.size()) || (add.size() == 1 && parval.size());
        }

        bool socket::is_parallel() const { return !!parval.size(); }

        bool socket::is_server() const {
            return (add.size() == 1 && !parval.size()) || (add.size() > 1 && parval.size());
        }

        void socket::set_address(const struct sockaddr_in add, const int addlen) {
            this->add.push_back(add);
            
            this->addlen = addlen;
            this->fla[0] = false;
        }

        //  NON-MEMBER FIELDS

        std::vector<struct socket*> thr;

        std::vector<struct socket*> socks;

        //  NON-MEMBER FUNCTIONS

        std::vector<int> socket_accept(const int fildes) {
            size_t i;
            for (i = 0; i < socks.size(); ++i) {
                if (socks[i]->val[0] == fildes) {
                    if (socks[i]->is_client())
                        return std::vector<int>();
                    
                    break;
                }
                
                if (socks[i]->is_server()) {
                    size_t j = 1;
                    while (j < socks[i]->val.size() && socks[i]->val[j] != fildes)
                        ++j;
                    
                    if (j != socks[i]->val.size())
                        return std::vector<int>();
                }
                
                size_t j = 1;
                while (j < socks[i]->parval.size() && socks[i]->parval[j] != fildes)
                    ++j;
                
                if (j != socks[i]->parval.size())
                    return std::vector<int>();
            }
            
            if (i == socks.size())
                return std::vector<int>();
            
            struct socket* sock = socks[i];
            
            i = 1;
            while (i < sock->val.size()) {
                if (::send(sock->val[i], std::string("\n").c_str(), 1, MSG_NOSIGNAL) <= 0)
                    sock->val.erase(sock->val.begin() + i);
                else
                    ++i;
            }
            
            return std::vector<int>(sock->val.begin() + 1, sock->val.end());
        }

        void handler_parallel_accept() {
            size_t thrnum = thr.size() - 1;
            
            while (1) {
                struct socket* sock = thr[thrnum];
                
                if (sock->fla[sock->fla.size() - 1])
                    break;
                
                //  returns nonnegative file descriptor or -1 for error
                int fildes = accept(sock->parval[1], (struct sockaddr *)&sock->add[sock->add.size() - 1], (socklen_t *)&sock->addlen);
                
                if (fildes == -1)
                    continue;
                
                sock->parval.push_back(fildes);
            }
            
            //  cout << "parallel accept joining...\n";
        }

        void handler_read() {
            size_t thrnum = thr.size() - 1;
                
            while (1) {
                struct socket* sock = thr[thrnum];
                
                if (sock->fla[sock->fla.size() - 1])
                    break;
                
                char valread[1024] = {0};
                
                //  returns message length (bytes), 0 for closed connection, or -1 for error
                if (read(sock->parval[0], valread, 1024) <= 0)
                    continue;
                
                std::string valv[strlen(valread) + 1];
                std::size_t valc = split(valv, std::string(valread), "\n");
                
                std::size_t i = 0;
                
                while (i < valc) {
                    if (valv[i].empty()) {
                        for (size_t j = i; j < valc - 1; ++j)
                            swap(valv[j], valv[j + 1]);
                        
                        --valc;
                    } else
                        ++i;
                }
                
                if (!valc)
                    continue;
                
                std::ostringstream ss;
                
                for (i = 0; i < valc - 1; ++i)
                    ss << valv[i] << "\n";
                
                ss << valv[i];
                
                std::string val = ss.str();
                        
                i = 2;
                while (i < sock->parval.size()) {
                    if (::send(sock->parval[i], val.c_str(), val.length(), MSG_NOSIGNAL) <= 0)
                        sock->parval.erase(sock->parval.begin() + i);
                    else
                        ++i;
                }
            }
            
            //  cout << "read joining...\n";
        }

        void handler_server_accept() {
            size_t thrnum = thr.size() - 1;
            
            while (1) {
                struct socket* sock = thr[thrnum];
                
                if (sock->fla[0])
                    break;
                
                //  returns nonnegative file descriptor or -1 for error
                int fildes = accept(sock->val[0], (struct sockaddr *)&sock->add[0], (socklen_t *)&sock->addlen);
                
                if (fildes == -1)
                    continue;
                
                sock->val.push_back(fildes);
            }
            
            //  cout << "server accept joining...\n";
        }

        int socket_client(const std::string src, const int port) {
            int fildes;
            
            while (1) {
                fildes = ::socket(AF_INET, SOCK_STREAM, 0);
                
                struct sockaddr_in add;

                add.sin_family = AF_INET;
                add.sin_port = htons(port);
                
                //  localhost
                inet_pton(AF_INET, src.c_str(), &add.sin_addr);
                
                //  returns 0 for success, -1 otherwise
                if (!connect(fildes, (struct sockaddr *)&add, sizeof(add)))
                    break;
                    
                close(fildes);
                
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            
            socks.push_back(new struct socket(fildes));
            
            return fildes;
        }

        int socket_close() {
            while (socks.size())
                socket_close(socks[0]->val[0]);
            
            return 0;
        }

        int socket_close(const int fildes) {
            for (size_t i = 0; i < socks.size(); ++i) {
                struct socket* sock = socks[i];
                
                if (sock->val[0] == fildes) {
                    if (sock->is_parallel()) {
                        sock->fla[sock->fla.size() - 1] = true;
                        
                        shutdown(sock->parval[1], SHUT_RDWR);
                        
                        for (size_t j = 1; j < sock->parval.size(); ++j)
                            close(sock->parval[j]);
                        
                        for (size_t j = sock->is_server() ? 1 : 0; j < sock->thr.size(); ++j)
                            if (sock->thr[j].joinable())
                                sock->thr[j].join();
                    }
                    
                    if (sock->is_client())
                        close(sock->val[0]);
                    else {
                        sock->fla[0] = true;
                        
                        shutdown(sock->val[0], SHUT_RDWR);
                        
                        for (size_t j = 0; j < sock->val.size(); ++j)
                            close(sock->val[j]);
                        
                        if (sock->thr[0].joinable())
                            sock->thr[0].join();
                    }
                    
                    delete sock;
                    
                    socks.erase(socks.begin() + i);
                    
                    return 0;
                }
                
                if (sock->is_server()) {
                    size_t j = 1;
                    while (j < sock->val.size() && sock->val[j] != fildes)
                        ++j;
                    
                    if (j != sock->val.size()) {
                        close(sock->val[j]);
                        
                        sock->val.erase(sock->val.begin() + j);
                        
                        return 0;
                    }
                }
                
                size_t j = 1;
                while (j < sock->parval.size() && sock->parval[j] != fildes)
                    ++j;
                
                if (j != sock->parval.size()) {
                    if (j == 1) {
                        sock->fla[sock->fla.size() - 1] = true;
                        
                        shutdown(sock->parval[1], SHUT_RDWR);
                        
                        for (size_t j = 1; j < sock->parval.size(); ++j)
                            close(sock->parval[j]);
                        
                        for (size_t j = sock->is_server() ? 1 : 0; j < sock->thr.size(); ++j)
                            if (sock->thr[j].joinable())
                                sock->thr[j].join();
                        
                        sock->parval.clear();
                        sock->add.erase(sock->add.end() - 1);
                        
                        return 0;
                    }
                    
                    return -1;
                }
            }
            
            return -1;
        }

        int socket_listen(const int fildes, const int port) {
            size_t i;
            for (i = 0; i < socks.size(); ++i) {
                if (socks[i]->val[0] == fildes) {
                    if (socks[i]->is_server() || socks[i]->is_parallel())
                        return -1;
                    
                    break;
                }
                
                if (socks[i]->is_server()) {
                    size_t j = 1;
                    while (j < socks[i]->val.size() && socks[i]->val[j] != fildes)
                        ++j;
                    
                    if (j != socks[i]->val.size()) {
                        int _fildes = ::socket(AF_INET, SOCK_STREAM, 0);
                        
                        int opt = 1;
                        
                        setsockopt(_fildes, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
                        
                        struct sockaddr_in add;
                            
                        add.sin_addr.s_addr = INADDR_ANY;
                        add.sin_family = AF_INET;
                        add.sin_port = htons(port);
                        
                        struct socket* sock = socks[i];
                        
                        bind(_fildes, (struct sockaddr *)&add, sock->addlen);
                        listen(_fildes, 1);
                        
                        sock->parval.push_back(fildes);
                        sock->parval.push_back(_fildes);
                        sock->add.push_back(add);
                        sock->fla.push_back(false);
                        
                        thr.push_back(sock);
                        
                        sock->thr.push_back(std::thread(handler_parallel_accept));
                        sock->thr.push_back(std::thread(handler_read));
                        
                        return _fildes;
                    }
                }
                
                size_t j = 1;
                while (j < socks[i]->parval.size() && socks[i]->parval[j] != fildes)
                    ++j;
                
                if (j != socks[i]->parval.size())
                    return -1;
            }
            
            //  socket is undefined
            if (i == socks.size())
                return -1;
            
            struct socket* sock = socks[i];
            
            int _fildes = ::socket(AF_INET, SOCK_STREAM, 0);
            
            int opt = 1;
            
            setsockopt(_fildes, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            
            struct sockaddr_in add;
                
            add.sin_addr.s_addr = INADDR_ANY;
            add.sin_family = AF_INET;
            add.sin_port = htons(port);
            
            int addlen = sizeof(add);
            
            bind(_fildes, (struct sockaddr *)&add, addlen);
            listen(_fildes, 1);
            
            sock->parval.push_back(fildes);
            sock->parval.push_back(_fildes);
            
            sock->set_address(add, addlen);
            
            thr.push_back(sock);
            
            sock->thr.push_back(std::thread(handler_parallel_accept));
            sock->thr.push_back(std::thread(handler_read));
            
            return _fildes;
        }

        std::string socket_recv(const int fildes) {
            size_t i;
            for (i = 0; i < socks.size(); ++i) {
                if (socks[i]->val[0] == fildes)
                    break;
                
                if (socks[i]->is_server()) {
                    size_t j = 1;
                    while (j < socks[i]->val.size() && socks[i]->val[j] != fildes)
                        ++j;
                    
                    if (j != socks[i]->val.size())
                        break;
                }
                
                size_t j = 1;
                while (j < socks[i]->parval.size() && socks[i]->parval[j] != fildes)
                    ++j;
                
                //  cannot read from socket
                if (j != socks[i]->parval.size())
                    return std::string();
            }
            
            //  socket is undefined
            if (i == socks.size())
                return std::string();
            
            while (1) {
                char valread[1024] = {0};
                
                if (read(fildes, valread, 1024) <= 0)
                    return std::string();
                
                std::string valv[strlen(valread) + 1];
                std::size_t valc = split(valv, std::string(valread), "\n");
                
                i = 0;
                
                while (i < valc) {
                    if (valv[i].empty()) {
                        for (size_t j = i; j < valc - 1; ++j)
                            swap(valv[j], valv[j + 1]);
                        
                        --valc;
                    } else
                        ++i;
                }
                
                if (!valc)
                    continue;
                
                std::ostringstream ss;
                            
                for (i = 0; i < valc - 1; ++i)
                    ss << valv[i] << "\n";
                
                ss << valv[i];
                
                return ss.str();
            }
        }

        int socket_send(const int fildes, const std::string msg) {
            size_t i;
            for (i = 0; i < socks.size(); ++i) {
                if (socks[i]->val[0] == fildes)
                    break;
                
                if (socks[i]->is_server()) {
                    size_t j = 1;
                    while (j < socks[i]->val.size() && socks[i]->val[j] != fildes)
                        ++j;
                    
                    if (j != socks[i]->val.size())
                        break;
                }
                
                size_t j = 1;
                while (j < socks[i]->parval.size() && socks[i]->parval[j] != fildes)
                    ++j;
                
                //  cannot write to socket
                if (j != socks[i]->parval.size())
                    return -1;
            }
            
            //  socket is undefined
            if (i == socks.size())
                return -1;
            
            //  check only that fildes does not belong to a listener
            
            return (int)::send(fildes, (msg + "\n").c_str(), msg.length() + 1, MSG_NOSIGNAL);
        }

        int socket_server(const int port, const int backlog) {
            //  check ports in use
            
            int fildes = ::socket(AF_INET, SOCK_STREAM, 0);
                    
            int opt = 1;
            
            setsockopt(fildes, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
            
            struct sockaddr_in add;
                
            add.sin_addr.s_addr = htonl(INADDR_ANY);
            add.sin_family = AF_INET;
            add.sin_port = htons(port);
            
            int addlen = sizeof(add);
            
            bind(fildes, (struct sockaddr *)&add, addlen);
            listen(fildes, backlog);
            
            struct socket* sock = new struct socket(fildes);
            
            sock->set_address(add, addlen);
            
            thr.push_back(sock);
            
            sock->thr.push_back(std::thread(handler_server_accept));
            
            socks.push_back(sock);
            
            return fildes;
        }
    }
}
