#include "App.h"

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
