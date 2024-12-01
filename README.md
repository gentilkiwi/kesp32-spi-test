# kesp32-spi-test

### Build

Set chip target (only `esp32s2` and `esp32s3` are supported for now):
```
idf.py set-target esp32s2
```

Optional: adjust settings (CPU speed, Flash size, etc.):
```
idf.py menuconfig
```

Finally:
```
idf.py build
```

### Flash

Flash the result (add `--port /dev/ttyS22`/`--port COM22` and/or `monitor` if needed):
```
idf.py flash
```

### DFU

If needed, you can convert program to DFU with:
```
idf.py dfu
```
The result is in file: `build/dfu.bin`

Then flash it with:
```
idf.py dfu-flash
```
Requires to `BOOT+RESET`, and may need `libusb` driver replace on Windows

## Author
Benjamin DELPY `gentilkiwi`, you can contact me on Twitter ( @gentilkiwi ) or by mail ( benjamin [at] gentilkiwi.com )