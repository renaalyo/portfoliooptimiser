#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <memory>
#include <fstream>

#include "investinstruments/stocks.h"
#include "investinstruments/stockBuilder.h"
#include "pricing/price_simulator.h"
#include "pricing/risk_analysis.h"  
#include "portfolio.h"

using namespace std;

void writePriceToCSF(const std::vector<double>& prices, const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }
    
    // Записываем заголовок
    file << "Day,Price\n";
    
    // Записываем данные
    for (size_t day = 0; day < prices.size(); ++day) {
        file << day + 1 << "," << prices[day] << "\n";
    }
    
    std::cout << "Successfully wrote " << prices.size() << " prices to " << filename << std::endl;
}

void writePricesToCSV(const std::vector<std::vector<double>>& simulations, const std::string& filename){
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

int main(){
    auto portfolio = make_unique<Portfolio>(1000.0);
    auto stockApple = StockBuilder()
        .setName("Tesla")
        .setTicker("TSLA")
        .setPrice(248)
        .Build();
    
    stockApple->loadHistoricalData("TSLA_returns.csv");

    stockApple->setGARCHParams(0.08, 0.1, 0.85);
    
    double mu = stockApple->setHistoricalMuParam();
    cout << "MU:" << mu << "\n";
    stockApple->setExpectedReturn(mu);

    cout << "\nBefore price simulation..." << endl;
    portfolio->addInstrument(move(stockApple));
    portfolio->printPortfolio();

    // Симуляция цены
    cout << "\nRunning price simulation..." << endl;

    // Получение результатов симуляции
    const int simulationDays = 252;  // 1 торговый год
    //auto simulatedPrice = portfolio->getInstruments()[0]->getPriceSimulation(simulationDays);
    
    // Вывод результатов
    // cout << "\nSimulation results:" << endl;
    // cout << "Simulated price after " << simulationDays << " days: $" 
    //      << simulatedPrice.back() << endl;
    //writePriceToCSF(simulatedPrice, "stocks.csv");
    //RiskAnalysis::analyzeRisk(simulatedPrice);
    int simulationCount = 100;
    auto simulatedMCPrices = portfolio->getInstruments()[0]->getMCSimulations(simulationDays, simulationCount);
    writePricesToCSV(simulatedMCPrices, "stocks.csv");
    return 0;
}