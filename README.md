# TOTIS
TechOps Toner Inventory System

TOTIS, or TechOps Toner Inventory System, is my project for my CPS 210 class at CMU. This project goes beyond just school and will be used professionally at TechOps as a way to handle our inventory of toner, something that can quickly go awry when done manually.

TOTIS will be ran on a Raspberry Pi and will use button input, LED output, and output to an LCD through I2C busses. The majority of TOTIS will be written in C, but due to ease of libraries for I2C LCDs being acquired forces the LCD output code to be written in Python.
