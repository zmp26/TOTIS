import lcddriver
from time import *

lcd = lcddriver.lcd()

lcd.lcd_clear()

lcd.lcd_display_string("     Error:     ", 1)
lcd.lcd_display_string("Invalid  Barcode", 2)

sleep(2.5)

lcd.lcd_display_string("     TOTIS      ", 1)
lcd.lcd_display_string("Press When Ready", 2)
