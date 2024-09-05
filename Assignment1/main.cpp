#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  std::string filename{};
  if (argc > 1) {
    filename = argv[1];
  }

  if (filename.size() == 0) {
    std::cout << "usage: program filepath";
    exit(1);
  }

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cout << "Failed to open " << filename << "\n";
    exit(2);
  }

  char c;
  while (file.get(c) && c != EOF) {
    switch (c) {
    case '/': {
      if (file.get(c) && c == '/') {
        while (file.get(c) && c != '\n') {
          continue;
          // std::cout << c;
        }
        // std::cout << c;
      } else {
        file.putback(c);
      }
      break;
    }
    default:
      std::cout << c;
    }
  }
}
