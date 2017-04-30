import lcddriver
from time import *

lcd = lcddriver.lcd()

lcd.lcd_clear()

lcd.lcd_display_string("Toner already at", 1)
lcd.lcd_display_string("     0 units    ", 2)

sleep(1.5)

lcd.lcd_display_string("     TOTIS      ", 1)
lcd.lcd_display_string("Press When Ready", 2)
