# Filecleaner - A Simple Directory Cleanup Tool

OVERVIEW

FileCleaner is a c++ utility which scans a specific directory for unnecessary files and temporary files (e.g., .log, .cache, .bak, .tmp) and removes them. This will help free up space within your disk and keep directories organized and viewable. 

FEATURES 
- Automatically detects and deletes unwanted file types.
- Logs deleted files for reference.
- The use of modern C++ (<filesystem> to handle files).
- The support of multiple system of temporary files.

USAGE
- Compile the program using a C++ compiler.
- Run the executable and specific the target which needs to be cleaned.
- The program will then scan and remove all unnecessary files while logging the process.

REQUIREMENT
C++17 or later (for <filesystem> support)
