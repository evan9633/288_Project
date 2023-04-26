import socket
import threading
import tkinter as tk

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
        self.socket.bind(('192.168.0.1', 1234))
        self.socket.listen(1)

        self.client_socket, self.client_address = self.socket.accept()
        self.receive_thread = threading.Thread(target=self.receive_data)
        self.receive_thread.start()

    def receive_data(self):
        while True:
            data = self.client_socket.recv(1024)
            if not data:
                break
            data_str = data.decode('utf-8')
            self.text_box.insert(tk.END, data_str)
            self.text_box.see(tk.END)

    def send(self, event):
        key = event.char
        if key:
            self.client_socket.sendall(key.encode())

    def quit(self):
        self.client_socket.close()
        self.socket.close()
        self.master.destroy()

if __name__ == '__main__':
    root = tk.Tk()
    app = App(root)
    root.mainloop()
