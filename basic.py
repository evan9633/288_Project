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
        #plt.ion()  # enable interactive mode
        self.xdata = []
        self.ydata = []
        self.ax.set_ylim([0,150]) # set y-axis limits
        self.ax.set_xlim([0,180]) # set x-axis limits
        self.canvas = matplotlib.backends.backend_tkagg.FigureCanvasTkAgg(self.fig, master=self.frame)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        self.data_obj = None
        self.width = None
        self.angle = None
        self.dist = None
        self.drop = None
        self.bump = None

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect(('192.168.1.1', 288))
        self.receive_thread = threading.Thread(target=self.receive_data)
        self.receive_thread.start()
        plt.show

    def receive_data(self):
        start_time = time.time()
        while True:
            data = self.socket.recv(1024)
            print(data)
            if not data:
                break
            if time.time() - start_time > 50:
                break

        data_str = data.decode('utf-8')#"width:5;angle:20;dist:25;drop:0;bump:0;"        
        while ';' not in data_str and ':' not in data_str:
            data += self.socket.recv(1024)
            data_str = data.decode('utf-8')
            time.sleep(0.1)
            #print(data)
            # Simulate receiving data for demonstration purposes
            

        # Split the data string into individual data members
        data_members = data_str.strip().split(';')
        data_dict = {}

        # Parse each data member and store it in a dictionary
            #print(data_members)
        for member in data_members:
            if ':' in member:
                key, value = member.strip().split(':')
                data_dict[key] = value

        # Check if all required data members are present in the dictionary
        if all(key in data_dict for key in ('width', 'angle', 'dist')):
            # Extract the required data from the dictionary
            self.width = float(data_dict['width'])
            self.angle = float(data_dict['angle'])
            self.dist = float(data_dict['dist'])

            # Create a new object with the data members
            if self.ax.lines:
                self.ax.lines[-1].set_alpha(0.5)

            color = 'b'
            MAX_CIRCLE_SIZE = 12
            #self.ax.plot([self.angle, self.angle], [0, self.dist], linewidth=, color=color, alpha=1.0)
            circle_size = min(self.width, MAX_CIRCLE_SIZE)
            if circle_size == MAX_CIRCLE_SIZE:
                color = 'r'
            self.dist +=2
            circle = plt.Circle((np.radians(self.angle), self.dist+2), circle_size, color=color, alpha=1.0)
            self.ax.add_artist(circle)
            self.canvas.draw_idle()

           

            # Append the x and y values to the lists
            #self.xdata.append(x)
            #self.ydata.append(y)

            # Redraw the plot with the updated data
            #self.ax.clear()
           # for circle in self.ax.artists:
            #    self.ax.add_artist(circle)
            #self.canvas.draw_idle()          
            time.sleep(0.1)
            #self.ax.set_ylim([0, MAX_CIRCLE_SIZE * 1.2])
            # Create a data object and do something with it, such as store it in a list or pass it to another function
            self.data_obj = {'width': self.width, 'angle': self.angle, 'dist': self.dist+2}
            print(self.data_obj)

        # Wait for a short period before checking for new data
        time.sleep(0.5)


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
