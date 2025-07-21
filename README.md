# Living Off The Land (LOTL) v0.11

**Author:** c0d3Ninja

A comprehensive privilege escalation enumeration tool designed for Linux/Unix security assessments. LOTL helps security professionals identify potential attack vectors by systematically checking for misconfigurations, exposed credentials, and vulnerable system components.

## Features

### 🔍 System Enumeration
- **System Information**: Gathers uname, hostname, uptime, and mount information
- **Dependency Detection**: Checks for required and optional system commands
- **SUID Binary Discovery**: Recursively finds SUID binaries across the filesystem

### 🔑 Credential Discovery
- **SSH Keys**: Locates private keys, known_hosts, and SSH configurations
- **Cloud Credentials**: Searches for AWS credentials and configurations
- **System Credentials**: Examines passwd, shadow, and authentication files
- **Application Secrets**: Finds .env files, API keys, and configuration files

### 📋 File Analysis
- **History Files**: Analyzes bash/zsh history for sensitive commands
- **Log Files**: Checks system logs for authentication and security events
- **Backup Files**: Discovers backup copies of sensitive configurations
- **Sensitive Files**: Locates database dumps, private keys, and configuration files

### ⚙️ Configuration-Driven
All enumeration targets are defined in `config/lotl.conf`, making the tool easily customizable for different environments.

## Installation

### Prerequisites
- Linux/Unix system
- g++ compiler with C++20 support
- Standard system utilities (cat, uname, hostname, etc.)

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
# Run the tool
./lotl
```

### Output Sections
The tool provides organized output in the following sections:
1. **Dependency Check**: Shows available system commands
2. **System Information**: Displays system details
3. **Credential Files**: Contents of discovered credential files
4. **History Files**: Contents of shell history files
5. **SUID Files**: List of SUID binaries found on the system

## Configuration

### Customizing Checks
Edit `config/lotl.conf` to customize what the tool searches for:

```ini
[Dependencies]
required = cat, ifconfig, whoami, uname, hostname, uptime, mount
optional = curl, wget

[Credentials]
checkCreds = /etc/passwd, /etc/shadow, ~/.aws/credentials, ~/.ssh/id_rsa

[bashHistory]
checkBashHistory = ~/.bash_history, ~/.zsh_history, ~/.bashrc

# ... additional sections for logs, SSH, backups, etc.
```

### Configuration Sections
- **Dependencies**: Required and optional system commands
- **Credentials**: Paths to credential files
- **Logs**: System log file locations
- **SSH**: SSH-related files and configurations
- **bashHistory**: Shell history and configuration files
- **BackUps**: Backup file patterns and locations
- **SensitiveFiles**: Application configs and sensitive data

## Project Structure

```
LivingOfTheLand/
├── main.cpp           # Entry point and main execution flow
├── core/
│   ├── init.cpp       # Core enumeration functionality
│   └── init.h         # Header definitions
├── tools/
│   ├── suids.cpp      # SUID binary discovery
│   └── suids.h        # SUID tool headers
├── config/
│   └── lotl.conf      # Configuration file
├── Makefile           # Build configuration
└── README.md          # This file
```

## Security Considerations

### Responsible Usage
- **Authorized Testing Only**: Use only on systems you own or have explicit permission to test
- **Legal Compliance**: Ensure compliance with applicable laws and regulations
- **Professional Context**: Intended for penetration testing, security assessments, and educational purposes

### Limitations
- Some checks may require elevated privileges
- Permission-denied errors are handled gracefully
- Results depend on file system permissions and access controls

## Development

### Building with Debug Info
```bash
# The Makefile includes debug flags by default
make clean && make
```

### Adding New Checks
1. Update `config/lotl.conf` with new paths or patterns
2. Modify parsing logic in `core/init.cpp` if needed
3. Add new tool modules in the `tools/` directory

## Version History
- **v0.3**: Current release with enhanced SUID detection and configuration-driven checks

## Disclaimer

This tool is intended for legitimate security testing and educational purposes only. Users are responsible for ensuring they have proper authorization before using this tool on any system. The author assumes no liability for misuse or damage caused by this software.

## License

Use responsibly and ethically. Respect system owners' rights and applicable laws. 
