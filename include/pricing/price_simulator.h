#pragma once

#include <vector>
#include <string>

class PriceSimulator{
public:
    struct GARCHparams{
        double omega;
        double alpha;
        double beta;
    };

    std::vector<double> loadLogReturns(const std::string& filename);

    double calculateMeanVolatility(const std::vector<double>& volatilities);
    std::vector<double> calculateHistoricalVolatility(
        const std::vector<double>& returns,
        const GARCHparams& params
    );

    double calculateMU(const std::vector<double>& prices);
    double singleGeometricBrownianMotion(
        double currentPrice,
        double mu,
        double volatility,
        double dt
    );

    std::vector<double> geometricBrownianMotion(
        double currentPrice,
        double mu,
        double volatility,
        double numDays
    );
};

