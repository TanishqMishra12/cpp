/**
 * TermBot: Your Personal Terminal Assistant
 * 
 * A C++ console chatbot that interprets natural language input
 * and maps it to real terminal commands.
 */

 #include <iostream>
 #include <string>
 #include <vector>
 #include <map>
 #include <regex>
 #include <fstream>
 #include <sstream>
 #include <algorithm>
 #include <array>
 #include <memory>
 #include <stdexcept>
 #include <cstdio>
 #include <thread>
 #include <ctime>
 #include <cctype>
  #ifdef _WIN32
 #include <windows.h>
 #include <shlobj.h>
 #elif __APPLE__
 #include <unistd.h>
 #include <pwd.h>
 #include <sys/stat.h>
 #else // Linux
 #include <unistd.h>
 #include <pwd.h>
 #include <sys/stat.h>
 #endif
 
 // Terminal color codes
 namespace Color {
     const std::string reset = "\033[0m";
     const std::string black = "\033[30m";
     const std::string red = "\033[31m";
     const std::string green = "\033[32m";
     const std::string yellow = "\033[33m";
     const std::string blue = "\033[34m";
     const std::string magenta = "\033[35m";
     const std::string cyan = "\033[36m";
     const std::string white = "\033[37m";
     const std::string bold = "\033[1m";
 }
 
 // UI helper functions
 void printColoredText(const std::string& text, const std::string& color) {
     std::cout << color << text << Color::reset << std::endl;
 }
 
 void printBanner() {
     std::cout << Color::cyan << Color::bold;
     std::cout << R"(
   _______                    ______       _   
  |__   __|                  |  ____|     | |  
     | | ___ _ __ _ __ ___   | |__ ___ ___| |_ 
     | |/ _ \ '__| '_ ` _ \  |  __/ _ | __| __|
     | |  __/ |  | | | | | | | | | (_) |__ \ |_ 
     |_|\___|_|  |_| |_| |_| |_|  \___/___/\__|
                                              
     )" << std::endl;
     std::cout << Color::reset;
     std::cout << Color::yellow << "      Your Personal Terminal Assistant" << Color::reset << std::endl;
     std::cout << std::endl;
 }
 
 void showLoadingAnimation(int milliseconds) {
     const std::vector<std::string> frames = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
     int framesCount = frames.size();
     int iterations = milliseconds / 100;
 
     for (int i = 0; i < iterations; i++) {
         std::cout << "\r" << "Loading " << Color::cyan << frames[i % framesCount] << Color::reset;
         std::cout.flush();
         std::this_thread::sleep_for(std::chrono::milliseconds(100));
     }
     std::cout << "\r" << "Loading " << Color::green << "✓" << Color::reset << std::endl;
 }
 
 void showTypingEffect(const std::string& text, int speedMilliseconds = 30) {
     for (char c : text) {
         std::cout << c << std::flush;
         std::this_thread::sleep_for(std::chrono::milliseconds(speedMilliseconds));
     }
     std::cout << std::endl;
 }
 
 void clearScreen() {
 #ifdef _WIN32
     system("cls");
 #else
     system("clear");
 #endif
 }
 
 void showProgressBar(int percent) {
     const int barWidth = 50;
     std::cout << "[";
     int pos = barWidth * percent / 100;
     for (int i = 0; i < barWidth; ++i) {
         if (i < pos) std::cout << "=";
         else if (i == pos) std::cout << ">";
         else std::cout << " ";
     }
     std::cout << "] " << percent << "%\r";
     std::cout.flush();
 }
 
 // System utilities
 namespace SystemUtils {
     std::string getOperatingSystem() {
 #ifdef _WIN32
         return "Windows";
 #elif __APPLE__
         return "macOS";
 #else
         return "Linux";
 #endif
     }
 
     bool createDirectoryIfNotExists(const std::string& path) {
 #ifdef _WIN32
         std::wstring widePath(path.begin(), path.end());
         if (CreateDirectory(widePath.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
             return true;
         }
         return false;
 #else
         struct stat st = {0};
         if (stat(path.c_str(), &st) == -1) {
             return mkdir(path.c_str(), 0700) == 0;
         }
         return true;
 #endif
     }
 
     bool fileExists(const std::string& filename) {
         std::ifstream file(filename);
         return file.good();
     }
 
     std::string getHomeDirectory() {
 #ifdef _WIN32
         wchar_t homePath[MAX_PATH];
         if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, homePath))) {
             std::wstring wideHomePath(homePath);
             return std::string(wideHomePath.begin(), wideHomePath.end());
         }
         return "";
 #else
         const char* homedir = getenv("HOME");
         if (homedir == nullptr) {
             homedir = getpwuid(getuid())->pw_dir;
         }
         return std::string(homedir);
 #endif
     }
 
     bool speakText(const std::string& text) {
 #ifdef _WIN32
         std::string command = "powershell -Command \"Add-Type -AssemblyName System.Speech; "
                              "(New-Object System.Speech.Synthesis.SpeechSynthesizer).Speak('" + text + "')\"";
         return system(command.c_str()) == 0;
 #elif __APPLE__
         std::string command = "say '" + text + "'";
         return system(command.c_str()) == 0;
 #else
         std::string command = "espeak '" + text + "' 2>/dev/null";
         return system(command.c_str()) == 0;
 #endif
     }
 }
 
 // CommandHandler definition
 class CommandHandler {
 public:
     CommandHandler() {
         dangerousCommands = {
             "rm -rf", "rmdir /s", "deltree", "format", "shutdown", "reboot",
             ":(){:|:&};:", "fork bomb", "dd if=/dev/zero", "chmod -R 777", "mkfs",
             "> /dev/sda", "> /dev/hda", "wget", "curl | sh"
         };
     }
 
     std::string executeCommand(const std::string& command) {
         if (!isSafeCommand(command)) {
             return "⚠️ Sorry, this command has been blocked for safety reasons.";
         }
 
         std::string result;
 
         // Handle special commands
         if (command == "get_system_info") return getSystemInfo();
         if (command == "get_memory_info") return getMemoryInfo();
         if (command == "get_ip_address") return getIPAddress();
         if (command == "get_weather") return getWeather();
 
         std::array<char, 128> buffer;
         std::string output;
 
 #ifdef _WIN32
         std::string fullCommand = "cmd /c " + command + " 2>&1";
 #else
         std::string fullCommand = command + " 2>&1";
 #endif
 
        #ifdef _WIN32
                #ifdef _WIN32
                                #ifdef _WIN32
                                                                 std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(fullCommand.c_str(), "r"), _pclose);
                                #else
                                                                 std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(fullCommand.c_str(), "r"), pclose);
                                #endif
                #else
                                 std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(fullCommand.c_str(), "r"), pclose);
                #endif
        #else
                 std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(fullCommand.c_str(), "r"), pclose);
        #endif
         if (!pipe) {
             return "❌ Error executing command.";
         }
 
         while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
             output += buffer.data();
         }
 
         return output.empty() ? "✅ Command executed successfully (no output)." : output;
     }
 
 private:
     std::string getSystemInfo() {
 #ifdef _WIN32
         return executeCommand("systeminfo");
 #elif __APPLE__
         return executeCommand("system_profiler SPHardwareDataType SPSoftwareDataType");
 #else
         std::string osInfo = executeCommand("cat /etc/os-release | grep PRETTY_NAME");
         std::string kernelInfo = executeCommand("uname -a");
         return "OS: " + osInfo + "\nKernel: " + kernelInfo;
 #endif
     }
 
     std::string getMemoryInfo() {
 #ifdef _WIN32
         return executeCommand("wmic OS get FreePhysicalMemory,TotalVisibleMemorySize /Value");
 #elif __APPLE__
         return executeCommand("top -l 1 -s 0 | grep PhysMem");
 #else
         return executeCommand("free -h");
 #endif
     }
 
     std::string getIPAddress() {
 #ifdef _WIN32
         return executeCommand("ipconfig | findstr IPv4");
 #else
         std::string localIP = executeCommand("ip addr | grep 'inet ' | grep -v 127.0.0.1");
         if (localIP.empty()) {
             localIP = executeCommand("ifconfig | grep 'inet ' | grep -v 127.0.0.1");
         }
         return localIP;
 #endif
     }
 
     std::string getWeather() {
         std::string weather = executeCommand("curl -s wttr.in/?format=3");
         return weather.empty() ? "⚠️ Could not fetch weather data. Check internet connection." : weather;
     }
 
     bool isSafeCommand(const std::string& command) {
         std::string lowerCommand = command;
         std::transform(lowerCommand.begin(), lowerCommand.end(), lowerCommand.begin(),
                        [](unsigned char c) { return std::tolower(c); });
         for (const auto& dangerous : dangerousCommands) {
             if (lowerCommand.find(dangerous) != std::string::npos) {
                 return false;
             }
         }
         return true;
     }
 
     std::vector<std::string> dangerousCommands;
 };
 
 // NLPEngine definition
 struct CommandPattern {
     std::vector<std::string> keywords;
     std::vector<std::regex> patterns;
     std::string command;
 };
 
 class NLPEngine {
 public:
     NLPEngine() { initializePatterns(); }
 
     std::string interpretInput(const std::string& input) {
         std::string lowerInput = input;
         std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                        [](unsigned char c) { return std::tolower(c); });
 
         for (const auto& pattern : commandPatterns) {
             if (matchesKeywords(lowerInput, pattern.keywords)) {
                 if (pattern.patterns.empty() || matchesRegex(lowerInput, pattern.patterns)) {
                     return pattern.command;
                 }
             }
         }
         return "";
     }
 
 private:
     std::vector<CommandPattern> commandPatterns;
 
     void initializePatterns() {
         commandPatterns.push_back({
             {"time", "current time", "what time"},
             {std::regex("what.*time.*it"), std::regex("tell.*time")},
 #ifdef _WIN32
             "time /t"
 #else
             "date +\"%T\""
 #endif
         });
 
         commandPatterns.push_back({
             {"date", "today", "day", "current date"},
             {std::regex("what.*date"), std::regex("what day.*is"), std::regex("today.*date")},
 #ifdef _WIN32
             "date /t"
 #else
             "date +\"%A, %B %d, %Y\""
 #endif
         });
 
         commandPatterns.push_back({
             {"calendar", "month", "cal"},
             {std::regex("show.*calendar"), std::regex("month.*calendar")},
 #ifdef _WIN32
             "powershell -Command \"Get-Calendar\""
 #else
             "cal"
 #endif
         });
 
         commandPatterns.push_back({
             {"files", "list", "directory", "folder", "ls", "dir"},
             {std::regex("show.*files"), std::regex("list.*files"), std::regex("what.*files")},
 #ifdef _WIN32
             "dir"
 #else
             "ls -la"
 #endif
         });
 
         commandPatterns.push_back({
             {"current", "directory", "folder", "pwd", "location"},
             {std::regex("(current|working|present).*directory"), std::regex("where.*am.*i")},
 #ifdef _WIN32
             "cd"
 #else
             "pwd"
 #endif
         });
 
         commandPatterns.push_back({
             {"system", "info", "about", "details", "computer"},
             {std::regex("system.*info"), std::regex("about.*computer"), std::regex("computer.*details")},
             "get_system_info"
         });
 
         commandPatterns.push_back({
             {"memory", "ram", "free", "available"},
             {std::regex("(how much|available|free).*memory"), std::regex("memory.*usage")},
             "get_memory_info"
         });
 
         commandPatterns.push_back({
             {"ip", "address", "network"},
             {std::regex("(what|my).*ip.*address"), std::regex("show.*ip")},
             "get_ip_address"
         });
 
         commandPatterns.push_back({
             {"weather", "forecast", "temperature", "rain", "sunny"},
             {std::regex("(what|how).*weather"), std::regex("weather.*forecast"), std::regex("is.*rain")},
             "get_weather"
         });
 
         commandPatterns.push_back({
             {"process", "running", "programs", "tasks"},
             {std::regex("(show|list).*process"), std::regex("running.*program"), std::regex("what.*running")},
 #ifdef _WIN32
             "tasklist"
 #else
             "ps aux"
 #endif
         });
 
         commandPatterns.push_back({
             {"disk", "space", "storage", "drive"},
             {std::regex("(disk|drive|storage).*space"), std::regex("how much.*space")},
 #ifdef _WIN32
             "wmic logicaldisk get size,freespace,caption"
 #else
             "df -h"
 #endif
         });
 
         commandPatterns.push_back({
             {"network", "connection", "internet", "ping", "check"},
             {std::regex("(check|test).*connection"), std::regex("(is|am).*online")},
 #ifdef _WIN32
             "ping -n 3 8.8.8.8"
 #else
             "ping -c 3 8.8.8.8"
 #endif
         });
     }
 
     bool matchesKeywords(const std::string& input, const std::vector<std::string>& keywords) {
         for (const auto& keyword : keywords) {
             if (input.find(keyword) != std::string::npos) {
                 return true;
             }
         }
         return false;
     }
 
     bool matchesRegex(const std::string& input, const std::vector<std::regex>& patterns) {
         for (const auto& pattern : patterns) {
             if (std::regex_search(input, pattern)) {
                 return true;
             }
         }
         return false;
     }
 };
 
 // TermBot definition
 class TermBot {
 public:
     TermBot() {
         SystemUtils::createDirectoryIfNotExists("logs");
         SystemUtils::createDirectoryIfNotExists("data");
 
         logFile.open("logs/termbot_log.txt", std::ios::app);
         if (!logFile.is_open()) {
             std::cerr << "Warning: Could not open log file. Logging disabled." << std::endl;
         } else {
             std::time_t now = std::time(nullptr);
             logFile << "\n--- TermBot Session Started: " << std::ctime(&now);
             logFile.flush();
         }
 
         loadAliases();
         initializeEasterEggs();
     }
 
     ~TermBot() {
         if (logFile.is_open()) {
             std::time_t now = std::time(nullptr);
             logFile << "--- TermBot Session Ended: " << std::ctime(&now);
             logFile.close();
         }
     }
 
     void processInput(const std::string& input) {
         if (input.empty()) return;
 
         std::string response;
         std::string command;
 
         if (input == "help") {
             showHelp();
             return;
         }
         if (input == "menu") {
             showMenu();
             return;
         }
         if (input == "clear") {
             clearScreen();
             printBanner();
             return;
         }
 
         if (checkForEasterEgg(input, response)) {
             showTypingEffect(response);
             logInteraction(input, response);
             return;
         }
 
         if (input.substr(0, 6) == "alias ") {
             std::string aliasCmd = input.substr(6);
             size_t pos = aliasCmd.find('=');
             if (pos != std::string::npos) {
                 std::string key = aliasCmd.substr(0, pos);
                 std::string value = aliasCmd.substr(pos + 1);
 
                 key.erase(0, key.find_first_not_of(" \t"));
                 key.erase(key.find_last_not_of(" \t") + 1);
                 value.erase(0, value.find_first_not_of(" \t"));
                 value.erase(value.find_last_not_of(" \t") + 1);
 
                 if (!key.empty() && !value.empty()) {
                     aliases[key] = value;
                     saveAliases();
                     response = "✅ Alias created: \"" + key + "\" → \"" + value + "\"";
                     printColoredText(response, Color::green);
                     logInteraction(input, response);
                 } else {
                     response = "❌ Invalid alias format. Use: alias name=command";
                     printColoredText(response, Color::red);
                 }
                 return;
             }
         }
 
         for (const auto& alias : aliases) {
             if (input == alias.first) {
                 std::cout << "🔄 Using alias: " << alias.second << std::endl;
                 command = alias.second;
                 break;
             }
         }
 
         if (command.empty()) {
             command = nlpEngine.interpretInput(input);
         }
 
         if (!command.empty()) {
             showLoadingAnimation(500);
             response = commandHandler.executeCommand(command);
             std::cout << response << std::endl;
         } else {
             std::string suggestion = getSuggestedCommand(input);
             if (!suggestion.empty()) {
                 std::cout << "🤔 Did you mean: \"" << suggestion << "\"? (y/n): ";
                 std::string confirm;
                 std::getline(std::cin, confirm);
                 if (confirm == "y" || confirm == "yes") {
                     processInput(suggestion);
                     return;
                 }
             }
             response = "😕 I'm not sure how to help with that. Type 'help' to see available commands.";
             printColoredText(response, Color::yellow);
         }
 
         logInteraction(input, response);
     }
 
 private:
     CommandHandler commandHandler;
     NLPEngine nlpEngine;
     std::map<std::string, std::string> aliases;
     std::ofstream logFile;
     std::map<std::string, std::string> eastereggs;
 
     void loadAliases() {
         std::ifstream file("data/aliases.txt");
         if (!file.is_open()) {
             SystemUtils::createDirectoryIfNotExists("data");
             std::ofstream createFile("data/aliases.txt");
             if (createFile.is_open()) createFile.close();
             return;
         }
 
         std::string line;
         while (std::getline(file, line)) {
             size_t pos = line.find('=');
             if (pos != std::string::npos) {
                 std::string key = line.substr(0, pos);
                 std::string value = line.substr(pos + 1);
                 aliases[key] = value;
             }
         }
         file.close();
     }
 
     void saveAliases() {
         std::ofstream file("data/aliases.txt");
         if (!file.is_open()) {
             std::cerr << "Error saving aliases: Could not open file." << std::endl;
             return;
         }
 
         for (const auto& alias : aliases) {
             file << alias.first << "=" << alias.second << std::endl;
         }
         file.close();
     }
 
     void logInteraction(const std::string& input, const std::string& response) {
         if (!logFile.is_open()) return;
 
         std::time_t now = std::time(nullptr);
         struct tm* timeinfo = std::localtime(&now);
         char timestamp[20];
         std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
 
         logFile << "[" << timestamp << "] User: " << input << std::endl;
         logFile << "[" << timestamp << "] Bot: " << response << std::endl;
         logFile.flush();
     }
 
     void showHelp() {
         std::cout << "\n📚 TermBot Help Menu" << std::endl;
         std::cout << "===================" << std::endl;
         std::cout << "• Natural language commands:" << std::endl;
         std::cout << "  - \"What time is it?\"" << std::endl;
         std::cout << "  - \"List files in this folder\"" << std::endl;
         std::cout << "  - \"Show available memory\"" << std::endl;
         std::cout << "  - \"What's my IP address?\"" << std::endl;
         std::cout << "  - \"Check the weather\"" << std::endl;
         std::cout << "\n• Aliases:" << std::endl;
         std::cout << "  - Create: alias name=command" << std::endl;
         std::cout << "  - Example: alias weather=curl wttr.in/?format=3" << std::endl;
         std::cout << "\n• System Commands:" << std::endl;
         std::cout << "  - help   : Show this menu" << std::endl;
         std::cout << "  - menu   : Show interactive menu" << std::endl;
         std::cout << "  - clear  : Clear the screen" << std::endl;
         std::cout << "  - exit   : Exit TermBot" << std::endl;
         std::cout << "===================" << std::endl;
     }
 
     void showMenu() {
         while (true) {
             clearScreen();
             printBanner();
             std::cout << "\n🔍 TermBot Interactive Menu" << std::endl;
             std::cout << "=========================" << std::endl;
             std::cout << "1. Files and Directories" << std::endl;
             std::cout << "2. System Information" << std::endl;
             std::cout << "3. Network Tools" << std::endl;
             std::cout << "4. Date and Time" << std::endl;
             std::cout << "5. Your Aliases" << std::endl;
             std::cout << "6. Exit Menu" << std::endl;
             std::cout << "=========================" << std::endl;
             std::cout << "Enter your choice (1-6): ";
 
             std::string choice;
             std::getline(std::cin, choice);
 
             if (choice == "1") {
                 std::cout << "\n📁 Files and Directories\n----------------------" << std::endl;
                 std::cout << "• \"Show files\" - List files" << std::endl;
                 std::cout << "• \"Current directory\" - Show working directory" << std::endl;
             } else if (choice == "2") {
                 std::cout << "\n💻 System Information\n----------------------" << std::endl;
                 std::cout << "• \"How much memory\" - Show memory usage" << std::endl;
                 std::cout << "• \"System info\" - Show system details" << std::endl;
             } else if (choice == "3") {
                 std::cout << "\n🌐 Network Tools\n----------------------" << std::endl;
                 std::cout << "• \"What's my IP\" - Show IP address" << std::endl;
                 std::cout << "• \"Check connection\" - Ping Google" << std::endl;
                 std::cout << "• \"Show weather\" - Display weather" << std::endl;
             } else if (choice == "4") {
                 std::cout << "\n🕒 Date and Time\n----------------------" << std::endl;
                 std::cout << "• \"What time is it\" - Show time" << std::endl;
                 std::cout << "• \"What's today's date\" - Show date" << std::endl;
                 std::cout << "• \"Show calendar\" - Display calendar" << std::endl;
             } else if (choice == "5") {
                 std::cout << "\n🔖 Your Aliases\n----------------------" << std::endl;
                 if (aliases.empty()) {
                     std::cout << "No aliases defined. Use 'alias name=command'" << std::endl;
                 } else {
                     for (const auto& alias : aliases) {
                         std::cout << "• \"" << alias.first << "\" → \"" << alias.second << "\"" << std::endl;
                     }
                 }
             } else if (choice == "6") {
                 std::cout << "Returning to main interface..." << std::endl;
                 break;
             } else {
                 std::cout << "Invalid choice. Try again." << std::endl;
             }
 
             std::cout << "\nPress Enter to continue...";
             std::cin.get();
         }
     }
 
     bool checkForEasterEgg(const std::string& input, std::string& response) {
         std::string lowerInput = input;
         std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                        [](unsigned char c) { return std::tolower(c); });
 
         auto it = eastereggs.find(lowerInput);
         if (it != eastereggs.end()) {
             response = it->second;
             return true;
         }
         return false;
     }
 
     void initializeEasterEggs() {
         eastereggs["i'm bored"] = "Why not explore the terminal? Try 'show files' or ask for a joke!";
         eastereggs["tell me a joke"] = "Why don't scientists trust atoms? Because they might be up to something!";
         eastereggs["who are you"] = "I'm TermBot, your friendly terminal assistant, here to make your command-line life easier!";
         eastereggs["what is the meaning of life"] = "42, of course! But if you want a real answer, try 'show files' to find meaning in your directories.";
         eastereggs["hello world"] = "A classic! Want to see what's in your world? Try 'list files'.";
         eastereggs["do a barrel roll"] = "Spinning through the terminal! Want to spin through your files? Say 'show files'.";
     }
 
     std::string getSuggestedCommand(const std::string& input) {
         std::vector<std::pair<std::string, int>> potentialMatches;
         std::vector<std::string> commonCommands = {
             "show files", "show date", "show time", "what time is it",
             "what is my ip", "how much memory", "show weather", "list processes",
             "check connection", "current directory"
         };
 
         std::string lowerInput = input;
         std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(),
                        [](unsigned char c) { return std::tolower(c); });
 
         for (const auto& cmd : commonCommands) {
             int score = 0;
             std::string lowerCmd = cmd;
             std::transform(lowerCmd.begin(), lowerCmd.end(), lowerCmd.begin(),
                            [](unsigned char c) { return std::tolower(c); });
 
             for (size_t i = 0; i < std::min(lowerInput.length(), lowerCmd.length()); i++) {
                 if (lowerInput[i] == lowerCmd[i]) score++;
             }
 
             std::istringstream iss1(lowerInput), iss2(lowerCmd);
             std::string word1, word2;
             while (iss1 >> word1 && iss2 >> word2) {
                 if (word1.length() > 2 && word2.length() > 2 && word1.substr(0, 2) == word2.substr(0, 2)) {
                     score += 2;
                 }
             }
 
             if (score > 3) potentialMatches.emplace_back(cmd, score);
         }
 
         for (const auto& alias : aliases) {
             int score = 0;
             std::string lowerAlias = alias.first;
             std::transform(lowerAlias.begin(), lowerAlias.end(), lowerAlias.begin(),
                            [](unsigned char c) { return std::tolower(c); });
 
             for (size_t i = 0; i < std::min(lowerInput.length(), lowerAlias.length()); i++) {
                 if (lowerInput[i] == lowerAlias[i]) score++;
             }
 
             if (score > 3) potentialMatches.emplace_back(alias.first, score);
         }
 
         if (!potentialMatches.empty()) {
             std::sort(potentialMatches.begin(), potentialMatches.end(),
                       [](const auto& a, const auto& b) { return a.second > b.second; });
             return potentialMatches[0].first;
         }
         return "";
     }
 };
 
 // Main function
 int main() {
     clearScreen();
     printBanner();
     showLoadingAnimation(1000);
 
     TermBot bot;
     printColoredText("Hello! I'm TermBot, your terminal assistant. Type 'help' to get started.", Color::green);
 
     while (true) {
         std::cout << "\n> ";
         std::string input;
         std::getline(std::cin, input);
 
         if (input == "exit" || input == "quit") {
             printColoredText("Goodbye! Thanks for using TermBot.", Color::yellow);
             break;
         }
 
         bot.processInput(input);
     }
 
     return 0;
 }