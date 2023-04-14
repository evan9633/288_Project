import tkinter as tk
import socket
import threading

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

    def receive_data(self):
        while True:
            data = self.socket.recv(1024)
            if not data:
                break
            data_str = data.decode('utf-8')
            self.text_box.insert(tk.END, data_str)
            self.text_box.see(tk.END)

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
    
