# SearchEngine

## Краткое описание проекта:
Данный проект представляет собой поисковый движок, осуществляющий поиск ключевых слов по текстовым файлам с помощью предварительной инвентированной
индексации базы данных этих документов. Параметры поиска, пути к документам, а также обая конфигурация задается при помощи JSON - файлов.
В проекте используются стандартные библиотеки C++, библиотека nlohmann/json для работы с JSON - файлами, а также GTest для Unit-тестирования.

## Описание кофигурационных файлов JSON:

### config.json
 - config — общая информация, без которой приложение не запускается. 
 - name — поле с названием поискового движка.
● version — поле с номером версии поискового движка. Впоследствии можно сделать проверку. Если поле version не совпадает с версией самого приложения, то необходимо выдавать ошибку config.json has incorrect file version.
● max_responses — поле, определяющее максимальное количество ответов на один запрос.
● files содержит пути к файлам, по которым необходимо осуществлять поиск. Внутри списка files лежат пути к файлам.
