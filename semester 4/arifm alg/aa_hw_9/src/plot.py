#!/Users/stepanorlov/env/bin/python3 
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

try:
    data = pd.read_csv('complexity_data.csv')
except FileNotFoundError:
    print("Error: complexity_data.csv not found. Please run the C++ program first.")
    sys.exit(1)

fig, axes = plt.subplots(2, 3)
manager = plt.get_current_fig_manager()
manager.resize(2560, 1564)

axes[0, 0].plot(data['precision'], data['pi_time_ms'], 'r-o', linewidth=2, markersize=4)
axes[0, 0].set_xlabel('Точность (количество знаков)', fontsize=12)
axes[0, 0].set_ylabel('Время (мс)', fontsize=12)
axes[0, 0].set_title('Время вычисления числа π', fontsize=14)
axes[0, 0].grid(True, alpha=0.3)

axes[0, 1].plot(data['precision'], data['pi_operations'], 'm-o', linewidth=2, markersize=4)
axes[0, 1].set_xlabel('Точность (количество знаков)', fontsize=12)
axes[0, 1].set_ylabel('Количество операций', fontsize=12)
axes[0, 1].set_title('Число арифметических операций для π', fontsize=14)
axes[0, 1].grid(True, alpha=0.3)

axes[0, 2].plot(data['precision'], data['pi_terms'], 'y-o', linewidth=2, markersize=4)
axes[0, 2].set_xlabel('Точность (количество знаков)', fontsize=12)
axes[0, 2].set_ylabel('Число членов ряда', fontsize=12)
axes[0, 2].set_title('Количество членов ряда для π', fontsize=14)
axes[0, 2].grid(True, alpha=0.3)

axes[1, 0].plot(data['precision'], data['e_time_ms'], 'b-o', linewidth=2, markersize=4)
axes[1, 0].set_xlabel('Точность (количество знаков)', fontsize=12)
axes[1, 0].set_ylabel('Время (мс)', fontsize=12)
axes[1, 0].set_title('Время вычисления числа e', fontsize=14)
axes[1, 0].grid(True, alpha=0.3)

axes[1, 1].plot(data['precision'], data['e_operations'], 'g-o', linewidth=2, markersize=4)
axes[1, 1].set_xlabel('Точность (количество знаков)', fontsize=12)
axes[1, 1].set_ylabel('Количество операций', fontsize=12)
axes[1, 1].set_title('Число арифметических операций для e', fontsize=14)
axes[1, 1].grid(True, alpha=0.3)

axes[1, 2].plot(data['precision'], data['e_terms'], 'c-o', linewidth=2, markersize=4)
axes[1, 2].set_xlabel('Точность (количество знаков)', fontsize=12)
axes[1, 2].set_ylabel('Число членов ряда', fontsize=12)
axes[1, 2].set_title('Количество членов ряда для e', fontsize=14)
axes[1, 2].grid(True, alpha=0.3)

plt.tight_layout()
plt.show()