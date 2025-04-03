#pragma once

#include "pricing/price_simulator.h"
#include <string>


using namespace std;

class InvestInstrument{
protected:
    PriceSimulator simulator_;
private:
    string name_;
    string ticker_;
    double expected_return_;
    double price_;
public:
    void setName(const string& name){
        name_ = name;
    }

    void setTicker(const string& ticker){
        ticker_ = ticker;
    }

    void setExpectedReturn(double expected_return){
        expected_return_ = expected_return;
    }

    void setPrice(double price){
        price_ = price;
    }

    string getName() const{
        return name_;
    }
    
    string getTicker() const{
        return ticker_;
    }

    double getExpectedReturn() const{
        return expected_return_;
    }

    double getPrice() const{
        return price_;
    }

    virtual void simulatePrice(){};
    virtual std::vector<double> getPriceSimulation(int days) = 0;
};