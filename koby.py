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
        self.ax.set_ylim([0,150]) # set y-axis limits
        self.ax.set_xlim([0,180]) # set x-axis limits
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.frame)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        self.data_obj = None
        self.width = None
        self.angle = None
        self.dist = None

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect(('192.168.1.1', 288))
        self.receive_thread = threading.Thread(target=self.receive_data)
        self.receive_thread.start()

    def receive_data(self):
        start_time = time.time()
        while True:
            data = self.socket.recv(1024)
            print(data)
            if not data:
                break
            if time.time() - start_time > 50:
                break

        data_str = data.decode('utf-8')        
        while ';' not in data_str and ':' not in data_str:
            data += self.socket.recv(1024)
            data_str = data.decode('utf-8')
            time.sleep(0.1)

        data_members = data_str.strip().split(';')
        data_dict = {}

        for member in data_members:
            if ':' in member:
                key, value = member.strip().split(':')
                data_dict[key] = value

        if all(key in data_dict for key in ('width', 'angle', 'dist')):
            self.width = float(data_dict['width'])
            self.angle = float(data_dict['angle'])
            self.dist = float(data_dict['dist'])

            color = 'b'
            MAX_CIRCLE_SIZE = 12
            circle_size = min(self.width, MAX_CIRCLE_SIZE)
            if circle_size == MAX_CIRCLE_SIZE:
                color = 'r'
            circle = plt.Circle((np.radians(self.angle), self.dist), circle_size, color=color, alpha=1.0)
            self.ax.add_artist(circle)
            self.canvas.draw_idle()

            time.sleep(5)

            self.data_obj = {'width': self.width, 'angle': self.angle, 'dist': self.dist}
            print(self.data_obj)

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
