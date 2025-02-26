//
// Created by crazy on 2025/2/20.
//
#include "ctp2pg.h"

#include <utility>
#include "fmt/format.h"

lueing::Ctp2Pg::Ctp2Pg(lueing::ProviderCtpPtr ctp, const std::string &connect_string, std::string table_name,
                       boost::asio::io_context &io) :
        ctp_(std::move(ctp)), table_name_(std::move(table_name)), conn_(std::make_unique<pqxx::connection>(connect_string)),
        timer_(io, boost::asio::chrono::milliseconds(200)) {
    tx_ = std::make_unique<pqxx::work>(*conn_);
    CreateTableIfNotExists();
    tx_->conn().prepare(
            PREPARED_INSERT_CLAUSE,
            fmt::format("INSERT INTO {} (                                                   "
                        "trading_day, reserve1, exchange_id, reserve2,                      "
                        "last_price, pre_settlement_price,                                  "
                        "pre_close_price, pre_open_interest,                                "
                        "open_price, highest_price, lowest_price, volume,                   "
                        "turnover, open_interest, close_price, settlement_price,            "
                        "upperlimit_price, lowerlimit_price, pre_delta, curr_delta,         "
                        "update_time, update_millisec,                                      "
                        "bid_price1, bid_volume1, ask_price1, ask_volume1,                  "
                        "bid_price2, bid_volume2, ask_price2, ask_volume2,                  "
                        "bid_price3, bid_volume3, ask_price3, ask_volume3,                  "
                        "bid_price4, bid_volume4, ask_price4, ask_volume4,                  "
                        "bid_price5, bid_volume5, ask_price5, ask_volume5,                  "
                        "average_price, action_day, instrument_id, exchange_inst_id,        "
                        "banding_upperprice, banding_lowerprice                             "
                        ") VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9,                      "
                        "$10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21         "
                        ", $22, $23, $24, $25, $26, $27, $28, $29, $30, $31, $32, $33, $34  "
                        ", $35, $36, $37, $38, $39, $40, $41, $42, $43, $44, $45, $46, $47  "
                        ", $48)                                                             ",
                        table_name_
            )
    );
}

lueing::Ctp2Pg::~Ctp2Pg() = default;

int lueing::Ctp2Pg::CreateTableIfNotExists() {
    std::string comment_clause = "COMMENT ON COLUMN {}.{} IS '{}'";
    std::string comments[49]{
            fmt::format(comment_clause, table_name_, "id", "序列号"),
            fmt::format(comment_clause, table_name_, "trading_day", "交易日"),
            fmt::format(comment_clause, table_name_, "reserve1", "保留的无效字段"),
            fmt::format(comment_clause, table_name_, "exchange_id", "交易所代码"),
            fmt::format(comment_clause, table_name_, "reserve2", "保留的无效字段"),
            fmt::format(comment_clause, table_name_, "last_price", "最新价"),
            fmt::format(comment_clause, table_name_, "pre_settlement_price", "上次结算价"),
            fmt::format(comment_clause, table_name_, "pre_close_price", "昨收盘"),
            fmt::format(comment_clause, table_name_, "pre_open_interest", "昨持仓量"),
            fmt::format(comment_clause, table_name_, "open_price", "今开盘"),
            fmt::format(comment_clause, table_name_, "highest_price", "最高价"),
            fmt::format(comment_clause, table_name_, "lowest_price", "最低价"),
            fmt::format(comment_clause, table_name_, "volume", "数量"),
            fmt::format(comment_clause, table_name_, "turnover", "成交金额"),
            fmt::format(comment_clause, table_name_, "open_interest", "持仓量"),
            fmt::format(comment_clause, table_name_, "close_price", "今收盘"),
            fmt::format(comment_clause, table_name_, "settlement_price", "本次结算价"),
            fmt::format(comment_clause, table_name_, "upperlimit_price", "涨停板价"),
            fmt::format(comment_clause, table_name_, "lowerlimit_price", "跌停板价"),
            fmt::format(comment_clause, table_name_, "pre_delta", "昨虚实度"),
            fmt::format(comment_clause, table_name_, "curr_delta", "今虚实度"),
            fmt::format(comment_clause, table_name_, "update_time", "最后修改时间"),
            fmt::format(comment_clause, table_name_, "update_millisec", "最后修改毫秒"),
            fmt::format(comment_clause, table_name_, "bid_price1", "申买价一"),
            fmt::format(comment_clause, table_name_, "bid_volume1", "申买量一"),
            fmt::format(comment_clause, table_name_, "ask_price1", "申卖价一"),
            fmt::format(comment_clause, table_name_, "ask_volume1", "申卖量一"),
            fmt::format(comment_clause, table_name_, "bid_price2", "申买价二"),
            fmt::format(comment_clause, table_name_, "bid_volume2", "申买量二"),
            fmt::format(comment_clause, table_name_, "ask_price2", "申卖价二"),
            fmt::format(comment_clause, table_name_, "ask_volume2", "申卖量二"),
            fmt::format(comment_clause, table_name_, "bid_price3", "申买价三"),
            fmt::format(comment_clause, table_name_, "bid_volume3", "申买量三"),
            fmt::format(comment_clause, table_name_, "ask_price3", "申卖价三"),
            fmt::format(comment_clause, table_name_, "ask_volume3", "申卖量三"),
            fmt::format(comment_clause, table_name_, "bid_price4", "申买价四"),
            fmt::format(comment_clause, table_name_, "bid_volume4", "申买量四"),
            fmt::format(comment_clause, table_name_, "ask_price4", "申卖价四"),
            fmt::format(comment_clause, table_name_, "ask_volume4", "申卖量四"),
            fmt::format(comment_clause, table_name_, "bid_price5", "申买价五"),
            fmt::format(comment_clause, table_name_, "bid_volume5", "申买量五"),
            fmt::format(comment_clause, table_name_, "ask_price5", "申卖价五"),
            fmt::format(comment_clause, table_name_, "ask_volume5", "申卖量五"),
            fmt::format(comment_clause, table_name_, "average_price", "当日均价"),
            fmt::format(comment_clause, table_name_, "action_day", "业务日期"),
            fmt::format(comment_clause, table_name_, "instrument_id", "合约代码"),
            fmt::format(comment_clause, table_name_, "exchange_inst_id", "合约在交易所的代码"),
            fmt::format(comment_clause, table_name_, "banding_upperprice", "上带价"),
            fmt::format(comment_clause, table_name_, "banding_lowerprice", "下带价")
    };
    std::string create_clause = "CREATE TABLE IF NOT EXISTS {} ("
                                "id serial8,                     "
                                "trading_day varchar(16),        "
                                "reserve1    varchar(32),        "
                                "exchange_id varchar(16),        "
                                "reserve2    varchar(32),        "
                                "last_price  float8,             "
                                "pre_settlement_price float8,    "
                                "pre_close_price float8,         "
                                "pre_open_interest float8,       "
                                "open_price float8,              "
                                "highest_price float8,           "
                                "lowest_price float8,            "
                                "volume int4,                    "
                                "turnover float8,                "
                                "open_interest float8,           "
                                "close_price float8,             "
                                "settlement_price float8,        "
                                "upperlimit_price float8,        "
                                "lowerlimit_price float8,        "
                                "pre_delta float8,               "
                                "curr_delta float8,              "
                                "update_time varchar(16),        "
                                "update_millisec int4,           "
                                "bid_price1 float8,              "
                                "bid_volume1 int4,               "
                                "ask_price1 float8,              "
                                "ask_volume1 int4,               "
                                "bid_price2 float8,              "
                                "bid_volume2 int4,               "
                                "ask_price2 float8,              "
                                "ask_volume2 int4,               "
                                "bid_price3 float8,              "
                                "bid_volume3 int4,               "
                                "ask_price3 float8,              "
                                "ask_volume3 int4,               "
                                "bid_price4 float8,              "
                                "bid_volume4 int4,               "
                                "ask_price4 float8,              "
                                "ask_volume4 int4,               "
                                "bid_price5 float8,              "
                                "bid_volume5 int4,               "
                                "ask_price5 float8,              "
                                "ask_volume5 int4,               "
                                "average_price float8,           "
                                "action_day varchar(16),         "
                                "instrument_id varchar(128),     "
                                "exchange_inst_id varchar(128),  "
                                "banding_upperprice varchar(128),"
                                "banding_lowerprice varchar(128)"
                                ")";
    tx_->exec(fmt::format(create_clause, table_name_));
    for (std::string &comment: comments) {
        tx_->exec(comment);
    }
    tx_->commit();
    return STATUS_OK;
}

int lueing::Ctp2Pg::Sink(std::vector<std::string> &instruments, int delay_ms) {
    // subscribe all instruments
    for (auto &instrument: instruments) {
        ctp_->SubscribeMarketData(instrument, "sink");
    }

    // init position
    for (std::string &instrument: instruments) {
        position_.emplace(instrument, 0);
    }

    // wait for data with timer_
    timer_.async_wait(std::bind(&Ctp2Pg::Sink_, this, delay_ms));
    return STATUS_OK;
}

void lueing::Ctp2Pg::Sink_(int delay_ms) {
    {
        std::unique_lock<std::mutex> lock(ctp_->MarketDataLock());
        for (auto &kv: ctp_->MarketData()) {
            int p = position_[kv.first];
            for (; p < static_cast<int>(kv.second.size()); ++p) {
                // write a record
                tx_->exec_prepared(PREPARED_INSERT_CLAUSE,
                                   kv.second[p].TradingDay,
                                   kv.second[p].reserve1,
                                   kv.second[p].ExchangeID,
                                   kv.second[p].reserve2,
                                   kv.second[p].LastPrice,
                                   kv.second[p].PreSettlementPrice,
                                   kv.second[p].PreClosePrice,
                                   kv.second[p].PreOpenInterest,
                                   kv.second[p].OpenPrice,
                                   kv.second[p].HighestPrice,
                                   kv.second[p].LowestPrice,
                                   kv.second[p].Volume,
                                   kv.second[p].Turnover,
                                   kv.second[p].OpenInterest,
                                   kv.second[p].ClosePrice,
                                   kv.second[p].SettlementPrice,
                                   kv.second[p].UpperLimitPrice,
                                   kv.second[p].LowerLimitPrice,
                                   kv.second[p].PreDelta,
                                   kv.second[p].CurrDelta,
                                   kv.second[p].UpdateTime,
                                   kv.second[p].UpdateMillisec,
                                   kv.second[p].BidPrice1,
                                   kv.second[p].BidVolume1,
                                   kv.second[p].AskPrice1,
                                   kv.second[p].AskVolume1,
                                   kv.second[p].BidPrice2,
                                   kv.second[p].BidVolume2,
                                   kv.second[p].AskPrice2,
                                   kv.second[p].AskVolume2,
                                   kv.second[p].BidPrice3,
                                   kv.second[p].BidVolume3,
                                   kv.second[p].AskPrice3,
                                   kv.second[p].AskVolume3,
                                   kv.second[p].BidPrice4,
                                   kv.second[p].BidVolume4,
                                   kv.second[p].AskPrice4,
                                   kv.second[p].AskVolume4,
                                   kv.second[p].BidPrice5,
                                   kv.second[p].BidVolume5,
                                   kv.second[p].AskPrice5,
                                   kv.second[p].AskVolume5,
                                   kv.second[p].AveragePrice,
                                   kv.second[p].ActionDay,
                                   kv.second[p].InstrumentID,
                                   kv.second[p].ExchangeInstID,
                                   kv.second[p].BandingUpperPrice,
                                   kv.second[p].BandingLowerPrice);
            }
            position_[kv.first] = p;
        }
        tx_->commit();
    }

    timer_.expires_at(timer_.expiry() + boost::asio::chrono::milliseconds(std::max(50, delay_ms)));
    timer_.async_wait(std::bind(&Ctp2Pg::Sink_, this, delay_ms));
}
