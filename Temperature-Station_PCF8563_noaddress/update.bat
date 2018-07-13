cd %~dp0
curl -F "image=@Temperature-Station_PCF8563_noaddress.ino.nodemcu.bin" -u admin:admin 192.168.2.98/update
pause