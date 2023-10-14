//
//  mysql.cpp
//  SimpleScript
//
//  Created by Corey Ferguson on 9/22/23.
//

#include "mysql.h"

namespace ss {
    namespace api {
        size_t autoincrement = 10000;

        bst<pair<size_t, int>>* connbst = NULL;
    
        vector<pair<size_t, sql::Connection*>> conns;

        size_t mysql_connect(const string host_name, const string user_name, const string password) {
            sql::Driver *driver = NULL;
            sql::Connection *conn = NULL;
            
            try {
                driver = get_driver_instance();
                
                conn = driver->connect(host_name, user_name, password);
                
            } catch (sql::SQLException &e) {
                throw e;
            }
            
            conns.push_back(pair<int, sql::Connection*>(autoincrement, conn));
            
            if (connbst != NULL)
                connbst->close();
            
            size_t symv[conns.size()];
            
            for (size_t i = 0; i < conns.size(); ++i)
                symv[i] = conns[i].first;
            
            connbst = build(symv, 0, (int)conns.size());
            
            return autoincrement++;
        }

        int mysql_close(const size_t connection) {
            if (connbst == NULL)
                return -1;
            
            int i = index_of(connbst, connection);
            
            if (i == -1)
                return -1;
            
            try {
                conns[i].second->close();
                
            } catch (sql::SQLException &e) {
                throw e;
            }
            
            delete conns[i].second;
            
            conns.erase(conns.begin() + i);
            
            connbst->close();
            
            if (conns.size()) {
                size_t symv[conns.size()];
                
                for (size_t i = 0; i < conns.size(); ++i)
                    symv[i] = conns[i].first;
                
                connbst = build(symv, 0, (int)conns.size());
            } else
                connbst = NULL;
            
            return 0;
        }

        int mysql_close() {
            if (connbst != NULL)
                connbst->close();
            
            for (size_t i = 0; i < conns.size(); ++i) {
                try {
                    conns[i].second->close();
                    
                    delete conns[i].second;
                    
                } catch (sql::SQLException &e) {
                    throw e;
                }
                
            }
            
            return 0;
        }

        bool mysql_set_schema(const size_t connection, const std::string schema) {
            if (connbst == NULL)
                return false;
            
            int i = index_of(connbst, connection);
            
            if (i == -1)
                return false;
            
            try {
                conns[i].second->setSchema(schema);
                
            } catch (sql::SQLException& e) {
                throw e;
            }
            
            return true;
        }

        sql::ResultSet* mysql_prepare_query(const size_t connection, const string sql, const size_t argc, string* argv) {
            if (connbst == NULL)
                return NULL;
            
            int i = index_of(connbst, connection);
            
            if (i == -1)
                return NULL;
            
            sql::PreparedStatement* prep_stmt = NULL;
            sql::ResultSet* res = NULL;
            
            try {
                prep_stmt = conns[i].second->prepareStatement(sql);
                
                for (int j = 0; j < argc; ++j) {
                    try {
                        double num = stod(argv[j]);
                        
                        prep_stmt->setDouble(j + 1, num);
                        
                    } catch (invalid_argument& e) {
                        prep_stmt->setString(j + 1, argv[j]);
                    }
                }
                
                res = prep_stmt->executeQuery();
                
            } catch (sql::SQLException &e) {
                throw e;
            }
            
            delete prep_stmt;
            
            return res;
        }

        int mysql_prepare_update(const size_t connection, const string sql, const size_t argc, string* argv) {
            if (connbst == NULL)
                return -1;
            
            int i = index_of(connbst, connection);
            
            if (i == -1)
                return -1;
            
            sql::PreparedStatement* prep_stmt = NULL;

            int res;
            
            try {
                prep_stmt = conns[i].second->prepareStatement(sql);
                
                for (int j = 0; j < argc; ++j) {
                    try {
                        double num = stod(argv[j]);
                        
                        prep_stmt->setDouble(j + 1, num);
                        
                    } catch (invalid_argument& e) {
                        prep_stmt->setString(j + 1, argv[j]);
                    }
                }
                
                res = prep_stmt->executeUpdate();
                
            } catch (sql::SQLException &e) {
                throw e;
            }
            
            delete prep_stmt;
            
            return res;
        }

        int mysql_update(const size_t connection, const string sql) {
            if (connbst == NULL)
                return -1;
            
            int i = index_of(connbst, connection);
            
            if (i == -1)
                return -1;
            
            sql::Statement *stmt = NULL;
            
            int res;
            
            try {
                stmt = conns[i].second->createStatement();
                
                res = stmt->executeUpdate(sql);
                
            } catch (sql::SQLException &e) {
                throw e;
            }
            
            delete stmt;
            
            return res;
        }

        sql::ResultSet* mysql_query(const size_t connection, const string sql) {
            if (connbst == NULL)
                return NULL;
            
            int i = index_of(connbst, connection);
            
            if (i == -1)
                return NULL;
            
            sql::Statement *stmt = NULL;
            sql::ResultSet *res = NULL;
            
            try {
                stmt = conns[i].second->createStatement();
                
                res = stmt->executeQuery(sql);
                
            } catch (sql::SQLException &e) {
                throw e;
            }
            
            delete stmt;
            
            return res;
        }
    }
}
