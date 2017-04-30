import lcddriver
from time import *

lcd = lcddriver.lcd()

lcd.lcd_clear()

temp = ""
totis = "-----TOTIS------"

for x in range(0, 16):
    temp += "-"
    lcd.lcd_display_string(temp, 1)
    lcd.lcd_display_string(totis[0:x+1], 2)
    sleep(0.25)

sleep(1)

lcd.lcd_display_string("   Welcome To   ", 1)
lcd.lcd_display_string("     TOTIS      ", 2)

sleep(3)

lcd.lcd_display_string("     TOTIS      ", 1)
lcd.lcd_display_string("Press When Ready", 2)

