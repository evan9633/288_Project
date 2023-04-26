import tkinter as tk
import socket
import threading
import matplotlib.pyplot as plt
import numpy as np

class App:
    def __init__(self, master):
        self.master = master
        self.frame = tk.Frame(self.master)
        self.frame.pack()

        self.quit_button = tk.Button(self.frame, text="Quit", command=self.quit)
        self.quit_button.pack(side=tk.BOTTOM)

        self.text_box = tk.Text(self.frame)
        self.text_box.pack()
        self.text_box.bind("<Key>", self.send)

        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect(('192.168.1.1', 288))

        self.receive_thread = threading.Thread(target=self.receive_data)
        self.receive_thread.start()
      #  self.width = None
        self.angle = None
        self.dist = None
        self.tape = None

    def receive_data(self):
        while True:
            data = self.socket.recv(1024)
            if not data:
                break
            data_str = data.decode('utf-8')
            self.text_box.insert(tk.END, data_str)
            self.text_box.see(tk.END)

            # split the data string into individual data members
            data_members = data_str.strip().split(';')
            #member_list = data_str.strip().split(':')
                # initialize plot
            fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
            ax.set_ylim([-10,10]) # set y-axis limits
            ax.set_xlim([-10,10]) # set x-axis limits
           
            for member in data_members:
                # split each data member into its key and value
                key, value = member.strip().split(':')
                if key == 'width':
                   self.width = float(value)
                elif key == 'angle':
                    self.angle = float(value)
                elif key == 'dist':
                    self.dist = float(value)
                elif key == 'tape':
                    self.tape = str(value)
                if self.angle is not None:
                    x = dist * np.cos(np.radians(angle))
                if self.angle is not None:
                    y = dist * np.sin(np.radians(angle))
                if self.angle is not None:
                    ax.plot([0, x], [0, y], linewidth=width, alpha=0.5)
                
                #plot coordinates in real time

                plt.pause(0.001)

                #update plot


            # check if all data members have been received and process them
            if self.width is not None and self.angle is not None and self.dist is not None:
                # create a new object with the data members
                data_obj = {'width': self.width, 'angle': self.angle, 'dist': self.dist}
                # do something with the data object, such as store it in a list or pass it to another function
                print(data_obj)
                # reset the data members to None for the next data object
                self.width = None
                self.angle = None
                self.dist = None

        plt.show()
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
    root.mainloop()
    
