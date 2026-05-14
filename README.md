# NetScanner-C

A simple network scanner made using C

## Start & Usage

### "Prerequisites"

- A C compiler 
- 'make' build utility
- Linux/Unix environment

### 1. Build the Tool ! 

Clone the repository and compile the project using 'make'.

### 2. Run it!

provide the target IP,the starting port, and the ending port.

```c
./bin/netscanner 192.168.1.1 20 100
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

- [ ] Multithreading: Implement pthreads to scan hundreds of ports simultaneously, drastically reducing scan time.

- [ ] CSV Export: Add a flag to save scan results to a file for later analysis.

- [ ] UDP Support: Expand scanning capabilities to include connectionless UDP protocols.

## **IMPORTANT**

**This project is for educational purposes. Only use it on networks and devices you own or have explicit permission to test.**