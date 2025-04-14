#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

namespace RiskAnalyzer{
    double calculatePercentile(const std::vector<double>& prices, double percentile){
        if (prices.empty()) return 0.0;

        std::vector<double> sorted(prices);
        std::sort(sorted.begin(), sorted.end());
        size_t index = static_cast<size_t>(percentile * (sorted.size() - 1));
        return sorted[index];
    }

    double calculateVaR(const std::vector<double>& prices, double confidence_level = 0.95){
        if (prices.size() < 2) return 0.0;

        std::vector<double> returns;
        returns.reserve(prices.size() - 1);
        for (size_t i = 1; i < prices.size(); i++){
            //CAREFULLY
            returns.push_back((prices[i] - prices[i - 1] )/ prices[i - 1]);
        }

        return -calculatePercentile(returns, 1 - confidence_level);
    }
 
    double calculateCVaR(const std::vector<double>& prices, double confidence_level = 0.95){
        if (prices.size() < 2) return 0.0;

        double var = calculateVaR(prices, confidence_level);
        std::vector<double> tailReturns;

        for (size_t i = 1; i < prices.size(); i++){
            double ret = (prices[i] - prices[i - 1] )/ prices[i - 1];
            if (-ret > var){
                tailReturns.push_back(ret);
            }
        }

        return (tailReturns.empty()) ?
            0.0
            :  
            -std::accumulate(tailReturns.begin(), tailReturns.end(), 0.0) / tailReturns.size();
    }

    void analyzeScenario(const std::vector<double>& prices){
        std::cout << "\nRisk Analysis:\n";
        std::cout << "5th Percentile: $" << calculatePercentile(prices, 0.05) << "\n";
        std::cout << "95th Percentile: $" << calculatePercentile(prices, 0.95) << "\n";
        std::cout << "VaR (95%): " << calculateVaR(prices) * 100 << "%\n";
        std::cout << "CVaR (95%): " << calculateCVaR(prices) * 100 << "%\n";
    }
    
    double calculateMean(const std::vector<double>& last_day_prices){
        if (last_day_prices.size() < 1) return 0.0;
        double mean = accumulate(last_day_prices.begin(), last_day_prices.end(), 0.0) / last_day_prices.size();
        return mean;
    }

    double calculateStDev(const std::vector<double>& last_day_prices){
        if (last_day_prices.size() < 1) return 0.0;
        double mean = calculateMean(last_day_prices);
        double sq_sum = inner_product(last_day_prices.begin(), last_day_prices.end(), last_day_prices.begin(), 0.0);
        double stdev = sqrt(sq_sum / last_day_prices.size() - mean * mean);
        return stdev;
    }
    void showMetrics(const std::vector<std::vector<double>>& simulations){
        std::vector<double> last_day_prices;
        for(const auto& sim : simulations){
            last_day_prices.push_back(sim.back());
        }
        //Mean
        double mean = calculateMean(last_day_prices);
        // Standard Deviation
        double stdev = calculateStDev(last_day_prices);
        // Median
        sort(last_day_prices.begin(), last_day_prices.end());
        double median = last_day_prices[last_day_prices.size() / 2];
    
        // Percentile (5% и 95%)
        double q5 = calculatePercentile(last_day_prices, 5);
        double q95 = calculatePercentile(last_day_prices, 95);;
        //Quantiles((5% и 95%))
        //double q5 = last_day_prices[last_day_prices.size() * 0.05];
        //double q95 = last_day_prices[last_day_prices.size() * 0.95];
    
        std::cout << "mean: " << mean << "\n";
        std::cout << "median: " << median << "\n";
        std::cout << "stdev: " << stdev << "\n";
        std::cout << "Percentile5: " << q5 << "\n";
        std::cout << "Percentile95: " << q95 << "\n";
        //std::cout << "VaR (95%): " << calculateVaR(last_day_prices) * 100 << "%\n";
        //std::cout << "CVaR (95%): " << calculateCVaR(last_day_prices) * 100 << "%\n";
    }
}