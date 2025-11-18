import tkinter as tk
import serial
import time
from tkinter import messagebox

# List to hold the serial objects (initialized later as serial objects or None)
arduinos = []
# List to hold the Tkinter Canvas indicator references
indicators = []

# Update with your actual Linux port names and baud rates
PORT_INFO = [
    ('/dev/ttyACM0', 9600),
    ('/dev/ttyUSB0', 9600),
    ('/dev/ttyACM1', 9600),
    ('/dev/ttyUSB1', 9600)
]

def connect_arduinos():
    """Attempts to connect to all specified serial ports and updates indicators."""
    print("Attempting to connect to Arduinos...")
    for i, (port, baud) in enumerate(PORT_INFO):
        try:
            ser = serial.Serial(port, baud, timeout=1)
            arduinos.append(ser)
            time.sleep(2) # Give the connection time to establish
            print(f"Connected to {port}")
            # Update the corresponding indicator to green
            if i < len(indicators):
                canvas, indicator_id = indicators[i]
                canvas.itemconfig(indicator_id, fill='green')
        except serial.SerialException as e:
            print(f"Could not open port {port}: {e}")
            arduinos.append(None) # Maintain index alignment
            # Update the corresponding indicator to red
            if i < len(indicators):
                canvas, indicator_id = indicators[i]
                canvas.itemconfig(indicator_id, fill='red')
            # messagebox.showerror("Connection Error", f"Could not connect to {port}.")


def send_command(arduino_index, command):
    """Sends a specific command to a specific Arduino."""
    if arduino_index < len(arduinos) and arduinos[arduino_index] is not None:
        try:
            arduinos[arduino_index].write(command.encode('utf-8'))
            print(f"Sent '{command}' to Arduino {arduino_index + 1}")
        except Exception as e:
            print(f"Error sending command to Arduino {arduino_index + 1}: {e}")
            messagebox.showerror("Communication Error", f"Failed to send command to Arduino {arduino_index + 1}.")
    else:
        print(f"Arduino {arduino_index + 1} not connected.")

# --- Tkinter GUI Setup ---

root = tk.Tk()
root.title("AIP Sonicator Control Interface")
root.geometry("1000x600")

def create_arduino_section(parent_frame, arduino_index, title_text):
    """Creates a framed section with buttons and a status indicator."""
    frame = tk.LabelFrame(parent_frame, text=title_text, padx=10, pady=10, borderwidth=2, relief="groove")
    
    # --- Status Indicator ---
    canvas = tk.Canvas(frame, width=30, height=30, bg=frame.cget('bg'), highlightthickness=0)
    # Start with a gray/disconnected indicator
    indicator_id = canvas.create_oval(5, 5, 25, 25, fill='gray', outline='black') 
    canvas.grid(row=0, column=2, padx=5, pady=5, sticky='e')

    # Store the canvas and the indicator ID globally so connect_arduinos can access it
    indicators.append((canvas, indicator_id))
    
    # --- Buttons ---
    btn1 = tk.Button(frame, text="Homing Positions", font=25, bg='ghost white',fg='black', border=5, command=lambda: send_command(arduino_index, 'A'))
    btn1.grid(row=1, column=0, padx=5, pady=5)
    
    btn2 = tk.Button(frame, text="Calibrate Position", font=25, bg='snow',fg='black', border=5, command=lambda: send_command(arduino_index, 'B'))
    btn2.grid(row=1, column=1, padx=5, pady=5)
    
    btn3 = tk.Button(frame, text="24 protocol", font=25, bg='RoyalBlue1',fg='black', border=5, command=lambda: send_command(arduino_index, 'C'))
    btn3.grid(row=2, column=0, padx=5, pady=5)
    
    btn4 = tk.Button(frame, text="96 protocol", font=25, bg='Purple3',fg='black', border=5, command=lambda: send_command(arduino_index, 'D'))
    btn4.grid(row=2, column=1, padx=5, pady=5)

    return frame

# Create and place the 4 sections
frame1 = create_arduino_section(root, 0, "Sonicator 1")
frame1.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")

frame2 = create_arduino_section(root, 1, "Sonicator 2")
frame2.grid(row=0, column=1, padx=10, pady=10, sticky="nsew")

frame3 = create_arduino_section(root, 2, "Sonicator 3")
frame3.grid(row=1, column=0, padx=10, pady=10, sticky="nsew")

frame4 = create_arduino_section(root, 3, "Sonicator 4")
frame4.grid(row=1, column=1, padx=10, pady=10, sticky="nsew")

# Configure the main window's grid to expand nicely
root.grid_columnconfigure(0, weight=1)
root.grid_columnconfigure(1, weight=1)
root.grid_rowconfigure(0, weight=1)
root.grid_rowconfigure(1, weight=1)

# Connect devices on startup. This function now updates the indicators.
connect_arduinos()

# Start the Tkinter event loop
root.mainloop()

# Ensure serial ports are closed when the GUI window is closed
for ser in arduinos:
    if ser is not None and ser.is_open:
        ser.close()
