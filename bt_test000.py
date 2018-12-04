#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Sep  6 15:19:57 2018

@author: david
"""
import tkinter as tk
import bluetooth


bt_name = 'HC-05'
# pas des moteur pan  64*64*6 24576
# pas des moteur litz 64*64*6 24576

pas_pan=24576
pas_tilt=24576

class apps:
    def __init__(self, bt_address):
        self.list_el = {}
        self.joy_a = False
        self.joy_x = 0
        self.joy_y = 0
        self.windows = tk.Tk()
        port = 1
        
        self.sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
        self.sock.connect((bt_address, port))
        self.sock.settimeout(1.0)
        
        self.list_el["gauche"] = tk.Button(self.windows, text="gauche")
        self.list_el["gauche"].grid(row=1, column=0)
        self.list_el["droit"]  = tk.Button(self.windows, text="droite")
        self.list_el["droit"].grid(row=1, column=2)
        self.list_el["haut"]   = tk.Button(self.windows, text="haut")
        self.list_el["haut"].grid(row=0, column=1)
        self.list_el["bas"]    = tk.Button(self.windows, text="bas")
        self.list_el["bas"].grid(row=2, column=1)

        self.list_el["pause"] = tk.Button(self.windows, text="pause",
                                    command=lambda: self.send_data("p", False))
        self.list_el["pause"].grid(row=0, column=4)
        self.list_el["zero"] = tk.Button(self.windows, text="reset origine",
                                    command=lambda: self.send_data("r", False))
        self.list_el["zero"].grid(row=1, column=4)        
        self.list_el["zeroA"] = tk.Button(self.windows, text="go to origine",
                                    command=lambda: self.send_data("d", False))
        self.list_el["zeroA"].grid(row=2, column=4)
        self.list_el["textValue"] = tk.StringVar() 
        self.list_el["textValue"].set("g%i %i 30000" %(pas_pan,pas_tilt))
        
        self.list_el["text"] = tk.Entry(self.windows,
                                        textvariable=self.list_el["textValue"],
                                        width=45)
        self.list_el["text"].grid(row=3, columnspan=3, column=0)
        self.list_el["textGo"] = tk.Button(self.windows, text="go",
            command=lambda: self.send_data(self.list_el["textValue"].get(), False))
        self.list_el["textGo"].grid(row=3, column=4)

        self.list_el["gauche"].bind('<Button-1>', 
                    lambda x: self.send_ct(1000,0,1))
        self.list_el["droit"].bind('<Button-1>', 
                    lambda x : self.send_ct(-1000,0,1))
        self.list_el["haut"].bind('<Button-1>', 
                    lambda x: self.send_ct(0,1000,1))
        self.list_el["bas"].bind('<Button-1>', 
                    lambda x: self.send_ct(0,-1000,1))

        self.list_el["mega_b"] = tk.Button(self.windows, text='', width=20, height=10)
        self.list_el["mega_b"].grid(row=0, rowspan=3, column=3)
        self.list_el["mega_b"].bind('<Button-1>',
                        lambda x : self.joy_ac(x, True))
        self.list_el["mega_b"].bind('<ButtonRelease-1>',
                        lambda x : self.joy_ac(x, False) ) 
        self.list_el["mega_b"].bind('<Motion>', self.joy) 

        self.windows.mainloop()
        self.sock.close()

    def send_data(self, op, as_pas=True):
        if as_pas:
            pas = self.list_el["pas"].get()
            self.sock.send("%s%s" %(op,pas))
        else:
            self.sock.send("%s" %(op))

    def send_ct(self, x, y, p):
        if x * 3 > p : p = x * 3
        if y * 3 > p : p = y * 3
        self.sock.send("G%i %i %i" %(x,y,p) )

    def joy_ac(self,el, val):
        self.joy_a = val
        if val:
            self.joy(el)
        else:
            self.send_data("s0 0 0",False)

    def joy(self, el):
        if self.joy_a:
            x = el.x
            y = el.y
            height = self.list_el["mega_b"].winfo_height()
            width = self.list_el["mega_b"].winfo_width()
            if x >0 and y >0 and x < width and y < height:
                w2 = width/2
                h2 = height/2
                X = int((x-w2)/w2 * 10)
                Y = int((y-h2)/h2 * 10)
                X2 = X-9 if X>=0 else X+9
                Y2 = Y-9 if Y>=0 else Y+9
                if X2 == 0 : X2 = 1 if x-w2 < 0 else -1
                if Y2 == 0 : Y2 = 1 if y-h2 < 0 else -1
                if X2 != self.joy_x or Y2 != self.joy_y:
                    self.joy_x = X2
                    self.joy_y = Y2
                    self.send_data("s%i %i 1" %(X2, Y2), False)

def find(bt_name): 
    for bt in bluetooth.discover_devices(4, lookup_names=True):
        if bt_name == bt[1]:
            return bt[0]
    return ''

bt_ad = ''   
while  bt_ad == '':
    bt_ad = find(bt_name)

App = apps(bt_ad)