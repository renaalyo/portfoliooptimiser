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
#include "portfolio.h"

using namespace std;

void writePricesToCSF(const std::vector<double>& prices, const std::string& filename) {
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

int main(){
    auto portfolio = make_unique<Portfolio>(1000.0);
    auto stockApple = StockBuilder()
        .setName("Tesla")
        .setTicker("TSLA")
        .setPrice(191)
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
    portfolio->simulateMonteCarlo();

    // Получение результатов симуляции
    const int simulationDays = 252;  // 1 торговый год
    auto simulatedPrices = portfolio->getInstruments()[0]->getPriceSimulation(simulationDays);
    
    // Вывод результатов
    cout << "\nSimulation results:" << endl;
    cout << "Simulated price after " << simulationDays << " days: $" 
         << simulatedPrices.back() << endl;
    //writePricesToCSF(simulatedPrices, "stocks.csv");
    return 0;
}