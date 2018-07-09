Muxtemp board.

Default address is 0x10. If you don't like it - change it in sketch

!Lock - device is processing data (low state) or it's available (high)
1wire (jumpers closed) - master device will maintain 1wire circuit (Dosen't show specific port used)
1wire (jumpers open) - Muxtemp will maintain 1wire circuit like local sensors
SDA/SCL - I2C
3V3 - fused in case of short
3V3_L - Logic 3V3 (unfused)

Power supply header is used for external power regulator.