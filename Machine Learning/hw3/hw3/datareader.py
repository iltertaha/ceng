# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import pickle

with open("./hw3_data/dt/data.pkl","rb") as f:
    train_data,test_data,attr_vals_list,attr_names = pickle.load(f)