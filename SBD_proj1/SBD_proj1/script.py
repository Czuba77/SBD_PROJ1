import csv
import math
import matplotlib.pyplot as plt

# Plik z danymi
filename = "exp.csv"

b = 10
n = 11

# Listy do wykresu
N_values = []
exp_ops = []
theory_ops = []

# Odczyt CSV
with open(filename, newline="", encoding="utf-8") as f:
    reader = csv.DictReader(f)
    for row in reader:
        ops = int(row["operacje"])
        N = int(row["rekordy"])

        N_values.append(N)
        exp_ops.append(ops)

        theory=2 * (N / b) * (math.log(N / b) / math.log(n))

        theory_ops.append(theory)

# Rysowanie wykresu
plt.figure(figsize=(10,6))
plt.plot(N_values, exp_ops, marker="o", label="Dane eksperymentalne")
plt.plot(N_values, theory_ops, marker="x", label="Wartości teoretyczne")

plt.xlabel("Liczba rekordów N")
plt.ylabel("Koszt operacji")
plt.title("Porównanie wyników eksperymentalnych z teorią")
plt.legend()
plt.grid(True)
plt.show()