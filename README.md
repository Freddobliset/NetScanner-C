# NetScanner-C

A simple network scanner made using C


## Features

- **Concurrent Scanning**: Utilizes `pthreads` to scan hundreds of ports in parallel, reducing scan times from minutes to seconds.
- **TCP Connect & Banner Grabbing**: Reliable detection of open ports with automatic service identification via banner grabbing.
- **UDP Scanning Support**: Includes a specialized UDP engine that handles ICMP "Destination Unreachable" signals via connected datagram sockets.
- **CSV Data Export**: Automatically generates `scan_results.csv` with quotation-bounded fields for easy analysis in Excel or Python/Pandas.
- **DNS Resolution**: Native support for scanning both raw IP addresses and hostnames.

## Start & Usage

### "Prerequisites"

- A C compiler 
- 'make' build utility
- Linux/Unix environment

### 1. Build the Tool ! 

Clone the repository and compile the project using 'make'.

### 2. Run it !

provide the target IP,the starting port, and the ending port.

```c
./bin/netscanner 192.168.1.1 20 100
```
### 2.1 Run a UDP Scan
Scanning UDP requires elevated privileges to intercept ICMP error packets.

```c
sudo ./bin/netscanner 192.168.1.1 1 100 --udp
```

### 3. Output example

```
Scanning Target: 192.168.1.1
PORT    | STATUS   | SERVICE/BANNER
--------------------------------------------------
22      | OPEN     | SSH-2.0-OpenSSH_8.9p1 Ubuntu...
53      | OPEN     | <no response>
80      | OPEN     | HTTP/1.0 200 OK..Server: ZTE web server 1.0...
--------------------------------------------------
Scan finished.
```

## Project Architecture

```
netscanner/
├── bin/                # Compiled binary executable
├── include/            # Header files
│   └── scanner.h       
├── src/                # Source code
│   ├── main.c          # Starts here (CLI & Loops)
│   └── scanner.c       # Core networking & Sockets
└── Makefile            # Build instructions
```

## Current Features

- TCP Connect Scanning: Reliable detection of open ports using the Berkeley Sockets API.

- Service Banner Grabbing: Captures and sanitizes response data from open ports to identify underlying services.

- Smart Timeouts: Implements SO_RCVTIMEO to ensure the scanner remains fast even when encountering silent or filtered ports.

- HTTP Probing: Sends generic HTTP requests to trigger responses from quiet web servers.

## Roadmap (coming soon)

- [X] DNS Resolution: Add support for hostnames (e.g., google.com) instead of just IP addresses.

- [X] Multithreading: Implement pthreads to scan hundreds of ports simultaneously, drastically reducing scan time.

- [X] CSV Export: Add a flag to save scan results to a file for later analysis.

- [X] UDP Support: Expand scanning capabilities to include connectionless UDP protocols.

- [X] Thread Pooling: Implement a fixed-size worker pool to manage resource consumption during massive port ranges (e.g., 65,535 ports).

- [ ] IP Range Scanning: Support CIDR notation (e.g., 192.168.1.0/24) to scan entire networks.

- [ ] OS Fingerprinting: Analyze TCP/IP stack quirks to guess the operating system of the target.

- [ ] JSON Output: Add support for JSON formatting for integration with modern web APIs and tools.

## **IMPORTANT**

This tool is developed for **educational and ethical security testing purposes only**. 

The author of this project takes no responsibility for any misuse, damage, or illegal activities conducted with this software. Unauthorized scanning of networks or devices without explicit permission is illegal and unethical. Use this tool only on infrastructure you own or have written consent to test.
