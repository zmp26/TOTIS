import lcddriver

lcd = lcddriver.lcd()

lcd.lcd_clear()

lcd.lcd_display_string("  Scan Barcode  ", 1)
lcd.lcd_display_string("      Now       ", 2)
