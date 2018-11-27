#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Sep  6 15:19:57 2018

@author: david
"""
import tkinter as tk
import bluetooth


bt_name = 'HC-05'
# pas des moteur haut 64*64*6 24576
# pas des moteur lat  64*64*4 16384

#label = tk.Label(windows,text="test")
#label.pack()
#for ligne in range(5):
#    for colonne in range(5):
#        tk.Button(windows, text='L%s-C%s' % (ligne, colonne), borderwidth=1) \
#            .grid(row=ligne, column=colonne)
class apps:
    def __init__(self, bt_address):
        self.list_el = {}
        self.windows = tk.Tk()
        port = 1
        
        self.sock = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
        self.sock.connect((bt_address, port))
        self.sock.settimeout(1.0)
        
        self.list_el["gauche"] = tk.Button(self.windows, text="gauche", 
                                    command=lambda: self.send_ct(0,-1000,5000))
        self.list_el["gauche"].grid(row=1, column=0)
        self.list_el["droit"]  = tk.Button(self.windows, text="droite",
                                    command=lambda: self.send_ct(0,1000,5000))
        self.list_el["droit"].grid(row=1, column=2)
        self.list_el["haut"]   = tk.Button(self.windows, text="haut",
                                    command=lambda: self.send_ct(-1000,0,5000))
        self.list_el["haut"].grid(row=0, column=1)
        self.list_el["bas"]    = tk.Button(self.windows, text="bas",
                                    command=lambda: self.send_ct(1000,0,5000))
        self.list_el["bas"].grid(row=2, column=1)
#        self.list_el["pasVal"]= tk.IntVar();
#        self.list_el["pasVal"].set(1000)
        
#        self.list_el["pas"]    = tk.Spinbox(self.windows, from_=0, to=10000, 
#                                            increment=100)
#        self.list_el["pas"].grid(row=1, column=1)
        self.list_el["pause"] = tk.Button(self.windows, text="pause",
                                    command=lambda: self.send_data("p", False))
        self.list_el["pause"].grid(row=0, column=3)
        self.list_el["zero"] = tk.Button(self.windows, text="reset origine",
                                    command=lambda: self.send_data("r", False))
        self.list_el["zero"].grid(row=1, column=3)        
        self.list_el["zeroA"] = tk.Button(self.windows, text="go to origine",
                                    command=lambda: self.send_data("d", False))
        self.list_el["zeroA"].grid(row=2, column=3)
        self.list_el["textValue"] = tk.StringVar() 
        self.list_el["textValue"].set("g24576 16384 30000")
        
        self.list_el["text"] = tk.Entry(self.windows,
                                        textvariable=self.list_el["textValue"],
                                        width=45)
        self.list_el["text"].grid(row=3, columnspan=3, column=0)
        self.list_el["textGo"] = tk.Button(self.windows, text="go",
            command=lambda: self.send_data(self.list_el["textValue"].get(), False))
        self.list_el["textGo"].grid(row=3, column=3)
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

def find(bt_name): 
    for bt in bluetooth.discover_devices(4, lookup_names=True):
        if bt_name == bt[1]:
            return bt[0]
    return ''

bt_ad = ''   
while  bt_ad == '':
    bt_ad = find(bt_name)

App = apps(bt_ad)




