# Emergency Light Unit
This is an project helps in accident situation. It do normally charges and control the situation in night when there is no source of energy left
## Overview

The Emergency Light Unit is a high-reliability LED-based illumination and emergency indication system designed for railway passenger coaches. The unit provides emergency lighting and exit guidance during power interruptions, ensuring passenger safety and compliance with railway safety standards.

The product is designed for operation in harsh railway environments, including vibration, shock, humidity, dust, and wide input voltage fluctuations. It incorporates energy-efficient LEDs, an electronic constant-current driver, and a rugged enclosure suitable for rolling stock applications.

---

## Features

* Energy-efficient LED technology
* Long operational life (>50,000 hours)
* Maintenance-free design
* High reliability under railway operating conditions
* Wide input voltage range
* Low power consumption
* EMI/EMC compliant driver
* IP65 protected enclosure
* Shock and vibration resistant construction
* Uniform light distribution with glare-free optics
* Fire-retardant and halogen-free materials
* Suitable for emergency exit indication and emergency illumination

---

## Applications

* Passenger coaches
* AC and Non-AC coaches
* LHB coaches
* EMU/MEMU coaches
* DEMU coaches
* Metro coaches
* Emergency exit indication systems
* Safety and evacuation lighting

---

## Technical Specifications

| Parameter          | Specification                          |
| ------------------ | -------------------------------------- |
| Light Source       | High-efficiency White LEDs             |
| LED Life           | ≥ 50,000 hours                         |
| Input Voltage      | 90–140 V AC/DC                         |
| Operating Supply   | 110 V AC/DC Railway System             |
| Protection Class   | IP65                                   |
| Power Consumption  | As per approved design                 |
| Colour Temperature | 6000K – 7000K (Cool Day White)         |
| Power Factor       | >0.90                                  |
| THD                | <15% (up to 4 W)                       |
| Driver Type        | Constant Current LED Driver            |
| LED Beam Angle     | ≥120°                                  |
| Housing Material   | Powder-coated steel/aluminium          |
| Diffuser Material  | Fire-retardant Polycarbonate (UL94-V0) |

---

## Environmental Conditions

| Parameter                   | Value                               |
| --------------------------- | ----------------------------------- |
| Maximum Ambient Temperature | +55°C                               |
| Minimum Ambient Temperature | -5°C                                |
| Relative Humidity           | Up to 98%                           |
| Altitude                    | Up to 1200 m                        |
| Train Speed                 | Up to 200 km/h                      |
| Dust Environment            | Severe Railway Conditions           |
| Coastal Environment         | Salt-laden and Corrosive Atmosphere |

---

## Electrical Protection

The Emergency Light Unit includes:

* Over-voltage protection
* Reverse polarity protection
* Short-circuit protection
* Surge protection
* Thermal protection
* Fuse protection on input side

---
## Serial Debugging 

05/03/40 | 20:00:16 | | S:1 | SV: 12.14|BV:12.06| Ch: +024| Ch:1|DM:0|DL:0[ T:0|CL:0| HL:0 | BBH:0[ 2HFLT|S00:00 A02:00 E00:00| | BØ:12.06,B1:12.06, dvdt: 00.00/1= 00.00| lvb=0, lvbS=0 
BF: 00000 LC:1<LF> 
<LF: 
05/03/40 | 20:00:16 | S: 1 |SV: 12.14|BV: 12.06 |Ch: +024|Ch: 1| DM: 0|DL: 0[ T:0 |CL:0| HL: 0| BBH: 0[ 2HELT | 500:00 A02:00 E00:00 |B0:12.06, B1:12.06, dvdt: 00.00/1= 00.00 |lvb=0, lvbS=0 
BF: 00000 LC:1<LF> 
<LF> 
05/03/40 |20:00:16|S:1|SV: 12.14|BV: 12.06 | Ch: +024 |ch: 1 |DM: 0|DL : 0[ T : 0 |CL:0|HL:0|BH:0[ 2HFLT|500:00 A02:00 00:00|B0:12.06,81:12.06,dvdt: 00.00/1= 00.00|1vb=0,lvbS=0 
|BF: 00000 LC:1<LF> 
‹LF 
05/03/40 | 20:00:16 | | S:1| SV:12.14|BV:12.06 | Ch: +024| Ch:1| DM:0|DL:0[ T:0|CL:0| HL:0 | BBH:0[ 2HFLT |S00:00 A02:00 E00:00| | B0:12.06,B1:12.06, dvdt: 00.00/1= 00.00| lvb=0, lvbS=0 
BF: 00000 LC:1<LF> 
<LF> 
05/03/40 | 20:00:16 | | S:1 | SV:12.14|BV:12.06 | Ch: +024| Ch:1|DM:0|DL:0[ T:0|CL:0|HL:0 | BBH:0[ 2HFLT |S00:00 A02:00 E00:00| | BØ:12.06,B1:12.06, dvdt: 00.00/1= 00.00| lvb=0, 1vbS=0 
|BF: 00000 LC:1<LF> 
<LF> 
05/03/40|20:00:16|S:1|SV: 12.14|BV: 12.06 |Ch: +024|Ch:1|DM:0|DL:0[ T:0|CL:0|HL:0|BH:0[ 2HFLT|500:00 A02:00 E00:00|B0:12.06,81:12.06,dvdt: 00.00/1= 00.00|1vb=0,lvbS=0 
BF: 00000 LC:1<LF> 
<LF 
05/03/40 | 20:00:16 | S:1 | SV: 12.14|BV: 12.05 | Ch: +024|Ch: 1| DM: 0|DL: 0[ T:0| CL: 0| HL: 0| BH: 0[ 2HELT| 500:00 A02:00 E00:00|B0:12.06,81:12.05, dvdt: - 00.01/1= - 00.01|lvb=0,lvbS=0 
BF: 00000 LC:1<LF> 

where 
     SV - Supply voltae
     S - Supply Enable 110V 
     BV - Current Battery voltage 
     CH  - Charging current 
     DM - Dummy load enable or dissable to discharge the Battery in descharge cycle at 1st and 16th of each mounth
     HL == High lux ...as light start glow will glow for 2 houres at high lux and then reamin at low lux as per spec
     Dv/Dt - rate ofcharge and discharge  if abnormally high hen declair battery is not healthy
     
## Mechanical Construction

* Rugged railway-grade construction
* Anti-corrosion powder-coated finish
* Vibration-resistant mounting arrangement
* Dust-proof and splash-proof enclosure
* Lightweight design
* Secure electrical connectors
* Low-smoke, halogen-free internal wiring

---

## Safety Compliance

The unit is designed to comply with applicable railway and lighting standards including:

* RDSO Railway Lighting Specifications
* IEC 60529 (IP Protection)
* IEC 60571 (Railway Electronic Equipment)
* IEC 61373 (Shock & Vibration)
* IEC 61000 (EMC)
* IEC 60598 (Luminaire Safety)
* IEC 62471 (Photobiological Safety)
* LM-80 / TM-21 LED Life Standards

---

## Testing

The Emergency Light Unit undergoes:

1. Visual & Dimensional Inspection
2. Insulation Resistance Test
3. High Voltage Test
4. Over Voltage Protection Test
5. Temperature Rise Test
6. Lux Measurement Test
7. Fire Retardancy Test
8. IP65 Protection Test
9. Shock & Vibration Test
10. Environmental Testing
11. EMI/EMC Testing
12. Endurance Testing

---

## Installation

1. Mount the unit using the approved railway mounting arrangement.
2. Connect input supply according to wiring diagram.
3. Verify polarity and supply voltage.
4. Ensure proper grounding where applicable.
5. Perform functional testing after installation.

---

## Maintenance

The unit is designed for maintenance-free operation. Periodic inspection is recommended for:

* Physical damage
* Mounting integrity
* Electrical connections
* Lens cleanliness
* Functional operation

---

## Advantages

* Improved passenger safety
* Reduced energy consumption
* Long service life
* Lower maintenance costs
* Enhanced reliability
* Environment-friendly operation

---

## Revision History

| Version | Date            | Description                 |
| ------- | --------------- | --------------------------- |
| 1.0     | Initial Release | Emergency Light Unit README |
