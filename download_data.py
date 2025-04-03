import sys
import yfinance as yf
import numpy as np


def download_data(ticker):
    try:
        yf.Ticker(ticker).get_info()
    except AttributeError:
        print("No ticker found")
        return -1 
    data = yf.download(ticker, start="2020-01-01", end="2023-12-31")
    
    # Расчет логарифмической доходности
    data['Log_Return'] = np.log(data['Close'] / data['Close'].shift(1)).dropna()
    
    # Сохранение в CSV
    data[['Close', 'Log_Return']].to_csv(f"{ticker}_returns.csv")
    
    print(f"Данные для {ticker} сохранены в {ticker}_returns.csv")


if __name__ == "__main__":
    if len(sys.argv) > 1:
        ticker = sys.argv[1]
        download_data(ticker)
    else:
        print("Укажите тикер акции (например: AAPL)")