# ttyX_Getter
A tool that help you get the device number of your ttyX decices on USB bus

## Required library

* libudev-dev

## How to use

1. Use udevadm to get the target device and its unique property.

    For example, I connect a USB2TTL module, and it is /dev/ttyUSB0 in my system.
    ```SHELL
    $ udevadm info -n /dev/ttyUSB0
    P: /devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.0/ttyUSB0/tty/ttyUSB0
    N: ttyUSB0
    L: 0
    S: serial/by-path/pci-0000:00:14.0-usb-0:1:1.0-port0
    S: serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0
    E: DEVPATH=/devices/pci0000:00/0000:00:14.0/usb1/1-1/1-1:1.0/ttyUSB0/tty/ttyUSB0
    E: DEVNAME=/dev/ttyUSB0
    E: MAJOR=188
    E: MINOR=0
    E: SUBSYSTEM=tty
    E: USEC_INITIALIZED=19133224608
    E: ID_BUS=usb
    E: ID_VENDOR_ID=10c4
    E: ID_MODEL_ID=ea60
    E: ID_PCI_CLASS_FROM_DATABASE=Serial bus controller
    E: ID_PCI_SUBCLASS_FROM_DATABASE=USB controller
    E: ID_PCI_INTERFACE_FROM_DATABASE=XHCI
    E: ID_VENDOR_FROM_DATABASE=Silicon Labs
    E: ID_MODEL_FROM_DATABASE=CP210x UART Bridge
    E: ID_VENDOR=Silicon_Labs
    E: ID_VENDOR_ENC=Silicon\x20Labs
    E: ID_MODEL=CP2102_USB_to_UART_Bridge_Controller
    E: ID_MODEL_ENC=CP2102\x20USB\x20to\x20UART\x20Bridge\x20Controller
    E: ID_REVISION=0100
    E: ID_SERIAL=Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001
    E: ID_SERIAL_SHORT=0001
    E: ID_TYPE=generic
    E: ID_USB_INTERFACES=:ff0000:
    E: ID_USB_INTERFACE_NUM=00
    E: ID_USB_DRIVER=cp210x
    E: ID_PATH=pci-0000:00:14.0-usb-0:1:1.0
    E: ID_PATH_TAG=pci-0000_00_14_0-usb-0_1_1_0
    E: ID_MM_CANDIDATE=1
    E: DEVLINKS=/dev/serial/by-path/pci-0000:00:14.0-usb-0:1:1.0-port0 /dev/serial/by-id/usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0
    E: TAGS=:systemd:
    ```

    The line started with ```E``` is the property of the device. Choose an unique property and it will be used in the second step. Here I use ```ID_SERIAL=Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001``` and ```ID_SERIAL_SHORT=0001```.
  
    **Note: Chances are that many of your devices have several properties because of mass production. Therefore, if you are going to connect at least two identical devices, then you should pick an unique property** 

2. Create your cfg file

    The cfg file should follow the following format.
    Empty line between devices is required.
    Space between ```DeviceName```, ```${DeviceName}``` and ```Property```, ```${PropertyName}```, ```${PropertyValue}``` is also requred.

    ```
    # Line start with # will be ignored
    DeviceName : USB2TTL1
    Property : ID_SERIAL : Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001
    Property : ID_SERIAL_SHORT : 0001

    DeviceName : ...
    ...
    ```

    You can follow the example cfg in the project.

3. Write program. There is an example c++ code and CMakeLists.txt in project directory. 

    The result:

    ```
    ------------ Load Device cfg ------------
    [Device Search] USB2TTL1
    [Device Search] ID_SERIAL : Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001
    [Device Search] ID_SERIAL_SHORT : 0001

    ------------ Search Device ------------
    [Device Search] Looking for device: USB2TTL1 ...
    [Device Search] USB2TTL1 found, DEVNAME: /dev/ttyUSB0

    ------------ Search Result ------------
    USB2TTL1 : /dev/ttyUSB0
    ------------ Search Result2 ------------
    USB2TTL1 : /dev/ttyUSB0
    ```
