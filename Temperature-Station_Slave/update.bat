cd %~dp0
curl -F "image=@Temperature-Station_Slave.ino.generic.bin" -u admin:admin 192.168.2.120/update
pause