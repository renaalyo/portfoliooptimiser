#include "pricing/price_simulator.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <numeric>
#include <random>

double PriceSimulator::calculateMeanVolatility(const std::vector<double>& variances){
    if (variances.empty()) return 0.0;
    double mv = std::accumulate(variances.begin(), variances.end(), 0.0);
    return mv / variances.size(); 
}

std::vector<double> PriceSimulator::calculateHistoricalVariances(
    const std::vector<double>& returns,
    const GARCHparams& params   
){
    std::vector<double> variances(returns.size(), 0.0);
    if (returns.empty()) return variances;

    //variances[0] = returns[0] * returns[0];
    variances[0] = params.omega / (1 - params.alpha - params.beta);

    for(size_t i = 1; i < returns.size(); i++){
        variances[i] = params.omega +
                       params.alpha * returns[i - 1] * returns[i - 1] +
                       params.beta * variances[i - 1];
        //CAREFULLY               
    }
    return variances;
}

double PriceSimulator::calculateCurrentVolatility(
    const std::vector<double>& returns,
    const GARCHparams& params
){
    if (returns.empty()) return 0.0;

    double variance = params.omega / (1 - params.alpha - params.beta);
    

    for(size_t i = 1; i < returns.size(); i++){
        variance = params.omega +
                       params.alpha * returns[i - 1] * returns[i - 1] +
                       params.beta * variance;
        //CAREFULLY               
    }
    return std::sqrt(variance);
}

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
        int numDays
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

std::vector<std::vector<double>> PriceSimulator::monteCarloGBM(
    double currentPrice,
    double mu,
    double volatility,
    int numDays,
    int simulationsCount
){
    std::vector<std::vector<double>> allSimulations(simulationsCount);

    for (int i = 0; i < simulationsCount; i++)
    {
        allSimulations[i] = geometricBrownianMotion(
            currentPrice,
            mu,
            volatility,
            numDays
        );
    }
    return allSimulations;
}