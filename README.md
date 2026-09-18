# Tempic
Small 50mm x 50mm solar powered ESP32c6 based indoor/outdoor WiFi/Zigbee sensor node.

# Features
- Onboard ESP32 C6 mini with
    - WiFi 6 (2.4 GHz)
    - Zigbee 3
    - Bluetooth 5.3

- BME688
    - Temperature
    - Humidity
    - Pressure
    - CO2 
    
- BMV080 (for outdoor use)
    - P.M. 1.0
    - P.M. 2.5
    - P.M. 10

- Small 5x5cm solar panel
- Small 5x5cm LiPo battery

Goal is easy Home Assistant integration with unlimited battery life and cable-less deployment.
# Roadmap
- [ ] Hardware 1.0
    - [x] Find suitable ICs
    - [x] Schematics 1.0
    - [x] Draw PCB 1.0
    - [ ] Order prototype
    - [ ] Assemble the PCB
    - [ ] Test PCB
- [ ] Firmware
    - [ ] Basic test and demo of sensors
    - [ ] Battery life optimization
    - [ ] Add Wifi
    - [ ] Setup connection to Home Assistant
    - [ ] Add Zigbee
    - [ ] (far) Add BLE peer to peer communication for bigger meshes
- [ ] Home assistant
    - [ ] Figure out what is the best way to make it work (do not know if any custom plugins are necessary)
