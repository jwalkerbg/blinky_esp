
Layout

Windows
\zephyr\
    zws\    # the workspace, arbitrary name
        .vest
        .vscode
        bootloader
        modules
        tools
        zephyr      # zephyr SDK
        blinky_esp  # the project

Linux
/path_to_zephyr_root_directory
    zws/    # the workspace, arbitrary name
        .vest
        .vscode
        bootloader
        modules
        tools
        zephyr      # zephyr SDK
        blinky_esp  # the project

Install (if not installed) `esptool` into the activated virtual environment

Activate Virtual environment

cd /path_to_zephyr_root_directory/zws
.venv/Scripts/activate
or
.venv/Scripts/Activate.ps1

Build:
west build -p -b  esp32s3_devkitm/esp32s3/procpu -- -DEXTRA_DTC_OVERLAY_FILE='esp32_devkitc_wroom.overlay'
Flash:
esptool --chip esp32s3 --port COM3 --baud 460800 write_flash 0 build\zephyr\zephyr.bin
