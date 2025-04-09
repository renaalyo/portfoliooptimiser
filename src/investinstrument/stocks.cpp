#include "investinstruments/stocks.h"
#include <iostream>

void Stock::loadHistoricalData(const string& filename){
    historicalReturns_ = simulator_.loadLogReturns(filename);
}

void Stock::setGARCHParams(double omega, double alpha, double beta){
    garchParams_.omega = omega;
    garchParams_.alpha = alpha;
    garchParams_.beta = beta;
}

double Stock::setHistoricalMuParam(){
    // cout << "Historical" << "\n";
    // for (const auto& el : historicalReturns_){
    //     cout << "Historical" << el << "\n";
    // }
    return simulator_.calculateMU(historicalReturns_);
}

void Stock::simulatePrice(int days){
    if (historicalReturns_.empty()){
        std::cout << "Historical data not loaded";
    }

    auto variances = this->simulator_.calculateHistoricalVariances(
        historicalReturns_, 
        garchParams_
    );

    std::vector<double> volatilities(variances.size());

    for(size_t i = 0; i < volatilities.size(); i++){
        volatilities[i] = std::sqrt(variances[i]);
    }

    double meanVol = this->simulator_.calculateMeanVolatility(volatilities);

    std::vector<double> simulatedPrices = this->simulator_.geometricBrownianMotion(
        this->getPrice(),
        this->getExpectedReturn(), 
        meanVol,
        days
    );

    this->setPrice(simulatedPrices.back());  
}

std::vector<double> Stock::getPriceSimulation(int days) {
    auto variances = this->simulator_.calculateHistoricalVariances(
        historicalReturns_, 
        garchParams_);

    std::vector<double> volatilities(variances.size());
    for (size_t i = 0; i < variances.size(); ++i) {
        volatilities[i] = std::sqrt(variances[i]);
    }
    
    double meanVol = simulator_.calculateMeanVolatility(volatilities);

    return this->simulator_.geometricBrownianMotion(
        this->getPrice(),
        this->getExpectedReturn(),
        meanVol,
        days
    );
}

std::vector<std::vector<double>> Stock::getMCSimulations(int days, int simulationsCount){
    auto variances = this->simulator_.calculateHistoricalVariances(
        historicalReturns_, 
        garchParams_);

    std::vector<double> volatilities(variances.size());
    for (size_t i = 0; i < variances.size(); ++i) {
        volatilities[i] = std::sqrt(variances[i]);
    }
    
    double meanVol = simulator_.calculateMeanVolatility(volatilities);
    std::cout << "Current Volatility: " << meanVol << "\n";
    return this->simulator_.monteCarloGBM(
        this->getPrice(),
        this->getExpectedReturn(),
        meanVol,
        days,
        simulationsCount
    );
}

std::vector<std::vector<double>> Stock::getNEWMCSimulations(int days, int simulationsCount){
    double vol = simulator_.calculateCurrentVolatility(historicalReturns_, garchParams_);
    std::cout << "Current Volatility: " << vol << "\n";
    return this->simulator_.monteCarloGBM(
        this->getPrice(),
        this->getExpectedReturn(),
        vol,
        days,
        simulationsCount
    );
}