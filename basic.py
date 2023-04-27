import socket
import threading
import time
import tkinter as tk
import numpy as np
import matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib import pyplot as plt
class App:
    def __init__(self, master):
        self.master = master
        self.frame = tk.Frame(self.master)
        self.frame.pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        self.quit_button = tk.Button(self.frame, text="Quit", command=self.quit)
        self.quit_button.pack(side=tk.BOTTOM)

        self.text_box = tk.Text(self.frame)
        self.text_box.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)
        self.text_box.bind("<Key>", self.send)

        self.fig, self.ax = plt.subplots(subplot_kw={'projection': 'polar'})
        plt.ion()  # enable interactive mode
        self.xdata = []
        self.ydata = []
        self.ax.set_ylim([-10,10]) # set y-axis limits
        self.ax.set_xlim([-10,10]) # set x-axis limits
        self.canvas = matplotlib.backends.backend_tkagg.FigureCanvasTkAgg(self.fig, master=self.frame)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        self.data_obj = None
        self.width = None
        self.angle = None
        self.dist = None
        self.drop = None
        self.bump = None


    def receive_data(self):
        while True:
        # Simulate receiving data for demonstration purposes
            data_str = "width:5;angle:20;dist:25;drop:0;bump:0;"

        # Split the data string into individual data members
            data_members = data_str.strip().split(';')
            data_dict = {}

        # Parse each data member and store it in a dictionary
            for member in data_members:
                if ':' in member:
                    key, value = member.strip().split(':')
                    data_dict[key] = value

        # Check if all required data members are present in the dictionary
            if all(key in data_dict for key in ('width', 'angle', 'dist')):
            # Extract the required data from the dictionary
                width = float(data_dict['width'])
                angle = float(data_dict['angle'])
                dist = float(data_dict['dist'])

            # Create a new object with the data members
                if self.ax.lines:
                    self.ax.lines[-1].set_alpha(0.5)

                if 'tape' in data_dict:
                    color = 'r'
                else:
                    color = 'g'

            self.ax.plot([angle, angle], [0, dist], linewidth=width, color=color, alpha=1.0)
            self.ax.plot([angle], [dist], marker='o', markersize=5, color=color, alpha=1.0)

            x = dist * np.cos(np.radians(angle))
            y = dist * np.sin(np.radians(angle))

            self.ax.plot([np.radians(angle)], [dist], marker='o', markersize=5, color='b', alpha=1.0)

            # Append the x and y values to the lists
            self.xdata.append(x)
            self.ydata.append(y)

            # Redraw the plot with the updated data
            self.ax.plot(self.xdata, self.ydata, 'r-')
            self.canvas.draw_idle()

            # Create a data object and do something with it, such as store it in a list or pass it to another function
            data_obj = {'width': width, 'angle': angle, 'dist': dist}
            print(data_obj)

        # Wait for a short period before checking for new data
        time.sleep(0.1)


    def send(self, event):
        key = event.char
        if key:
            self.socket.sendall(key.encode())

    def quit(self):
        self.socket.close()
        self.master.destroy()

if __name__ == '__main__':
    root = tk.Tk()
    app = App(root)
    app.receive_thread = threading.Thread(target=app.receive_data)
    app.receive_thread.start()
    root.mainloop()
