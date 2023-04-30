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

        self.quit_button = tk.Button(self.frame, text="Quit", command=self.quit)
        self.quit_button.pack(side=tk.BOTTOM)

        self.text_box = tk.Text(self.frame)
        self.text_box.pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)
        self.text_box.bind("<Key>", self.send)

        self.fig, self.ax = plt.subplots(subplot_kw={'projection': 'polar'})
        #plt.ion()  # enable interactive mode
        self.xdata = []
        self.ydata = []
        self.ax.set_ylim([0,110]) # set y-axis limits #size of graph
        self.ax.set_xlim([0,np.pi]) # set x-axis limits
        
        self.canvas = matplotlib.backends.backend_tkagg.FigureCanvasTkAgg(self.fig, master=self.frame)
        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)
        #self.ax.set_thetamin(0)
        #self.ax.set_thetamax(180)
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
        #self.master.bind("<Key-W>", self.reset)
        self.canvas.mpl_connect('key_press_event', self.clear_graph)
    def clear_graph(self, event):
        if event.key == 'c':
            self.ax.clear()
            self.ax.set_ylim([0,110]) # set y-axis limits #size of graph
            self.ax.set_xlim([0,np.pi])
            #self.canvas.draw()

    def receive_data(self):
        self.ax.clear()
        self.ax.set_ylim([0, 110])
        self.ax.set_xlim([0, np.pi])
        while True:
            data = self.socket.recv(1024)
            #print(data)
            if not data:
                break
            
            def clear_graph(self, event):
                if event.key == 'C':
                    self.ax.clear()
                    self.canvas.draw()
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
            for obj in range(0, len(data_members)-1, 3):
                
                width = float(data_members[obj].split(':')[1])
                angle = float(data_members[obj + 1].split(':')[1])
                dist = float(data_members[obj + 2].split(':')[1])
                color = 'blue'# if int(data_members[obj + 3].split(':')[1]) == 0 else 'red'
                shape = 'o' #if int(data_members[obj + 4].split(':')[1]) == 0 else '^'
                #if obj+3 < len(data_members) and data_members[obj + 3].startswith('drop:'):
                   # drop = bool(int(data_members[obj + 3].split(':')[1]))
                    #color = 'blue' if not drop else 'red'

                #if obj+4 < len(data_members) and data_members[obj + 4].startswith('bump:'):
                  #  bump = bool(int(data_members[obj + 4].split(':')[1]))
                  #  shape = 'o' if not bump else '^'


                self.ax.scatter(np.radians(angle), dist, c=color, marker=shape, s=width)


            self.canvas.draw()
            time.sleep(0.9)
            
            #self.data_obj = {'width': self.width, 'angle': self.angle, 'dist': self.dist}
            #print(self.data_obj)

        # Wait for a short period before checking for new data
        time.sleep(30)
        
        #self.receive_data()

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
