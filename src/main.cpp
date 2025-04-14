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
    
    double mu = stockApple->calculateHistoricalMuParam();
    cout << "MU:" << mu << "\n";
    stockApple->setExpectedReturn(mu);

    cout << "\nBefore price simulation..." << endl;
    portfolio->addInstrument(move(stockApple));
    portfolio->printPortfolio();

    // Симуляция цены
    cout << "\nRunning price simulation..." << endl;

    // Получение результатов симуляции
    const int simulationDays = 252;  // 1 торговый год

    int simulationCount = 1000;
    auto simulatedMCPrices = portfolio->getInstruments()[0]->getMCSimulations(simulationDays, simulationCount);
    stockApple->writePriceToCSV(simulatedMCPrices, "stocks.csv");
    //RiskAnalyzer::showMetrics(simulatedMCPrices);
    return 0;
}