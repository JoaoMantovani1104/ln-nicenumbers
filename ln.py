import numpy as np
import matplotlib.pyplot as plt

def plot_error_from_file(filename):
    x_values = []
    exact_values = []
    approx_values = []
    error_values = []

    with open(filename, 'r') as file:
        next(file)  # Ignorar o cabeçalho
        for line in file:
            parts = line.strip().split(',')
            if len(parts) == 4:  # Garantir que a linha contém exatamente 4 colunas
                try:
                    x, exact, approx, error = map(float, parts)
                    x_values.append(x)
                    exact_values.append(exact)
                    approx_values.append(approx)
                    error_values.append(error)
                except ValueError:
                    continue  # Ignorar linhas com valores inválidos

    if not x_values:
        print("Nenhum dado válido encontrado no arquivo.")
        return

    # Plotagem do gráfico
    plt.figure(figsize=(10, 6))
    plt.plot(x_values, error_values, label="Erro absoluto", color="red")
    plt.title("Erro absoluto entre ln(x) exato e aproximado")
    plt.xlabel("x")
    plt.ylabel("Erro absoluto")
    plt.grid(True)
    plt.legend()
    plt.show()

# Nome do arquivo
filename = "log_results.csv"
plot_error_from_file(filename)
