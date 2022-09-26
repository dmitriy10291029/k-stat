#include <iostream>
#include <fstream>
#include <vector>

const bool IS_FIRST_GROUP = false;
const bool IS_MY_OS_WIN = true;

struct Flags {
    bool printChars;
    bool printBytes;
    bool printWords;
    bool printLines;
    bool showUsageFlag;

    bool isEmpty() {
        return !(printChars || printBytes || printWords || printLines);
    }

    void allEqual(bool x) {
        printChars = x;
        printBytes = x;
        printWords = x;
        printLines = x;
    }
};

enum Errors {
    fileCantBeOpened
};

void showError(Errors error, char* errorConfig);

void showUsage();

bool isLetter(char ch);

bool isPrint(char ch);

void parser(int argc, char* argv[], Flags &flags, std::vector<char*> &files);

void counter(char* filename, Flags &flags);

int main(int argc, char* argv[]) {
    Flags flags;
    flags.allEqual(false); 
    flags.showUsageFlag = false;
    std::vector<char*> files;
    parser(argc, argv, flags, files);

    if (files.empty()) {
        showUsage();
    } else {
        if (flags.showUsageFlag) {
            showUsage();
        }
        for (int i = 0; i < files.size(); ++i) {
            counter(files[i], flags);
        }
    }
}

void showError(Errors error, char* errorConfig) {
    std::cerr << "Error: \"" << errorConfig << "\" - ";
    switch (error) {
        case fileCantBeOpened:
            std::cerr << "file can not be opened" << std::endl; break;
        default:
            std::cerr << "undefined error" << std::endl; break;
    }
    exit(EXIT_FAILURE);
}

void showUsage() {
    std::cout << "Usage: \n" 
              << "\t WordCount.exe flags... filenames...\n"
              << "Flags: \n"
              << "\t --lines (-l) print number of lines\n"
              << "\t --words (-w) print number of words\n"
              << "\t --bytes (-c) print number of bytes\n"
              << "\t --chars (-m) print number of chars" 
              << std::endl;
}

bool isLetter(char ch) {
    int ich = (int) ch;
    return ((ich >= 33) || (ich < 0)) && (ich != 127) && (ich != -128);
}

bool isPrint(char ch) {
    int ich = (int) ch;
    return ((ich >= 32) || (ich < 0)) && (ich != 127) && (ich != -128);
}

void parser(int argc, char* argv[], Flags &flags, std::vector<char*> &files) {
    for (int i = 1; i < argc; ++i) {
        char* arg = argv[i];
        if (arg[0] != '-' || strcmp(arg, "-") == 0) {
            files.push_back(arg);
        } else if (arg[1] == '-') {
            if (strcmp(arg, "--chars") == 0) {
                flags.printChars = true;
            } else if (strcmp(arg, "--words") == 0) {
                flags.printWords = true;
            } else if (strcmp(arg, "--bytes") == 0) {
                flags.printBytes = true;
            } else if (strcmp(arg,  "--lines") == 0) {
                flags.printLines = true;
            } else if (strcmp(arg, "--help") == 0) {
                flags.showUsageFlag = true;
            } else {
                files.push_back(arg);
            }
        } else {
            Flags newFlags = flags;
            int j = 1;
            for (j; arg[j] != '\0' ; ++j) {
                if (arg[j] == 'm') {
                    newFlags.printChars = true;
                } else if (arg[j] == 'c') {
                    newFlags.printBytes = true;
                } else if (arg[j] == 'w') {
                    newFlags.printWords = true;
                } else if (arg[j] == 'l') {
                    newFlags.printLines = true;
                } else if (arg[j] == 'h') {
                    newFlags.showUsageFlag = true;
                } else {
                    files.push_back(arg); 
                    break;
                }
            }
            if (arg[j] == '\0') {
                flags = newFlags;
            }
        }
    }
    if (flags.isEmpty()) {
        flags.allEqual(true);
    }
    flags.printChars &= IS_FIRST_GROUP;
}

void counter(char* filename, Flags &flags) {
    unsigned int lines = 0, words = 0, bytes = 0, chars = 0;
    std::ifstream file(filename);
    if (!file.is_open()) {
        showError(fileCantBeOpened, filename);
    } else {
        char ch = file.get();
        char previousChar = NULL;
        while (!file.eof()) {
            bytes++;
            if (ch == '\n') {
                if (IS_MY_OS_WIN) {
                    bytes++;
                }
                lines++;
            }
            if (isPrint(ch)) {
                chars++;
            }
            previousChar = ch;
            ch = file.get();
            if (!isLetter(ch) & isLetter(previousChar)) {
                words++;
            }
        }
    }
    //output for this file and each flag:
    if (flags.printLines) {
        std::cout << lines << ' ';
    }
    if (flags.printWords) {
        std::cout << words << ' ';
    }
    if (flags.printBytes) {
        std::cout << bytes << ' ';
    }
    if (flags.printChars) {
        std::cout << chars << ' ';
    }
    std::cout << filename << std::endl;
}