#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include "investinstruments/investinstrument.h"

using namespace std;

class Portfolio{
private:
    double budget_;
    vector<unique_ptr<InvestInstrument>> instruments; 

public:
    Portfolio(double budget) : budget_(budget){}

    const vector<unique_ptr<InvestInstrument>>& getInstruments() const {
        return instruments;
    }

    vector<unique_ptr<InvestInstrument>>& getInstruments() {
        return instruments;
    }

    void addInstrument(unique_ptr<InvestInstrument> inv){
        if (!inv) return;
        double price = inv->getPrice();
        if (price > budget_){
            cout << "No money " << price <<  endl;
            return;
        }
        instruments.push_back(std::move(inv));
        budget_ -= price;
    };

    void printPortfolio() const{
        cout << "Portfolio:\n";
        for(const auto& instrument : instruments){
            if (!instrument) continue;
            cout << "- " << instrument->getName() 
                 << " (Price: " << instrument->getPrice() << ")\n";
        }
        cout << "Remaining budget: " << budget_ << endl;
    }

    void simulateMonteCarlo() {
        for(auto& instrument : instruments){
            if (instrument){
                instrument->simulatePrice();
            }
        }
    }
};