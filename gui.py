import tkinter as tk
import serial
import time
from tkinter import messagebox
from PIL import Image, ImageTk

# List to hold the serial objects (initialized later as serial objects or None)
arduinos = []
# List to hold the Tkinter Canvas indicator references
indicators = []

# Update with your actual Linux port names and baud rates
PORT_INFO = [
    ('/dev/ttyACM0', 9600),
    ('/dev/ttyACM1', 9600),
    ('/dev/ttyACM2', 9600),
    ('/dev/ttyACM3', 9600)
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
root.geometry("1200x900")


# Path to your image file
image_path = "/home/aipuser/arduino/Logomark-Gradient-Dark.png" 
image2_path = "/home/aipuser/arduino/Logo-Gradient-Dark.png" 

# Open the image using Pillow
image = Image.open(image_path)
image2 = Image.open(image2_path)
image2= image2.resize((505, 126), Image.Resampling.LANCZOS)

# Convert the image to a Tkinter PhotoImage object
photo = ImageTk.PhotoImage(image)
photo2 = ImageTk.PhotoImage(image2)

# Set the window icon. The first argument (True/False) determines if 
# the icon applies to top-level windows (True) or only the root (False).
root.iconphoto(False, photo) 

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
    
    btn3 = tk.Button(frame, text="24 protocol x2", font=25, bg='RoyalBlue1',fg='black', border=5, command=lambda: send_command(arduino_index, 'C'))
    btn3.grid(row=2, column=0, padx=5, pady=5)
    
    btn4 = tk.Button(frame, text="96 protocol x2", font=25, bg='Purple3',fg='black', border=5, command=lambda: send_command(arduino_index, 'D'))
    btn4.grid(row=2, column=1, padx=5, pady=5)

    btn5 = tk.Button(frame, text="24 protocol x1", font=25, bg='RoyalBlue1',fg='black', border=5, command=lambda: send_command(arduino_index, 'E'))
    btn5.grid(row=3, column=0, padx=5, pady=5)
    
    btn6 = tk.Button(frame, text="96 protocol x1", font=25, bg='Purple3',fg='black', border=5, command=lambda: send_command(arduino_index, 'F'))
    btn6.grid(row=3, column=1, padx=5, pady=5)

    btn7 = tk.Button(frame, text="Stop", font=25, bg='red',fg='black', border=5, command=lambda: send_command(arduino_index, 'S'))
    btn7.grid(row=4, column=0, padx=5, pady=5)

    btn8 = tk.Button(frame, text="Test", font=25, bg='blue',fg='white', border=5, command=lambda: send_command(arduino_index, 'T'))
    btn8.grid(row=4, column=1, padx=5, pady=5)

    return frame

# Create and place the 4 sections

image_label =tk.Label(root, image=photo2)
image_label.grid(row=2, column=1, pady=20)

frame1 = create_arduino_section(root, 0, "Sonicator A")
frame1.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")

frame2 = create_arduino_section(root, 1, "Sonicator B")
frame2.grid(row=0, column=1, padx=10, pady=10, sticky="nsew")

frame3 = create_arduino_section(root, 2, "Sonicator C")
frame3.grid(row=1, column=0, padx=10, pady=10, sticky="nsew")

frame4 = create_arduino_section(root, 3, "Sonicator D")
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
