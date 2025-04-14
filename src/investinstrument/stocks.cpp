#include "investinstruments/stocks.h"
#include <iostream>

void Stock::loadHistoricalData(const string& filename){
    historicalReturns_ = loadLogReturns(filename);
}

std::vector<double> Stock::loadLogReturns(const std::string& filename){
    std::vector<double> logReturns;
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return logReturns;  // Пустой вектор если файл не открылся
    }

    std::string line;
    bool dataSection = false;

    while(std::getline(file,line)){
        if (!dataSection) {
            if (line.find("Date") != std::string::npos) {
                dataSection = true;
            }
            continue;
        }

        // Разбиваем строку на части
        size_t last_comma = line.find_last_of(',');
        if (last_comma == std::string::npos || last_comma == line.size() - 1) {
            continue;  // Пропускаем строки без данных
        }

        // Пробуем преобразовать в число
        std::string value_str = line.substr(last_comma + 1);
        double value;
        if (std::istringstream(value_str) >> value) {
            logReturns.push_back(value);
        }
    }

    // std::cout << "logReturns" << "\n";
    // for (const auto& el : logReturns){
    //     std::cout << "logReturns" << el << "\n";
    // }
    return logReturns;
}

void Stock::writePriceToCSV(const std::vector<std::vector<double>>& simulations, const std::string& filename){
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    // Записываем заголовок
    file << "Day,Simulations,Price\n";

    size_t num_days = simulations[0].size();
    for (const auto& sim : simulations) {
        if (sim.size() != num_days) {
            std::cerr << "Error: Inconsistent number of days across simulations." << std::endl;
            return;
        }
    }
    
    // Записываем заголовок (дни в первой строке)
    // file << "Simulation\\Day";
    // for (size_t day = 0; day < num_days; ++day) {
    //     file << "," << day + 1;
    // }
    // file << "\n";
    
    // Записываем данные (каждая строка — одна симуляция)
    for (size_t i = 0; i < simulations.size(); ++i) {
        file << "Sim " << i + 1;  // Номер симуляции
        for (double price : simulations[i]) {
            file << "," << price;
        }
        file << "\n";
    }

    std::cout << "Successfully wrote " << simulations.size() 
                << " simulations (" << num_days << " days each) to " 
                << filename << std::endl;
    
}

void Stock::setGARCHParams(double omega, double alpha, double beta){
    garchParams_.omega = omega;
    garchParams_.alpha = alpha;
    garchParams_.beta = beta;
}

double Stock::calculateHistoricalMuParam(){
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