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

void App::run() {
  setVersion("0.1");
  try {
    App::CheckConfig();
    auto docs = ConverterJSON::getTextDocuments();
    auto reqs = ConverterJSON::getRequests();
    InvertedIndex dataBase;
    dataBase.UpdateDocumentBase(docs);
    SearchServer s(dataBase);
    auto searchResults = s.search(reqs);
    ConverterJSON::putAnswers(s.formAnswers(searchResults));
  } catch (const exception &x) {
    cerr << "Error: " << x.what() << endl;
  }
}

void App::CheckConfig() {
  ifstream configFile("../JSON/config.json");
  if (!configFile.is_open()) throw NoConfigFileException();
  else {
    json config;
    configFile >> config;
    configFile.close();
    if (config["config"].empty()) throw EmptyConfigException();
    if (config["config"]["version"] != version) throw IncorrectVersionException();
  }
}
