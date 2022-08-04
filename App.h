#include "SearchServer.h"


class NoConfigFileException: public exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "config file is missing.";
    }
};

class EmptyConfigException: public exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "config file is empty.";
    }
};

class IncorrectVersionException: public exception {
    [[nodiscard]] const char* what() const noexcept override {
      return "config.json has incorrect file version.";
    }
};

class App {
private:
    string version = "0.1";
public:
    void setVersion(const string& s) { version = s; }
    App() = default;
    void CheckConfig();
    void run();
};
