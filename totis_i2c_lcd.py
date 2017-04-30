import lcddriver
from time import *
import argparse

"""
NOTATION FOR PUTTING STRING TO LCD:
lcd = lcddriver.lcd()
lcd.lcd_display_string("string goes here", 1)
1 == first line, 2 == second line

CHECK lcddriver.py
FOR MORE DOCUMENTATION

"""
parser = argparse.ArgumentParser(description="Display user feedback for the given action and given toner.")
parser.add_argument("action", type=int, help="The action taken with the given toner, 1 is add, -1 is remove, 0 if no toner in stock")
parser.add_argument("tonername", type=str, help="A string holding the name of toner to be displayed")
args = parser.parse_args()


lcd = lcddriver.lcd()

#ALWAYS START DISPLAYING THE FOLLOWING:
def mainDisplay():
    lcd.lcd_clear()
    lcd.lcd_display_string("     TOTIS      ", 1)
    lcd.lcd_display_string("Press When Ready", 2)

#mainDisplay()
#sleep(2)
if(args.action == -1):
    #removing toner
    lcd.lcd_clear()
    lcd.lcd_display_string("Toner to remove:", 1)
    lcd.lcd_display_string(args.tonername, 2)
    sleep(1.5)
    lcd.lcd_clear()
    lcd.lcd_display_string(args.tonername, 1)
    lcd.lcd_display_string("    removed!    ", 2)
    sleep(1.5)
    mainDisplay()
elif(args.action == 1):
    #adding toner
    lcd.lcd_clear()
    lcd.lcd_display_string("Toner to add:", 1)
    lcd.lcd_display_string(args.tonername, 2)
    sleep(1.5)
    lcd.lcd_clear()
    lcd.lcd_display_string(args.tonername, 1)
    lcd.lcd_display_string("      added!      ", 2)
    sleep(1.5)
    mainDisplay()
