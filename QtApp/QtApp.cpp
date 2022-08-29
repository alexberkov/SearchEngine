#include <QApplication>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include "../ConverterJson.h"
#include "../SearchServer.h"

using namespace std;

void clearLayout(QLayout *layout) {
  if (layout == nullptr) return;
  QLayoutItem *item;
  while((item = layout->takeAt(0))) {
    if (item->layout()) {
      clearLayout(item->layout());
      delete item->layout();
    }
    if (item->widget()) delete item->widget();
    delete item;
  }
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QSizePolicy p1(QSizePolicy::Preferred, QSizePolicy::Preferred);
  QSizePolicy p2(QSizePolicy::Preferred, QSizePolicy::Preferred);
  p1.setHorizontalStretch(1);
  p2.setHorizontalStretch(3);
  auto *mainWindow = new QWidget;
  mainWindow->setMinimumSize(1280, 720);
  auto *window = new QHBoxLayout(mainWindow);
  auto *leftSide = new QWidget;
  auto *leftSideLayout = new QVBoxLayout(leftSide);
  auto *resGroup = new QWidget;
  auto *resGroupLayout = new QVBoxLayout(resGroup);
  auto *searchGroup = new QWidget;
  auto *searchGroupLayout = new QHBoxLayout(searchGroup);
  auto *searchBar = new QLineEdit;
  auto *searchButton = new QPushButton;
  searchButton->setText(QString("Поиск"));
  auto *docView = new QPlainTextEdit;
  docView->setSizePolicy(p2);
  leftSide->setSizePolicy(p1);
  searchGroupLayout->addWidget(searchBar);
  searchGroupLayout->addWidget(searchButton);
  leftSideLayout->addWidget(searchGroup);
  leftSideLayout->addWidget(resGroup);
  window->addWidget(leftSide);
  window->addWidget(docView);
  leftSideLayout->setAlignment(Qt::AlignTop);

  auto docs = ConverterJSON::getTextDocuments("../");
  auto docNames = ConverterJSON::getFileNames("../");
  int responseLimit = ConverterJSON::getResponseLimit("../");
  InvertedIndex dataBase;
  dataBase.UpdateDocumentBase(docs);
  SearchServer s(dataBase);
  vector<string> query;
  vector<vector<RelativeIndex>> results;

  QObject::connect(searchButton, &QPushButton::clicked, [&searchBar, &query, &s, &results,
                                                         &resGroupLayout, &docNames, &docView, &docs, &responseLimit]() {
      auto q = searchBar->text().toStdString();
      query.clear();
      query.push_back(q);
      results = s.search(query);
      auto result = results[0];

      clearLayout(resGroupLayout);

      if (result.empty()) {
        auto *answer = new QLabel;
        answer->setText(QString("Нет результатов"));
        resGroupLayout->addWidget(answer);
      } else {
        for (int i = 0; i < result.size() & i < responseLimit; ++i) {
          size_t id = result[i].docID;
          auto *res = new QWidget;
          auto *resLayout = new QHBoxLayout(res);
          auto *docName = new QLabel;
          auto str = docNames[id];
          auto *openButton = new QPushButton;
          openButton->setText(QString("Открыть"));
          docName->setText(QString::fromStdString(str));
          QObject::connect(openButton, &QPushButton::clicked, [&docView, &docs, id]() {
              docView->clear();
              docView->insertPlainText(QString::fromStdString(docs[id]));
          });
          resLayout->addWidget(docName);
          resLayout->addWidget(openButton);
          resGroupLayout->addWidget(res);
        }
      }
  });

  mainWindow->show();
  return QApplication::exec();
}
