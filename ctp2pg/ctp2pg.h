//
// Created by crazy on 2025/2/20.
//

#ifndef LUEING_QMT_CTP2PG_H
#define LUEING_QMT_CTP2PG_H

#include <string>

#include <pqxx/pqxx>
#include <boost/asio.hpp>
#include "hq.h"

namespace lueing {
#define STATUS_OK   0
#define STATUS_FAIL 1
#define PREPARED_INSERT_CLAUSE "prepared_insert_clause"


    class Ctp2Pg {
    private:
        ProviderCtpPtr ctp_;
        std::string table_name_;
        std::unique_ptr<pqxx::connection> conn_;
        std::unique_ptr<pqxx::work> tx_;
        boost::asio::steady_timer timer_;
        absl::node_hash_map<std::string, int> position_;

    public:
        explicit Ctp2Pg(ProviderCtpPtr ctp, const std::string &connect_string, std::string table_name,
                        boost::asio::io_context &io);

        ~Ctp2Pg();

    private:
        void Sink_(int delay_ms);
        int CreateTableIfNotExists();

    public:
        int Sink(std::vector<std::string> &instruments, int delay_ms);
    };
}
#endif //LUEING_QMT_CTP2PG_H
