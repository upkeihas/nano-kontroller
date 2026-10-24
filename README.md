# nano-kontroller


### Simple RGB LED controller for Arduino NANO / Uno R3

- Uses Arduino NANO or Uno R3 to drive LED strips connected to PWM pins `3`, `5`, `6`, `9`, `10` and `11`.
- Has IR remote control receiver module listening on Digital pin `2`.
- Uses interrupt-based [TinyIRReceiver](https://github.com/Arduino-IRremote/Arduino-IRremote#tiny-nec-receiver-and-sender) library to free up all 6 PWM channels for driving LEDs.
- LED effect programs can be selected using remote controller.
- PWM channel layout is meant for driving 2 RGB LED strips, so one channel per colour. See the diagram for wiring details.



Currently only supports NEC protocol for IR remote!
The very simple NEC IR protocol is probably the most common one used for LED strips, so there's a good chance that you already own a compatible remote controller.
The ubiquitous Chinese IR remote that shipped with any LED strip "should just work".



```mermaid
flowchart RL
    Arduino(<b>Arduino NANO/Uno R3</b>
            PIN11
            PIN10
            PIN9
            PIN6
            PIN5
            PIN3
            PIN2
            GND
            +5V)
    subgraph Strip 1
    RLed_1@{ shape: delay, label: "RLED_1"}
    GLed_1@{ shape: delay, label: "GLED_1"}
    BLed_1@{ shape: delay, label: "BLED_1"}
    end
    subgraph Strip 2
    RLed_2@{ shape: delay, label: "RLED_2"}
    GLed_2@{ shape: delay, label: "GLED_2"}
    BLed_2@{ shape: delay, label: "BLED_2"}
    end
    subgraph IR
    IRreceiver(IR Remote controller receiver)
    end

    BLed_1 -- "to PIN3 and GND" --> Arduino
    GLed_1 -- "to PIN5 and GND" --> Arduino
    RLed_1 -- "to PIN6 and GND" --> Arduino
    BLed_2 -- "to PIN11 and GND" --> Arduino
    GLed_2 -- "to PIN10 and GND" --> Arduino
    RLed_2 -- "to PIN9 and GND" --> Arduino
    IRreceiver -- "to PIN2, +5V and GND" --> Arduino
```


### References

- https://docs.arduino.cc/tutorials/nano-r4/external-interrupts/
- https://github.com/Arduino-IRremote/Arduino-IRremote
- https://github.com/Arduino-IRremote/Arduino-IRremote#tiny-nec-receiver-and-sender



### Author


- [Ukko-Pekka Keihäs](https://github.com/upkeihas)

