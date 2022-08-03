# SearchEngine

## Краткое описание проекта:
Данный проект представляет собой поисковый движок, осуществляющий поиск ключевых слов по текстовым файлам с помощью предварительной инвентированной
индексации базы данных этих документов. Параметры поиска, пути к документам, а также обая конфигурация задается при помощи JSON - файлов.
В проекте используются стандартные библиотеки C++, библиотека nlohmann/json для работы с JSON - файлами, а также GTest для Unit-тестирования.

## Описание кофигурационных файлов JSON:

### config.json
 - config — общая информация, без которой приложение не запускается. 
 - name — поле с названием поискового движка.
 - version — поле с номером версии поискового движка.
 - max_responses — поле, определяющее максимальное количество ответов на один запрос.
 - files содержит пути к файлам, по которым необходимо осуществлять поиск.

### requests.json
 - requests - состоит из списка запросов, которые необходимо обработать поисковым движком.
 
 ### answers.json
 - answers — базовое поле в этом файле, которое содержит ответы на запросы.
 - request[№] — идентификатор запроса, по которому сформирован ответ.
 - result – результат поиска запроса. Если он принимает значение true, значит по данному запросу найден хотя бы один документ.
 - relevance - включается в файл answers.json, если на этот запрос удалось найти более одного документа.
   - docid - идентификатор документа
   - rank - ранг или поисковый рейтинг

## Описание программных файлов:

### App.h
Класс приложения посикового движка. Содержит текстовое поле версии приложения, а также следующие методы:
 - setVersion - сеттер для поля версии.
 - checkConfig - проверяет наличие и правильность заполнения конфигурационного JSON - файла, в случае неверного заполнения 
 или отстутствия выдает соответствующие ошибки.
 - run - Запускает приложение и выполняет следующие действия:
   - проверяет конфигурацию при помощи checkConfig
   - считывает документы и поисковые запросы
   - составляет и индексирует базу даннных
   - осуществляет поиск по базе данных
   - формирует и записывает ответы
   
### ConverterJSON.h
Класс, ответственный за работу с файлами JSON. Содержит следующие методы:
 - getTextDocuments - считывает пути документов из конфигурационного файла, обрабатывает документы и возвращает их содержимое в виде массива строк.
 - getResponseLimit - считывает из конфигурационного файла максимальное кол-во результатов поиска и возвращает полученнное знанчение в виде целого числа.
 - getRequests - считывает поисковые запросы из JSON - файла и возвращает результа в виде массива строк
 - putAnswers - принимает результаты поиска в виде двумерного массива пар id документа и его релевантности (целое число - число с плавающей точкой)
 и записывает их в файл с ответами.
 
 ### InvertedIndex.h
 Класс, ответсвенный за построение и индексацию базы данных документов. Содержит поля docs (документы в виде массива строк) и
 freqDictionary (ассоциативный контейнер со словами в виде ключей и массивами записей в виде значений).
 Структура данных Entry (запись) - содержит id документа и кол-во упоминаний определнного слова в этом документе.
 Данный класс содержит следующие методы:
 - processDocument - принимает документ в виде строки и его id в виде целого неотрицательного числа, индексирует документ - добавлет новые слова в
 словарь, добавляет или изменяет записи по уже существующим словам.
 - updateDocumentBase - принимает содержимое документов в виде массива строк, формирует базу данных и запускает индексацию документов в отдельных потоках.
 - getWordCount - принимает слово в виде строки и id документа в виде целого неотрицательного числа (для ускорения работы) и возвращает расчитанный
 массив записей для данного слова (документы в которых оно встречается и кол-во упоминаний).
 - getTotalWordCount - принимает слово в виде строки и возвращает общее кол-во упоминаний данного слова во всех документах в виде целого
 неотрицательного  числа (необходмо для определения наиболее редких и частых слов при обработке поискового запроса).
 
 ### SearchServer.h
 Основной класс поискового движка, осуществляющий поиск по запросам, формирование и ранжирование ответов. Содержит экземпляр класса InvertedIndex в 
 качестве индексированной базы данных, по которой осуществляется поиск. Ранжирование результатов осуществляется с помощью следующих структур:
  - AbsoluteIndex - пара целых неотрицательных чисел id документа - его абсолютная релевантность (определенная по кол-ву искомых слов)
  - RelativeIndex - пара id документа (целое неотрицательное цисло) - его ранг (релевантность нормированная по наиболее релевантному документу)
  Класс содержит следующие методы:
  - processQueries - получает поисковые запросы в виде массива строк, разбивает их на слова и возваращает в виде двумерного массива строк.
  - processWords - принимает слова из поискового запроса в виде массива строк, считает общую частоту каждого слова в базе данных, сортирует
  полученный результат от натболее редких к наиболее частым и возвращает его виде массива пар строка-число.
  - processResults - принимает результаты поиска в виде массива элементов AbsoluteIndex, формирует из них элементы RelativeIndex, ранжирует их по
  релевантности и отбирает допустимое кол-во результатов, возвращает массив элементов RelativeIndex.
  - search - принимает поисковые запросы в виде массива строк и осуществляет поиск путем последовательного применения предыдущих трех 
  методов, возвращает результат в виде двумерного массива элементов RelativeIndex.
  - formAnswers - получает результаты поиска из предыдущего метода и предобрабатывает их для дальнейшей записи в файл JSON.
  
  ### Tests.cpp
  Файл с предварительными тестами работы функций индексации и поиска. Тестирование осуществляется при помощи библиотеки GTest (GoogleTest)
  
  ### CMakeLists.txt
  Стандартный файл необходимый для сборки проекта. В нем также указаны пути к внешним библиотекам и процесс их подключения.
  
  ### main.cpp
  Файл, из которого запускается приложение. ВАЖНО: для запуска необходимо отключить тесты (убрать файл с тестами из исполняемых)
