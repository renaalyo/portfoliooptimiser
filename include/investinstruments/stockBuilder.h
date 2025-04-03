#pragma once

#include "stocks.h"
#include <memory>

class StockBuilder{
public:
    friend class Stock;
    StockBuilder& setSector(string sector){
        sector_ = sector;
        return *this;
    }
    StockBuilder& setName(string name){
        name_ = name;
        return *this;
    }
    StockBuilder& setTicker(string ticker){
        ticker_ = ticker;
        return *this;
    }
    StockBuilder& setExpectedReturn(double expected_return){
        expected_return_ = expected_return;
        return *this;
    }
    StockBuilder& setPrice(double price){
        price_ = price;
        return *this;
    }

    unique_ptr<Stock> Build() const{
        auto stock = make_unique<Stock>();
        stock->setName(name_);
        stock->setTicker(ticker_);
        stock->setSector(sector_);
        stock->setExpectedReturn(expected_return_);
        stock->setPrice(price_);
        return stock;
    }
private:
    string name_;
    string ticker_;
    string sector_;
    double expected_return_;
    double price_;
};