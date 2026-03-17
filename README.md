# RGM Mobility Model for FANETs: ns-3 Implementation

This repository provides the ns-3 source code for the **Regulated Gauss-Markov Mobility Model** for FANETs.

The implementation corresponds to the model presented in the paper:

**“A New GPS Spoofing Attack and a Regulated Gauss-Markov Mobility Model for FANETs”**, published in *Ad Hoc Networks*.

## Files
The implementation consists of the following files:
- `reg-gauss-markov-mobility-model.h`
- `reg-gauss-markov-mobility-model.cc`

## Installation Steps

To integrate the model into ns-3, follow these steps:

1. Copy the following files into: /ns3/src/mobility/model/

  - `reg-gauss-markov-mobility-model.h`
  - `reg-gauss-markov-mobility-model.cc`

2. Place the provided `CMakeLists.txt` file in: /ns3/src/mobility/ (i.e., `CMakeLists.txt` ensures that the `build_lib` command listing the source and header files of the model).

3. Reconfigure ns-3: ./ns3 configure

4. Rebuild ns-3:./ns3 build


## Usage

Refer to `reg-gauss-markov-mobility-model.h` for a commented example demonstrating how to use the model.

## Citation

Please cite this work as follows:

*TBA (to be updated upon publication).*

## Note

**The paper is currently under review. The source code will be fully available upon acceptance.**

## Contact

For more details, please contact Ala Altaweel at altaweel.ala@gmail.com
