# Complaint-management-system

## Features

### User
- Register a new complaint with a custom description
- Edit a complaint (only while it's still *Pending*)
- View all submitted complaints with their current status
- Share (display) a specific complaint by ID

### Admin
- View all complaints from all users
- Confirm a complaint
- Reject a complaint

### General
- File-based persistence — complaints and credentials survive between sessions
- Account registration on first launch (admin + user)
- Colorful, styled terminal UI via ANSI color codes
- Input validation and error handling throughout

## Getting Started

### Prerequisites
- A C++ compiler supporting **C++11** or later
- A terminal that supports **ANSI escape codes** (Linux, macOS, Windows Terminal, Git Bash)

### Compilation
g++ -o complaint_system main.cpp


### Running
linux:
./complaint_system


On Windows:
complaint_system.exe

## File Structure

main.cpp            # Full source code
admin.txt           # Stores admin credentials (auto-created)
user.txt            # Stores user credentials (auto-created)
complaints.txt      # Stores all complaints (auto-created)


**Note:** The `.txt` files are created automatically on first run. Do not manually edit them to avoid data corruption.

## Usage

### First Launch
On the first run (no accounts registered), you'll be prompted to set up:
- Admin username & password
- User username & password

### Login
Choose to log in as **Admin** or **User** and enter your credentials.

### User Dashboard

1. Register Complaint
2. Edit Complaint
3. View Complaints
4. Share Complaint
5. Logout


### Admin Dashboard

1. View All Complaints
2. Confirm Complaint
3. Reject Complaint
4. Logout



