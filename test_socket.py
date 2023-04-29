import numpy as np
import matplotlib.pyplot as plt
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

        #self.quit_button = tk.Button(self.frame, text="Quit", command=self.quit)
        #self.quit_button.pack(side=tk.BOTTOM)

        self.text_box = tk.Text(self.frame)
        self.text_box.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)
        #self.text_box.bind("<Key>", self.send)

        self.fig, self.ax = plt.subplots(subplot_kw={'projection': 'polar'})
        #plt.ion()  # enable interactive mode
        self.xdata = []
        self.ydata = []
        self.ax.set_ylim([0,90]) # set y-axis limits
        self.ax.set_xlim([0,180]) # set x-axis limits
        self.canvas = matplotlib.backends.backend_tkagg.FigureCanvasTkAgg(self.fig, master=self.frame)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        self.data_obj = None
        self.width = None
        self.angle = None
        self.dist = None
        self.drop = None
        self.bump = None

        #self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
       # self.socket.connect(('192.168.1.1', 288))
        self.receive_thread = threading.Thread(target=self.graph_data)
        self.receive_thread.start()
        plt.show
    def graph_data(self):
        data ="width:12;angle:140;dist:73;drop:1;bump:0;width:6;angle:75;dist:9;drop:0;bump:1;width:17;angle:32;dist:56;drop:1;bump:1;width:3;angle:120;dist:45;drop:0;bump:0;width:9;angle:10;dist:22;drop:1;bump:1;width:1;angle:89;dist:68;drop:0;bump:1;width:18;angle:177;dist:12;drop:0;bump:0;width:10;angle:45;dist:87;drop:1;bump:0;width:4;angle:160;dist:30;drop:1;bump:1;width:7;angle:60;dist:80;drop:0;bump:0;"
        self.ax.clear()

    # Set plot limits
        self.ax.set_ylim([0, 90])
        self.ax.set_xlim([0, np.pi])

    # Get data string
    #data = "width:12;angle:140;dist:73;drop:1;bump:0;width:6;angle:75;dist:9;drop:0;bump:1;width:17;angle:32;dist:56;drop:1;bump:1;width:3;angle:120;dist:45;drop:0;bump:0;width:9;angle:10;dist:22;drop:1;bump:1;width:1;angle:89;dist:68;drop:0;bump:1;width:18;angle:177;dist:12;drop:0;bump:0;width:10;angle:45;dist:87;drop:1;bump:0;width:4;angle:160;dist:30;drop:1;bump:1;width:7;angle:60;dist:80;drop:0;bump:0;"

    # Parse data string
        data_members = data.strip().split(';')
        data_dict = {}
        for member in data_members:
            if ':' in member:
                key, value = member.strip().split(':')
                data_dict[key] = value

    # Plot objects
        for obj in range(0, len(data_members) - 1, 5):
            width = float(data_members[obj].split(':')[1])
            angle = float(data_members[obj + 1].split(':')[1])
            dist = float(data_members[obj + 2].split(':')[1])
            color = 'blue' if int(data_members[obj + 3].split(':')[1]) == 0 else 'red'
            shape = 'o' if int(data_members[obj + 4].split(':')[1]) == 0 else '^'

            self.ax.scatter(np.radians(angle), dist, c=color, marker=shape, s=width)

    # Draw plot
        self.canvas.draw()

    # Wait and repeat
        time.sleep(0.5)
        self.graph_data()

if __name__ == '__main__':
    
    root = tk.Tk()
    app = App(root)
    app.receive_thread = threading.Thread(target=app.graph_data)
    app.receive_thread.start()
    root.mainloop()
