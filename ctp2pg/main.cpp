//
// Created by crazy on 2025/2/21.
//
#include "ctp2pg.h"

#include <CLI/App.hpp>

int main(int argc, char *argv[]) {
    CLI::App app("sink data to postgresql from ctp");

    std::string ctp_config{"ctp.yaml"};
    app.add_option("-c,--config", ctp_config, "ctp config file with yaml format.")->required();
    std::string pg_conn{};
    app.add_option("-p,--pg_conn", pg_conn,
                   "pg connection as: "
                   "postgresql://user:password@127.0.0.1:5432/test")->required();
    std::string table{"t_finance_ctp_hq"};
    app.add_option("-t,--table", table, "table name of hq data");
    std::vector<std::string> instruments{};
    app.add_option("-i,--instruments", instruments, "instruments to sink")->required();
    int delay{100};
    app.add_option("-d,--delay", delay, "delay in ms between each batch of data. Default is 100ms.");
    CLI11_PARSE(app, argc, argv);

    boost::asio::io_context io;
    lueing::Ctp2Pg ctp2pg(std::make_shared<lueing::ProviderCtp>(lueing::CreateCtpConfig(ctp_config)),
                          pg_conn, table, io);
    ctp2pg.Sink(instruments, delay);
    io.run();

    return 0;
}