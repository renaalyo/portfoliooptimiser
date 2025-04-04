#include "pricing/price_simulator.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <numeric>
#include <random>

double PriceSimulator::calculateMeanVolatility(const std::vector<double>& volatilities){
    if (volatilities.empty()) return 0.0;
    double mv = std::accumulate(volatilities.begin(), volatilities.end(), 0.0);
    return mv / volatilities.size(); 
}

std::vector<double> PriceSimulator::calculateHistoricalVolatility(
    const std::vector<double>& returns,
    const GARCHparams& params   
){
    std::vector<double> variances(returns.size(), 0.0);
    if (returns.empty()) return variances;

    variances[0] = returns[0] * returns[0];
    //variances[0] = params.omega / (1 - params.alpha - params.beta);

    for(size_t i = 1; i < returns.size(); i++){
        variances[i] = params.omega +
                       params.alpha * returns[i - 1] * returns[i - 1] +
                       params.beta * variances[i - 1];
        //CAREFULLY               
    }
    return variances;
}

std::vector<double> PriceSimulator::loadLogReturns(const std::string& filename){
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
};


double PriceSimulator::calculateMU(const std::vector<double>& logReturns){
    if (logReturns.size() < 2) return 0.09;

    double sum_log_returns = std::accumulate(
        logReturns.begin(), 
        logReturns.end(),
        0.0
    );

    double mean_daily_return = sum_log_returns / logReturns.size();
    return mean_daily_return * 252;
}

double PriceSimulator::singleGeometricBrownianMotion(
    double currentPrice,
    double mu,
    double volatility,
    double dt
){

    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::normal_distribution<double> jumpDist(0.0, 0.02);
    static std::uniform_real_distribution<double> probDist(0.0, 1.0);

    std::normal_distribution<double> normDist(0.0, 1.0);
    double Z = normDist(rng);

    double daily_return = exp(
        (mu - 0.5 * volatility * volatility) * dt 
        + volatility * sqrt(dt) * Z
    );

    //Jump Diffusion
    if (probDist(rng) < 0.01) { // 1% chance
        double jump = jumpDist(rng); // ±5% jump
        daily_return *= exp(jump);
    }
    
    return currentPrice * daily_return;
}

std::vector<double> PriceSimulator::geometricBrownianMotion(
        double currentPrice,
        double mu,
        double volatility,
        double numDays
){
    std::vector<double> prices(numDays);
    if (numDays == 0) return prices;

    const double dt = 1.0 / 252.0;
    prices[0] = currentPrice;

    //CAREFULL numDays > volatilities.size() 
    for(int i = 1; i < numDays; ++i){
        prices[i] = singleGeometricBrownianMotion(
            prices[i-1] 
            ,mu
            ,volatility
            ,dt
        );
    }
    return prices;
}