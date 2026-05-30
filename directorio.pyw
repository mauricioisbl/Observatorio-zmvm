from pathlib import Path
from datetime import datetime


def main():
    base = Path(__file__).resolve().parent
    salida = base / "rutas_directorio.txt"

    with salida.open("w", encoding="utf-8") as f:
        f.write(f"Directorio base: {base}\n")
        f.write(f"Generado: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n")

        for ruta in sorted(base.rglob("*"), key=lambda p: str(p).lower()):
            f.write(str(ruta) + "\n")


if __name__ == "__main__":
    main()