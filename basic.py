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

        self.receive_thread = threading.Thread(target=self.receive_data)
        self.receive_thread.start()

        self.data_obj = None
        self.width = None
        self.angle = None
        self.dist = None

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

            # check if all data members have been received and process them
            if self.width is not None and self.angle is not None and self.dist is not None:
                # create a new object with the data members
                self.data_obj = {'width': self.width, 'angle': self.angle, 'dist': self.dist}
                # do something with the data object, such as store it in a list or pass it to another function
                print(self.data_obj)
                # reset the data members to None for the next data object
                self.width = None
                self.angle = None
                self.dist = None

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