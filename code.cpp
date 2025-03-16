
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <ctime>
#ifdef _WIN32
#include <windows.h>
#endif

namespace fs = std::filesystem;

const std::string EXT_TMP = ".tmp";
const std::string EXT_LOG = ".log";
const std::string EXT_CACHE = ".cache";
const std::string EXT_BAK = ".bak";

const std::vector<std::string> TARGET_EXTENSIONS = {EXT_TMP, EXT_LOG, EXT_CACHE, EXT_BAK};

const std::string LOG_FILE = "file_cleaner.log";

std::string getTimestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm{};
#ifdef _WIN32
    localtime_s(&local_tm, &now_c);
#else
    local_tm = *std::localtime(&now_c);
#endif
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_tm);
    return std::string(buffer);
}

void logDeletion(const std::string &filePath)
{
    std::ofstream logFile(LOG_FILE, std::ios::app);
    if (logFile)
    {
        logFile << "[" << getTimestamp() << "] Deleted: " << filePath << "\n";
    }
}

bool isTargetFile(const fs::path &filePath)
{
    std::string extension = filePath.extension().string();
    for (const auto &ext : TARGET_EXTENSIONS)
    {
        if (extension == ext)
        {
            return true;
        }
    }
    return false;
}

void displayDirectoryStats(const std::string &directoryPath)
{
    int fileCount = 0;
    int targetFileCount = 0;
    uintmax_t totalSize = 0;

    if (fs::exists(directoryPath) && fs::is_directory(directoryPath))
    {
        for (const auto &entry : fs::directory_iterator(directoryPath))
        {
            if (fs::is_regular_file(entry))
            {
                fileCount++;
                totalSize += fs::file_size(entry);
                if (isTargetFile(entry.path()))
                {
                    targetFileCount++;
                }
            }
        }
        std::cout << "\nDirectory Statistics:" << std::endl;
        std::cout << "Total files: " << fileCount << std::endl;
        std::cout << "Target files to delete: " << targetFileCount << std::endl;
        std::cout << "Total size: " << totalSize / 1024 << " KB\n";
    }
    else
    {
        std::cerr << "Error: Directory does not exist or is inaccessible.\n";
    }
}

void cleanDirectory(const std::string &directoryPath)
{
    try
    {
        int deletedFiles = 0;
        uintmax_t freedSpace = 0;
        if (fs::exists(directoryPath) && fs::is_directory(directoryPath))
        {
            for (const auto &entry : fs::directory_iterator(directoryPath))
            {
                if (fs::is_regular_file(entry) && isTargetFile(entry.path()))
                {
                    uintmax_t fileSize = fs::file_size(entry.path());
                    fs::remove(entry.path());
                    logDeletion(entry.path().string());
                    std::cout << "Deleted: " << entry.path().string() << " (" << fileSize / 1024 << " KB)\n";
                    deletedFiles++;
                    freedSpace += fileSize;
                }
            }
            std::cout << "\nCleanup complete. " << deletedFiles << " files removed, freeing " << freedSpace / 1024 << " KB of space.\n";
        }
        else
        {
            std::cerr << "Error: Directory does not exist or is inaccessible.\n";
        }
    }
    catch (const fs::filesystem_error &e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::string targetDirectory;
    std::cout << "Enter the directory path to clean (e.g., \"C:/path/to/folder\"): ";
    std::getline(std::cin, targetDirectory);

    displayDirectoryStats(targetDirectory);
    cleanDirectory(targetDirectory);

    return 0;
}
