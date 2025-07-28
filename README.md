# Living Off The Land (LOTL) v0.15

**Author:** c0d3Ninja

A comprehensive privilege escalation enumeration tool designed for Linux/Unix security assessments. LOTL helps security professionals identify potential attack vectors by systematically checking for misconfigurations, exposed credentials, vulnerable system components, and provides advanced persistence mechanisms for authorized testing.

## Features

### 🔍 System Enumeration
- **System Information**: Gathers uname, hostname, uptime, and mount information
- **Dependency Detection**: Checks for required and optional system commands
- **SUID Binary Discovery**: Recursively finds SUID binaries across the filesystem
- **Network Scanning**: Built-in port scanner for local network discovery
- **IP Discovery**: Automatic local IP address detection via ifconfig parsing

### 🔑 Credential Discovery
- **SSH Keys**: Locates private keys, known_hosts, and SSH configurations
- **Cloud Credentials**: Searches for AWS credentials and configurations
- **System Credentials**: Examines passwd, shadow, and authentication files
- **Application Secrets**: Finds .env files, API keys, and configuration files
- **Dynamic Content Display**: Automatically displays contents of discovered credential files

### 📋 File Analysis
- **History Files**: Analyzes bash/zsh history for sensitive commands
- **Log Files**: Checks system logs for authentication and security events
- **Backup Files**: Discovers backup copies of sensitive configurations
- **Sensitive Files**: Locates database dumps, private keys, and configuration files
- **Recursive Scanning**: Advanced filesystem traversal with sensitive file detection

### 🛠️ Advanced Tools
- **Port Scanner**: Fast local port scanning capabilities (1-10000 range)
- **Log Clearing**: Automated log cleanup functionality
- **Web Tools**: HTTP-based reconnaissance utilities
- **Base64 Encoding**: Built-in encoding/decoding for payload obfuscation

### 🎯 Persistence Mechanisms (Authorized Testing)
- **Cron Job Backdoors**: Automated reverse shell installation via cron (every 5 minutes)
- **PAM Module Integration**: PAM-based persistence mechanisms
- **SSH Key Injection**: Automated SSH key deployment
- **User Account Creation**: Automated user account backdoors
- **Base64 Payload Encoding**: Obfuscated payload deployment for evasion

### ⚙️ Configuration-Driven
All enumeration targets and backdoor options are defined in `config/lotl.conf`, making the tool easily customizable for different environments and testing scenarios.

## Installation

### Prerequisites
- Linux/Unix system
- g++ compiler with C++20 support
- Standard system utilities (cat, uname, hostname, ifconfig, etc.)
- Root privileges (for some persistence features)

### Build Instructions

```bash
# Clone or navigate to the LivingOfTheLand directory
cd LivingOfTheLand

# Compile the tool
make

# Clean previous builds (optional)
make clean

# Force rebuild (optional)
make rebuild
```

## Usage

### Basic Usage
```bash
# Run the tool (standard enumeration)
./lotl

# Ensure proper permissions for full functionality
sudo ./lotl
```

### Output Sections
The tool provides organized output in the following sections:
1. **Dependency Check**: Shows available system commands
2. **System Information**: Displays system details, IP addresses, and open ports
3. **Sensitive Information**: File system scanning results
4. **Credential Files**: Contents of discovered credential files
5. **History Files**: Contents of shell history files
6. **Log Files**: System log analysis
7. **SSH Files**: SSH configuration and key analysis
8. **Backup Files**: Discovered backup file contents

## Configuration

### Basic Configuration
Edit `config/lotl.conf` to customize enumeration targets and enable persistence features:

```ini
[Dependencies]
required = cat, ifconfig, whoami, uname, hostname, uptime, mount
optional = curl, wget, netstat, ss

[BackDoor]
# Enable/disable backdoor functionality (authorized testing only)
backdoor = no

[BackDoorOptions]
# IP address for reverse connections
ip = 127.0.0.1

# Persistence method selection
pam = no      # PAM module backdoor
cron = yes    # Cron job backdoor (every 5 minutes)
ssh = no      # SSH key injection
user = no     # User account creation

[Credentials]
checkCreds = /etc/passwd, /etc/shadow, ~/.aws/credentials, ~/.ssh/id_rsa

[bashHistory]
checkBashHistory = ~/.bash_history, ~/.zsh_history, ~/.bashrc

[Logs]
checkLogs = /var/log/auth.log, /var/log/secure, /var/log/messages

[SSH]
checkSSH = ~/.ssh/id_rsa, ~/.ssh/known_hosts, ~/.ssh/config

[BackUps]
CheckBackUp = *.bak, *.backup, *.old, *.save

[SensitiveFiles]
checkSensitive = .env, *.conf, *.config, *.json
```

### Configuration Sections
- **Dependencies**: Required and optional system commands
- **BackDoor**: Enable/disable persistence features
- **BackDoorOptions**: Configure persistence mechanisms and target IP
- **Credentials**: Paths to credential files
- **Logs**: System log file locations
- **SSH**: SSH-related files and configurations
- **bashHistory**: Shell history and configuration files
- **BackUps**: Backup file patterns and locations
- **SensitiveFiles**: Application configs and sensitive data

## Project Structure

```
LivingOfTheLand/
├── main.cpp              # Entry point and argument processing
├── core/
│   ├── init.cpp          # Core enumeration and persistence functionality
│   ├── init.h            # Core header definitions
│   └── init.o            # Compiled object
├── modules/
│   ├── executils.cpp     # Command execution utilities
│   ├── executils.h       # Execution utility headers
│   ├── parsers.cpp       # Configuration file parsing
│   ├── parsers.h         # Parser headers
│   ├── base64.cpp        # Base64 encoding/decoding implementation
│   └── base64.h          # Base64 headers
├── tools/
│   ├── suids.cpp         # SUID binary discovery
│   ├── suids.h           # SUID tool headers
│   ├── portscanner.cpp   # Network port scanning
│   ├── portscanner.h     # Port scanner headers
│   ├── clearlogs.cpp     # Log clearing functionality
│   ├── clearlogs.h       # Log cleaner headers
│   ├── web.cpp           # Web reconnaissance tools
│   └── web.h             # Web tool headers
├── config/
│   └── lotl.conf         # Main configuration file
├── Makefile              # Build configuration
├── README.md             # This documentation
└── lotl                  # Compiled binary
```

## Advanced Features

### Port Scanning
The built-in port scanner automatically detects the local IP address and scans ports 1-10000:
- Fast TCP connection-based scanning
- Automatic IP detection via ifconfig parsing
- Results integrated into system information output

### Persistence Mechanisms
**⚠️ AUTHORIZED TESTING ONLY ⚠️**

The tool includes several persistence mechanisms for authorized penetration testing:

#### Cron Job Backdoor
```bash
# Automatically creates a cron job that executes every 5 minutes
# Uses base64 encoding for payload obfuscation
*/5 * * * * root bash -c "$(echo <base64_payload> | base64 -d)"
```

#### Configuration Example
```ini
[BackDoor]
backdoor = yes

[BackDoorOptions]
ip = <your_listener_ip>
cron = yes
```

### Base64 Payload Encoding
All reverse shell payloads are automatically base64 encoded for:
- Evasion of basic detection mechanisms
- Cleaner cron job entries
- Reduced signature-based detection

## Security Considerations

### Responsible Usage
- **Authorized Testing Only**: Use only on systems you own or have explicit written permission to test
- **Legal Compliance**: Ensure compliance with all applicable laws and regulations
- **Professional Context**: Intended for penetration testing, security assessments, and educational purposes
- **Clean Up**: Always remove any persistence mechanisms after testing

### Ethical Guidelines
- Document all changes made to target systems
- Provide clear remediation guidance
- Use minimal necessary privileges
- Follow responsible disclosure practices

### Detection and Cleanup
- Cron jobs are created in `/etc/cron.d/rev`
- Monitor for base64-encoded cron entries
- Check for unauthorized user accounts
- Review SSH authorized_keys files
- Examine PAM configurations for modifications

### Limitations
- Some checks require elevated privileges
- Persistence features require root access
- Permission-denied errors are handled gracefully
- Results depend on file system permissions and access controls

## Development

### Building with Debug Info
```bash
# The Makefile includes debug flags by default
make clean && make
```

### Adding New Features
1. Update `config/lotl.conf` with new configuration options
2. Modify parsing logic in `modules/parsers.cpp`
3. Add implementation in `core/init.cpp`
4. Create new tool modules in the `tools/` directory as needed
5. Update Makefile to include new source files

### Code Structure
- **Modular Design**: Separate modules for different functionalities
- **Configuration-Driven**: All options controlled via conf file
- **Error Handling**: Graceful handling of permission and file access errors
- **Cross-Platform**: Designed for Unix-like systems

## Version History
- **v0.11**: Current release with advanced persistence mechanisms, base64 encoding, port scanning, and comprehensive file analysis
- **v0.3**: Enhanced SUID detection and configuration-driven checks

## Disclaimer

**⚠️ IMPORTANT SECURITY NOTICE ⚠️**

This tool is intended **EXCLUSIVELY** for legitimate security testing, penetration testing, and educational purposes. The persistence mechanisms and backdoor functionality included in this tool are powerful and potentially destructive.

### Legal and Ethical Use
- Only use on systems you own or have explicit written authorization to test
- Ensure compliance with all applicable local, state, and federal laws
- Follow your organization's security testing policies and procedures
- Document all testing activities and system modifications

### Liability
The author assumes **NO LIABILITY** for:
- Misuse or unauthorized use of this software
- Damage caused to systems or networks
- Legal consequences resulting from improper use
- Data loss or system compromise

### Professional Responsibility
Users are **SOLELY RESPONSIBLE** for:
- Obtaining proper authorization before use
- Ensuring legal compliance in their jurisdiction
- Removing all persistence mechanisms after testing
- Following responsible disclosure practices
- Maintaining professional ethical standards

## License

This software is provided for educational and authorized security testing purposes. Use responsibly, ethically, and in compliance with all applicable laws. Respect system owners' rights and maintain the highest professional standards in cybersecurity practice. 
