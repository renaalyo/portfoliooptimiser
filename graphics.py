import sys
import pandas as pd
import matplotlib.pyplot as plt

def plot_stock_prices(csv_file):
    """Строит график цен акций из CSV файла"""
    try:
        # Чтение данных с проверкой формата
        df = pd.read_csv(csv_file)
        
        if 'Day' not in df.columns or 'Price' not in df.columns:
            print("Ошибка: Файл должен содержать колонки 'Day' и 'Price'")
            return -1
        
        # Создание графика
        plt.figure(figsize=(12, 6))
        plt.plot(df['Day'], df['Price'], linewidth=2, color='steelblue')
        
        # Оформление
        plt.title(f'Stock Price Simulation ({len(df)} trading days)')
        plt.xlabel('Trading Day', fontsize=12)
        plt.ylabel('Price ($)', fontsize=12)
        plt.grid(True, linestyle='--', alpha=0.6)
        
        # Сохранение и показ
        output_file = csv_file.replace('.csv', '_chart.png')
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        print(f"График сохранен как {output_file}")
        plt.show()
        
        return 0
        
    except FileNotFoundError:
        print(f"Ошибка: Файл {csv_file} не найден")
        return -1
    except Exception as e:
        print(f"Неизвестная ошибка: {str(e)}")
        return -1


if __name__ == "__main__":
    if len(sys.argv) > 1:
        csv_file = sys.argv[1]
        retcode = plot_stock_prices(csv_file)
        sys.exit(retcode)
    else:
        print("Укажите CSV файл с данными (например: prices.csv)")
        print("Пример использования: python plot_prices.py stock_prices.csv")
        sys.exit(1)