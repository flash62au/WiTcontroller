echo;
powershell -sta -ExecutionPolicy Unrestricted -Command "Get-WmiObject -Class Win32_PnPEntity -Filter \"PNPClass='Ports'\" | select Name,Manufacturer,DeviceID"
set /P PORT="WiFi DCC COM Port Number ?"

.\esptool.exe --chip esp32c3 --port COM%PORT% --baud 921600 ^
	--before default_reset --after hard_reset ^
	write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB ^
	0x0 P008.ino.bootloader.bin ^
	0x8000 P008.ino.partitions.bin ^
	0xe000 boot_app0.bin ^
	0x10000 P008.ino.bin 
pause