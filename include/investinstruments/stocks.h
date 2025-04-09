#pragma once

#include "investinstrument.h"
#include <cmath>

class Stock : public InvestInstrument{
private:
    string sector_;
    double dividend_yield_;
    std::vector<double> historicalReturns_;
    PriceSimulator::GARCHparams garchParams_;
public:
    
    void setSector(const string& sector){
        sector_ = sector;
    }

    void loadHistoricalData(const string& filename);
    double setHistoricalMuParam();
    void setGARCHParams(double omega, double alpha, double beta);
    void simulatePrice(int days);
    
    std::vector<double> getPriceSimulation(int days) override;
    std::vector<std::vector<double>> getMCSimulations(int days, int simulationsCount) override;
    std::vector<std::vector<double>> getNEWMCSimulations(int days, int simulationsCount) override;
};