# nano-kontroller


### Simple 6 channel RGB LED controller for Arduino NANO / Uno R3

- Uses Arduino NANO or Uno R3 to drive RBG LEDs connected to PWM pins `3`, `5`, `6`, `9`, `10` and `11`.
- Has IR remote control receiver module listening on Digital pin `2`.
- Uses interrupt-based [TinyIRReceiver](https://github.com/Arduino-IRremote/Arduino-IRremote#tiny-nec-receiver-and-sender) library to free up all 6 PWM channels for driving LEDs.
- LED effect programs can be selected using remote controller.
- PWM channel layout is meant for driving 2 RGB LED groups, so one PWM channel per colour. See the diagram for wiring details.


**Warning: Arduino can only safely deliver max 20mA per PWM channel!**

If you need to drive a whole LED strip then you need an external MOSFET setup to handle passing the higher current required.


Currently this controller only supports the NEC IR protocol. The very simple NEC IR protocol is probably the most common one used for LED strips, so there's a good chance that you already own a compatible remote controller. The ubiquitous Chinese IR remote that shipped with any LED strip "should just work".


```mermaid
flowchart RL
    Arduino(<b>Arduino NANO/Uno R3</b>
            +5V
            GND
            PIN2
            PIN3
            PIN5
            PIN6
            PIN9
            PIN10
            PIN11)
    subgraph LED Group 1
    R_1@{ shape: delay, label: "R_1"}
    G_1@{ shape: delay, label: "G_1"}
    B_1@{ shape: delay, label: "B_1"}
    end
    subgraph LED Group 2
    R_2@{ shape: delay, label: "R_2"}
    G_2@{ shape: delay, label: "G_2"}
    B_2@{ shape: delay, label: "B_2"}
    end
    subgraph IR
    IRreceiver(IR Remote controller receiver)
    end

    IRreceiver -- "to PIN2, +5V and GND" --> Arduino
    R_1 -- "to PIN3 and GND" --> Arduino
    G_1 -- "to PIN5 and GND" --> Arduino
    B_1 -- "to PIN6 and GND" --> Arduino
    R_2 -- "to PIN9 and GND" --> Arduino
    G_2 -- "to PIN10 and GND" --> Arduino
    B_2 -- "to PIN11 and GND" --> Arduino
```


### Arduino NANO PIN layout

<img width="1135" height="895" alt="nano_pinout" src="https://github.com/user-attachments/assets/da52a5ca-b87c-4a35-af8c-de632d75a320" />


### References

- https://docs.arduino.cc/tutorials/nano-r4/external-interrupts/
- https://github.com/Arduino-IRremote/Arduino-IRremote
- https://github.com/Arduino-IRremote/Arduino-IRremote#tiny-nec-receiver-and-sender


### Author

- [Ukko-Pekka Keihäs](https://github.com/upkeihas)

