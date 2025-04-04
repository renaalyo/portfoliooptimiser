#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

namespace RiskAnalysis{
    // void logVector(const std::string& name, const std::vector<double>& vec) {
    //     std::cout << name << " (size: " << vec.size() << "): [";
    //     for (size_t i = 0; i < vec.size(); ++i) {
    //         std::cout << vec[i];
    //         if (i != vec.size() - 1) std::cout << ", ";
    //     }
    //     if (vec.size()) std::cout << ", ...";
    //     std::cout << "]\n";
    // }

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

        //logVector("Returns", returns);

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

        //logVector("tailReturns", tailReturns);
    
        return (tailReturns.empty()) ?
            0.0
            :  
            -std::accumulate(tailReturns.begin(), tailReturns.end(), 0.0) / tailReturns.size();
    }

    void analyzeRisk(const std::vector<double>& prices){
        std::cout << "\nRisk Analysis:\n";
        std::cout << "5th Percentile: $" << calculatePercentile(prices, 0.05) << "\n";
        std::cout << "95th Percentile: $" << calculatePercentile(prices, 0.95) << "\n";
        std::cout << "VaR (95%): " << calculateVaR(prices) * 100 << "%\n";
        std::cout << "CVaR (95%): " << calculateCVaR(prices) * 100 << "%\n";
    }
}