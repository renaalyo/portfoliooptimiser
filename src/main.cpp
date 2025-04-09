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

double mean = 0.0;
double sq_sum = 0.0;
double stdev = 0.0;
double median = 0.0;
double q5 = 0.0;
double q95 = 0.0;

void showMetrics(const std::vector<std::vector<double>>& simulations){
    std::vector<double> last_day_prices;
    for(const auto& sim : simulations){
        last_day_prices.push_back(sim.back());
    }
    //Mean
    double mean = accumulate(last_day_prices.begin(), last_day_prices.end(), 0.0) / last_day_prices.size();

    // Standard Deviation
    double sq_sum = inner_product(last_day_prices.begin(), last_day_prices.end(), last_day_prices.begin(), 0.0);
    double stdev = sqrt(sq_sum / last_day_prices.size() - mean * mean);

    // Median
    sort(last_day_prices.begin(), last_day_prices.end());
    double median = last_day_prices[last_day_prices.size() / 2];

    // Quantiles (5% и 95%)
    double q5 = last_day_prices[last_day_prices.size() * 0.05];
    double q95 = last_day_prices[last_day_prices.size() * 0.95];

    std::cout << "mean: " << mean << "\n";
    std::cout << "median: " << median << "\n";
    std::cout << "stdev: " << stdev << "\n";
    std::cout << "q5: " << q5 << "\n";
    std::cout << "q95: " << q95 << "\n";
}

int main(){
    auto portfolio = make_unique<Portfolio>(1000.0);
    auto stockApple = StockBuilder()
        .setName("Apple")
        .setTicker("AAPL")
        .setPrice(191)
        .Build();
    
    //stockApple->loadHistoricalData("TSLA_returns.csv");
    stockApple->loadHistoricalData("AAPL_returns.csv");

    stockApple->setGARCHParams(0.01, 0.05, 0.85);
    
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
    
    //writePriceToCSF(simulatedPrice, "stocks.csv");
    //RiskAnalysis::analyzeRisk(simulatedPrice);
    int simulationCount = 1000;
    auto simulatedMCPrices = portfolio->getInstruments()[0]->getMCSimulations(simulationDays, simulationCount);
    writePricesToCSV(simulatedMCPrices, "stocks.csv");
    showMetrics(simulatedMCPrices);
    return 0;
}