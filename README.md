## IoTware
_IoTware Project_ is a research project funded by national government. The IoTware Project aims to develop a small IoT framework for providing microservices composed of tasks in IoT devices. The project was started in June 2018 and is currently under development.

### IoTware Modules
* `MICROSERVICE\SIFW`: Service Infra Interworking framework module that communicates with infra network
* `MICROSERVICE\SMFW`: Service mission framework module that manages microservices composed of a combination of tasks  
* `MICROSERVICE\ROFW`: Resource operation framework module that managse device power
* `MICROSERVICE\DCFW`: Distributed collaboration framework module to communiate with members of distributed collaboration cluster
* `oal`: Common and basic embedded software codes such as bsp, drivers, interface, iotware oal(OS abstration layer)

### Dependency
* [FreeRTOS](https://www.freertos.org/) (> 8.2.1, [GPL 2.0](https://www.freertos.org/a00114.html))
  * _FreeRTOS_ is a real-time operating system kernel for embedded devices that has been ported to 35 microcontroller platforms.
* [NanoQplus] (> 3.0.0, [GPL 2.0])
  * _NanoQplus_ is a small operating system for sensor network nodes made by the Electronics and Telecommunications Research Institute (ETRI).
* [RIOT](https://www.riot-os.org/) (> 2018.04, [GPL 2.1](https://www.riot-os.org/))
  * _RIOT_ is a small operating system for networked, memory-constrained systems with a focus on low-power wireless Internet of Things (IoT) devices.
* [Contiki](https://www.contiki-ng.org/) (> 3.1, [3-clause BSD License](https://www.contiki-ng.org/))
  * _Contiki_ is an operating system for networked, memory-constrained systems with a focus on low-power wireless IoT devices.
* [apache mynewt](http://mynewt.apache.org/) (> 1.8.0, [Apache License 2.0](http://mynewt.apache.org/))
  * _Apache Mynewt_ is a modular real-time operating system for connected IoT devices that must operate for long times under power, memory, and storage constraints.

### How to Run Codes
* **Microsoft Windows**
  * Prerequisite
    * [openocd](http://openocd.org/): Fusing Tools
    * Chrome (85.0.4183.121_64bit version or higher)
  * URL access using Chrome.
    * www.iotware-ide.namowebiz.com 
  * Log in to IoTWare IDE.
  * Create Workspace, Project, Microservice.
  * Build between Microservice and Framework and Library.
  * Download IoTWare Application and Fusing Tools on your PC.
  * Fuse IoTWare Application into target device via fusing tools.

### Tutorials
* YouTube
  * https://www.youtube.com/channel/UC_-0Usaw4Esra6PDRufk3EA

### History
* IoTwareFW_v1.0.zip is updated on October 22, 2020

### How to Contribute Your Codes
Please refer [CONTRIBUTING.md](CONTRIBUTING.md).

### Authors
Please refer [AUTHORS.md](AUTHORS.md).

### License
Please refer [IoTware Project LSA](LICENSE.md).

### Acknowledgement
The authors thank the following contributors and projects.

* This work was supported by Institute of Information & communications Technology Planning & Evaluation (IITP) grant funded by the Korea government(MSIT) (2018-0-01411, A Micro-Service IoTWare Framework Technology Development for Ultra small IoT Device) .
