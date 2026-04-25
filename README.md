# RGM Mobility Model for FANETs: ns-3 Implementation

This repository provides the ns-3 source code for the **Regulated Gauss-Markov Mobility Model** for FANETs.

The implementation corresponds to the model presented in the paper:

***“A new GPS spoofing attack and a Regulated Gauss–Markov mobility model for FANETs”***, published in ***Ad Hoc Networks***.

## Citation

If you use this repository, please cite our paper as follows:

```bibtex
@article{ALTAWEEL_AdHocNetworks_2026,
  title = {A new GPS spoofing attack and a Regulated Gauss--Markov mobility model for FANETs},
  journal = {Ad Hoc Networks},
  pages = {104267},
  year = {2026},
  issn = {1570-8705},
  doi = {https://doi.org/10.1016/j.adhoc.2026.104267},
  url = {https://www.sciencedirect.com/science/article/pii/S1570870526001332},
  author = {Ala Altaweel and Bilal Arain and Abdulaziz Abushawish and Ibrahim Kamel},
  keywords = {GPS spoofing attack, UAVCollide attack, Flying Ad-Hoc Network (FANETs), Gauss--Markov model, Regulated Gauss--Markov model},
  abstract = {This paper presents a novel GPS spoofing attack on Flying Ad-Hoc Networks (FANETs) called UAVCollide. In this attack, a malicious attacker redirects victim UAVs to a specific target location to maximize in-air collisions. A mathematical formulation and detailed description of the steps required to conduct UAVCollide are provided. The Gauss--Markov (GM) mobility model, commonly used in FANET research, was first used to study the impact of UAVCollide. However, the GM model’s inherent randomness can cause UAV paths to intersect, resulting in collisions even without the attack. Therefore, a novel Regulated Gauss--Markov (RGM) mobility model is proposed to address the limitations of the standard Gauss--Markov model in FANET applications. RGM employs altitude-based separation to generate realistic UAV trajectories while ensuring collision-free operation of FANET under normal (non-attack) scenarios. The RGM model’s mathematical framework is presented and implemented in the NS3 simulator. Using RGM, the UAVCollide attack is analyzed using three parameters: attack time, attacker coverage space, and attack duration. These parameters are evaluated using three performance metrics: number of collisions, collision latency, and deviated area. The effects of UAV speed and mobility patterns on FANET vulnerability to UAVCollide are examined, and a detailed analysis of the results from both the attacker’s and network’s perspectives is provided. The results indicate that attacker coverage space and attack duration are the most influential parameters, and that FANET vulnerability to UAVCollide increases with UAV speed.}
}```


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
