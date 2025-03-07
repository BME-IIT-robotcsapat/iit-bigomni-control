# Introduction

Low level control functionalities for the big omnidirectional robot of IIT department. Because of the direct access to the hardware, it has to be run on the robot.

The node waits for the PWM duties (0-255) of the 6 PWM channels that are used and sets each motor GPIO accordingly.

# Usage

The `pigpiod` daemon program has to be started before running the node, with
```bash
sudo pigpiod
```

Then the node can be run by
```bash
ros2 run bigomni_control control_client
```

(Don't forget to source the workspace before usage!)

The `std_msgs/msg/UInt8MultiArray` message interface is used. **IMPORTANT**: the code does not check the size of the data provided, only the data field is used. This makes it possible to send only partly initialized messages but also prone to reading corrupted data. Complete data structure:

- topic: `/control`
- interface type: `std_msgs/msg/UInt8MultiArray`

Interface description :
```yaml
{
  layout: {
      dim: [
        {
          label: 'pwm',
          size: 6,
          stride: 0
        }
      ],
      data_offset: 0
  },
  data: [ <PWM1F, PWM1B, PWM2F, PWM2B, PWM3F, PWM3B]
}
```
Where `PWM<i><d>` stands for:
- `<i>`: motor number (`1`,`2`,`3`)
- `<d>`: direction (`F`: forward, `B`: backward)


# Example command line usage
## Fully defined

Sending the message repeatedly at 10Hz:

```bash
ros2 topic pub -r 10 /control /std_msgs/msg/UInt8MultiArray "{layout: {dim: [{label: 'pwm', size: 6, stride: 0}], data_offset: 0}, data: [120,8,0,128,0,128,0]}"
```

## Minimalist

Sending the message repeatedly at 10Hz:

```bash
ros2 topic pub -r 10 /control /std_msgs/msg/UInt8MultiArray "data: [120,8,0,128,0,128,0]"
```
