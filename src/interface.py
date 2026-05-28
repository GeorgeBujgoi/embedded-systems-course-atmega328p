import serial
import tkinter as tk
from tkinter import ttk
import math

PORT = "COM12"
BAUD = 57600

ser = serial.Serial(PORT, BAUD, timeout=1)

def water_to_percent(value):
    empty = 300
    full = 700

    percent = (value - empty) * 100 / (full - empty)
    percent = max(0, min(100, percent))

    return int(percent)

def temp_to_celsius(adc):
    if adc <= 0 or adc >= 1023:
        return 0

    r_fixed = 10000
    beta = 3950
    room_temp = 298.15
    r0 = 10000

    resistance = r_fixed * (1023.0 / adc - 1.0)

    temperature_kelvin = 1.0 / (
        (1.0 / room_temp) +
        (1.0 / beta) * math.log(resistance / r0)
    )

    return round(temperature_kelvin - 273.15, 1)

def update_data():
    try:
        line = ser.readline().decode(errors="ignore").strip()

        if line:
            if "AUTO MODE" in line:
                mode_value.set("AUTO")

            if "MANUAL MODE" in line:
                mode_value.set("MANUAL")

            parts = line.split()

            for part in parts:
                if part.startswith("Soil="):
                    soil_adc = int(part.split("=")[1])
                    soil_value.set(str(soil_adc))

                elif part.startswith("Water="):
                    water_adc = int(part.split("=")[1])
                    water_value.set(f"{water_to_percent(water_adc)} %")

                elif part.startswith("Temp="):
                    temp_adc = int(part.split("=")[1])
                    temp_value.set(f"{temp_to_celsius(temp_adc)} °C")

                elif part.startswith("Mode="):
                    mode_value.set(part.split("=")[1])

            if "NO WATER" in line:
                water_status.set("NO WATER")
            else:
                water_status.set("OK")

    except:
        pass

    root.after(300, update_data)


root = tk.Tk()
root.title("Smart Irrigation Monitor")
root.geometry("420x300")

soil_value = tk.StringVar(value="-")
water_value = tk.StringVar(value="-")
temp_value = tk.StringVar(value="-")
mode_value = tk.StringVar(value="AUTO")
water_status = tk.StringVar(value="-")

ttk.Label(
    root,
    text="Smart Irrigation System",
    font=("Arial", 16, "bold")
).pack(pady=10)

ttk.Label(
    root,
    textvariable=mode_value,
    font=("Arial", 14, "bold")
).pack(pady=5)

frame = ttk.Frame(root)
frame.pack(pady=10)

ttk.Label(frame, text="Soil:").grid(row=0, column=0, padx=10, pady=8, sticky="w")
ttk.Label(frame, textvariable=soil_value).grid(row=0, column=1)

ttk.Label(frame, text="Water Level:").grid(row=1, column=0, padx=10, pady=8, sticky="w")
ttk.Label(frame, textvariable=water_value).grid(row=1, column=1)

ttk.Label(frame, text="Temperature:").grid(row=2, column=0, padx=10, pady=8, sticky="w")
ttk.Label(frame, textvariable=temp_value).grid(row=2, column=1)

ttk.Label(frame, text="Water Status:").grid(row=3, column=0, padx=10, pady=8, sticky="w")
ttk.Label(frame, textvariable=water_status).grid(row=3, column=1)

update_data()
root.mainloop()