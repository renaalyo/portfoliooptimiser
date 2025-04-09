import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def load_and_plot_simulations(csv_file):
    # Чтение CSV с указанием правильного разделителя и обработкой формата
    try:
        # Сначала читаем как обычный текст
        with open(csv_file, 'r') as f:
            lines = f.readlines()
        
        # Обрабатываем заголовок
        header = lines[0].strip().split(',')
        
        # Обрабатываем строки данных
        data = []
        for line in lines[1:]:
            parts = line.strip().split(',')
            
            prices = list(map(float, parts[2:]))  # Цены
            data.append(prices)
        
        # Создаем DataFrame
        df = pd.DataFrame(data).T
        
    except Exception as e:
        print(f"Ошибка при чтении файла: {e}")
        return

    colors = plt.cm.gist_ncar(np.linspace(0, 1, 1000))
    
    # Построение графика
    plt.figure(figsize=(15, 8), dpi=100)
    
    for i, col in enumerate(df.columns):
        plt.plot(df[col], color=colors[i], alpha=0.6, linewidth=1.2)

    plt.title('Monte Carlo Price Simulations', fontsize=14, pad=20)
    plt.xlabel('Day', fontsize=12)
    plt.ylabel('Price', fontsize=12)
    plt.grid(True, linestyle=':', alpha=0.6)

    # Сохранение и показ
    output_file = csv_file.replace('.csv', '_chart.png')
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"График сохранен как {output_file}")
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    csv_file = "Debug/stocks.csv"  # Укажите ваш путь
    load_and_plot_simulations(csv_file)