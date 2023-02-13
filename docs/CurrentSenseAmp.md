# MAX4071 (Maxim Integrated)

## Pin assignment

(8pin uMAX/uSOP package)
| Pin number | Name | Description |
| - | - | - |
| 1 | nSHDN | Pull-up |
| 2 | RS- | Shunt resistor - |
| 3 | RS+ | Shunt resistor + |
| 4 | GND | GND |
||||
| 5 | REFOUT | NC |
| 6 | OUT | Controller ADC |
| 7 | VCC | VCC |
| 8 | GSEL | GND (50V/V gain) |

## Design

### Requirements and conditions

+ Power supply voltage is supplied from the CPU VCC (+3.3V).
+ Current sensing part shall be designed to measure currents in the range of -3.0A to +3.0A ($I_{MAX} = 3.0 \mathrm{A}$).
+ With the MAX4071, the amplifier gain can be selected the amplifier gain from 50x or 100x.
+ Internal reference voltage $V_{REF}$ is 1.5V.
+ Maximum input voltage to the CPU $V_{MAX}$ is 3.0V (twice the reference voltage for simplicity). It should be VMAX < VCC.
+ Shutdown (nSHDN) and reference voltage output (REFOUT) function are not used.

### Parameters determined

+ The gain is selected as 50x (for frequency response reason).
+ Range of the measuring voltage $V_{SENSE}$ is from -30mV to +30mV.  
  $|V_{SENSE}| = (V_{MAX} - V_{REF}) / Gain$
+ Shunt resistor $R_{SENSE}$ is 10mOhm.  
  $R_{SENSE} = |V_{SENSE}|/I_{MAX}$
+ From above, the output voltage corresponding to the input current of -3.0A to +3.0A is  0V to +3.0V.  
  $V_{OUT} = (V_{SENSE} \times Gain) + V_{REF}$  
  $V_{SENSE} = -30 \mathrm{mV} \sim +30 \mathrm{mV}$  
  $Gain = 50 \mathrm{x}$  
  $V_{REF} = 1.5 \mathrm{V}$
