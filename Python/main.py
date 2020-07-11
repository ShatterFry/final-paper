import sys
import tkinter

class Application(tkinter.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.hi_there = tkinter.Button(self)
        self.hi_there["text"] = "Hello, World!\n(click me)"
        self.hi_there["command"] = self.say_hi
        self.hi_there.pack(side="top")

        self.quit = tkinter.Button(self, text="QUIT", bg="blue", fg="red", command=self.master.destroy)
        self.quit.pack(side="bottom")

    def say_hi(self):
        print("hi there, everyone!")

def main():
    print("Hello, World!")

    root = tkinter.Tk()
    app = Application(master=root)
    app.master.title("My Application Title")
    window_resolutions = ["800x600"]
    app.master.geometry("640x480")
    app.mainloop()

    sys.exit(0)

if __name__ == "__main__":
    main()