import csv
import io
import subprocess
import sys
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def run_benchmark(executable):
    result = subprocess.run([executable], capture_output=True, text=True, check=True)
    return result.stdout


def parse_csv(csv_text):
    rows = list(csv.DictReader(io.StringIO(csv_text)))
    k = np.array([float(row["k"]) for row in rows])
    time_ms = np.array([float(row["time_ms"]) for row in rows])
    return k, time_ms


def linear_fit(x, y):
    slope, intercept = np.polyfit(x, y, 1)
    prediction = slope * x + intercept
    r_squared = 1 - ((y - prediction) ** 2).sum() / ((y - y.mean()) ** 2).sum()
    return slope, intercept, r_squared


def plot(k, time_ms, png_path):
    slope, intercept, r_squared = linear_fit(k, time_ms)
    loglog_slope, loglog_intercept = np.polyfit(np.log(k), np.log(time_ms), 1)

    fig, axes = plt.subplots(1, 2, figsize=(10, 4.4))

    axes[0].plot(k / 1e3, time_ms, "o", label="medido (mediana de 7)")
    axes[0].plot(k / 1e3, slope * k + intercept, "--",
                 label=f"ajuste lineal: {slope * 1e3:.2f} µs·k (R²={r_squared:.4f})")
    axes[0].axvline(100, color="gray", ls=":", lw=1)
    axes[0].text(105, time_ms.max() * 0.5, "límite del\nproblema\n(k = 10⁵)",
                 fontsize=8, color="gray")
    axes[0].set(xlabel="k (miles de elementos)", ylabel="tiempo (ms)",
                title="(a) Escala lineal")

    axes[1].loglog(k, time_ms, "o-", label="medido")
    axes[1].loglog(k, np.exp(loglog_intercept) * k ** loglog_slope, "--",
                   label=f"pendiente = {loglog_slope:.2f}")
    axes[1].set(xlabel="k", ylabel="tiempo (ms)", title="(b) Escala log-log")

    for axis in axes:
        axis.legend(fontsize=8)
        axis.grid(alpha=0.3, which="both")

    fig.suptitle("Tiempo de ejecución de solve() vs. tamaño de la entrada k (aᵢ de 60 bits)")
    fig.tight_layout()
    fig.savefig(png_path, dpi=150)

    print(f"t = {slope * 1e3:.3f} µs * k + {intercept:.2f} ms   R2={r_squared:.5f}   "
          f"log-log slope={loglog_slope:.3f}")


def main():
    executable, directory = sys.argv[1], Path(sys.argv[2])
    directory.mkdir(parents=True, exist_ok=True)

    csv_text = run_benchmark(executable)
    (directory / "times.csv").write_text(csv_text)

    k, time_ms = parse_csv(csv_text)
    plot(k, time_ms, directory / "time_vs_k.png")
    print(f"Results in {directory}")


if __name__ == "__main__":
    main()
