# RGM Mobility Model for FANETs: ns-3 Implementation

This repository provides the ns-3 source code for the **Regulated Gauss-Markov Mobility Model** for FANETs.

The implementation corresponds to the model presented in the paper:

[**A new GPS spoofing attack and a Regulated Gauss–Markov mobility model for FANETs**](https://www.sciencedirect.com/science/article/pii/S1570870526001332), published in ***Ad Hoc Networks***.

## Citation

If you use this repository, please cite our paper as follows:

```bibtex
@article{ALTAWEEL_AdHocNetworks_2026,
  title = {A new GPS spoofing attack and a Regulated Gauss–Markov mobility model for FANETs},
  journal = {Ad Hoc Networks},
  volume = {189},
  pages = {104267},
  year = {2026},
  issn = {1570-8705},
  doi = {https://doi.org/10.1016/j.adhoc.2026.104267},
  url = {https://www.sciencedirect.com/science/article/pii/S1570870526001332},
  author = {Ala Altaweel and Bilal Arain and Abdulaziz Abushawish and Ibrahim Kamel},
  keywords = {GPS spoofing attack, UAVCollide attack, Flying Ad-Hoc Network (FANETs), Gauss–Markov model, Regulated Gauss–Markov model}
} 
```

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

## Contact

For more details, please contact Ala Altaweel at altaweel.ala@gmail.com
