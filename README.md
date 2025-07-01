west build -p -b  esp32s3_devkitm/esp32s3/procpu -- -DEXTRA_DTC_OVERLAY_FILE='esp32_devkitc_wroom.overlay'
esptool --chip esp32s3 --port COM3 --baud 460800 write_flash 0 build\zephyr\zephyr.bin
