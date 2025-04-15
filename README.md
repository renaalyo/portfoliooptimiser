GBM Monte Carlo Simulator with GARCH Volatility Modeling

Overview
This program predicts future stock prices using a Geometric Brownian Motion (GBM) model enhanced with GARCH volatility parameters. It combines C++ for high-performance computations and Python for data visualization, with plans to expand into a full Qt-based GUI application.

Key Features
1. CORE ENGINE (C++)

GBM Simulations: Models stock price paths via Monte Carlo, incorporating drift (expected return) and stochastic volatility.
GARCH(1,1) Integration: Dynamically adjusts volatility based on historical returns (ω, α, β parameters).

2. DATA PIPELINE

Input: Historical stock prices (CSV/API) → Converted to log-returns for GARCH calibration.
Output: Forecasted price distributions (mean, median, 5%/95% quantiles) saved to CSV.

3. VISUALISATION (Python)

Matplotlib/Seaborn: Generates interactive plots of simulated trajectories, volatility clusters, and confidence intervals.
Statistical Metrics: Computes skewness, kurtosis, and Jarque-Bera tests for normality checks.

4. PLANNED UPGRADES

Qt GUI: User-friendly interface to adjust parameters (e.g., forecast horizon, GARCH coefficients) and visualize results in real time.
Automated Testing: GoogleTest (gtest) integration for validating model accuracy and edge cases.
Performance Boost: Hybrid parallelization (OpenMP + MPI) for distributed computing on multi-node systems.
