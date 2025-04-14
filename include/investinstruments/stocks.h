#pragma once

#include "investinstrument.h"
#include <cmath>
#include <fstream>
#include <sstream>

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

    double calculateHistoricalMuParam();
    void setGARCHParams(double omega, double alpha, double beta);
    
    void loadHistoricalData(const string& filename);
    std::vector<double> loadLogReturns(const std::string& filename);
    void writePriceToCSV(const std::vector<std::vector<double>>& simulations, const std::string& filename);
    
    void simulatePrice(int days);
    std::vector<double> getPriceSimulation(int days) override;
    std::vector<std::vector<double>> getMCSimulations(int days, int simulationsCount) override;
    std::vector<std::vector<double>> getNEWMCSimulations(int days, int simulationsCount) override;
};