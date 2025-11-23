import csv
import math
import matplotlib.pyplot as plt

# Plik z danymi
filename = "exp.csv"

b = 5
n = 6

# Listy do wykresu
N_values = []
exp_ops = []
theory_ops = []
theorylog10_ops = []
theorylgn1_ops=[]

# Odczyt CSV
with open(filename, newline="", encoding="utf-8") as f:
    reader = csv.DictReader(f)
    for row in reader:
        ops = int(row["operacje"])
        N = int(row["rekordy"])

        N_values.append(N)
        exp_ops.append(ops)

        theory=2 * (N / b) * (math.log(N / b, n))
        theory_lg2 = 2 * (N / (b*math.log2(n))) * (math.log2(N / b))
        theory_lgn1 = 2 * (N / b) * (1+(math.log(N/(n*b),n-1)))

        theory_ops.append(theory)
        theorylog10_ops.append(theory_lg2)
        theorylgn1_ops.append(theory_lg2)
# Rysowanie wykresu
plt.figure(figsize=(10,6))
plt.plot(N_values, exp_ops,  label="Dane eksperymentalne")
plt.plot(N_values, theory_ops, linestyle='-', label="Wartości teoretyczne dla b=5, a n=6")
#plt.plot(N_values, theorylog10_ops,linestyle='--',  label="Wartości teoretyczne wzoru 2")
#plt.plot(N_values, theorylgn1_ops,linestyle='--',  label="Wartości teoretyczne wzoru 3")

plt.xlabel("Liczba rekordów N")
plt.ylabel("Ilość operacji")
plt.title("Porównanie wyników eksperymentalnych z teorią")
plt.legend()
plt.grid(True)
plt.show()
plt.savefig("wykres.png")
