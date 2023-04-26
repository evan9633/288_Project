import tkinter as tk
import socket
import threading
import time

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

        ax.set_ylim([-10,10]) # set y-axis limits
        ax.set_xlim([-10,10]) # set x-axis limits
        
        self.receive_thread = threading.Thread(target=self.receive_data)
        self.receive_thread.start()

        self.fig, self.ax = plt.subplots(subplot_kw={'projection': 'polar'})

        self.data_obj = None
        self.width = None
        self.angle = None
        self.dist = None
        self.drop = None
        self.bump = None

    def receive_data(self):
        while True:
            data = self.socket.recv(1024)
            if not data:
                break
            data_str = data.decode('utf-8')
            self.text_box.insert(tk.END, data_str)
            self.text_box.see(tk.END)

            # wait until data is enough for a key, value pair
            while ';' not in data_str and ':' not in data_str:
                data += self.socket.recv(1024)
                data_str = data.decode('utf-8')
                time.sleep(0.1)

            # split the data string into individual data members
            data_members = data_str.strip().split(';')
            for member in data_members:
                # split each data member into its key and value
                if ':' in member:
                    key, value = member.strip().split(':')
                    if key == 'width':
                        self.width = float(value)
                    elif key == 'angle':
                        self.angle = float(value)
                    elif key == 'dist':
                        self.dist = float(value)
                    elif key == 'drop':
                        self.drop = True
                    elif key == 'bump':
                        self.drop = str(value)

            # check if all data members have been received and process them
            if 'dist' in locals() and 'angle' in locals() and dist is not None and angle is not None:
                # create a new object with the data members
                if self.ax.lines:
                    self.ax.lines[-1].set_alpha(0.5)

                if tape is not None:
                    color = 'r'
                else:
                    color = 'g'
                
                x = dist * np.cos(np.radians(angle))
                y = dist * np.sin(np.radians(angle))

                self.ax.plot([angle, angle], [0, dist], linewidth=width, color=color, alpha=1.0)
                self.ax.plot([angle], [dist], marker='o', markersize=5, color=color, alpha=1.0)

                plt.draw()
                self.data_obj = {'width': self.width, 'angle': self.angle, 'dist': self.dist}
                # do something with the data object, such as store it in a list or pass it to another function
                print(self.data_obj)
                # reset the data members to None for the next data object
                self.width = None
                self.angle = None
                self.dist = None
                self.drop = None
                self.drop = None

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