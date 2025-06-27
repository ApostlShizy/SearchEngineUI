# Search Engine UI

Данное приложение является интерфейсом для другого моего проекта [SearchEngine](https://github.com/ApostlShizy/SearchEngine). Алгоритм поиска не претерпел значительных изменений так что за справкой можно обратиться к документации из проекта. Все изменения можно прочитать в заголовках ниже.

**Задача проекта: предоставить конечному пользователю более удобное, понятное и функциональное приложение чем оригинальный проект.**

### Основные пункты документации 
- [Изменения ConverterJson](#изменения-converterjson)
- [Интерфейс Приложения](#интерфейс-приложения)
- [Код приложения](#код-приложения)

<br>

## Изменения ConverterJSON

Касательно изменений базового алгоритма - был затронут ``ConverterJSON``. Изменён формат чтения и записи файла ``"config.json"``, а именно ...

Было: 

```json
{
    "config": {
        "name": "SearchEngine",
        "version": "0.7",
        "max_responses": 5
    },
    "files": [
        "txt example/new 1.txt",
        "txt example/new 2.txt",
        "txt example/new 3.txt",
        "txt example/new 4.txt",
        "txt example/new 5.txt",
        "txt example/new 6.txt"
    ]
}
```

Стало:

```json
{
    "config": {
        "name": "SearchEngineUi",
        "version": "0.1",
        "max_responses": 5
    },
    "files": [
        "txt example/new 1.txt",
        "txt example/new 2.txt",
        "txt example/new 3.txt",
        "txt example/new 4.txt",
        "txt example/new 5.txt",
        "txt example/new 6.txt",
		"txt example/new 7.txt"
    ],
	"requestsFile": "requests.json",
	"answerFile": "answer.json"
}
```

Теперь файл в файле конфигураций в том числе содержатся пути к файлу запроса ``` requests.json``` и файлу ответа ```answer.json```. Данные имена-пути по умолчанию.

<br>

### В самом классе появился новый метод: 

```C++
    void setConfigPath(std::vector<std::string> & docsPath,
                    std::string configPath = "config.json",
                    std::string requestsPath = "requests.json",
                    std::string answerPath = "answer.json",
                    size_t maxResponses = 5)
```

Данный метод сохраняет предустановки программы в файл конфигурации для будущей загрузки данного пресета. Если какое либо значение отсутствует - оно устанавливается по умолчанию.

<br>

### Был переработан метод ```getConfig```.

Было:
```C++
    int getConfig(std::vector<std::string>& filePath,
    std::string configPath = "config.json")
```

Стало:

```C++
    int getConfig(std::vector<std::string>& filePath,
                  std::string & requestsPath,
                  std::string & answerPath,
                  const std::string configPath = "config.json") 
```

Теперь метод в качестве аргументов ожидает не только ссылку на вектор строк для путей ```filePath```, сам путь к самому файлу конфигурации ```configPath (по умолчанию "config.json")```. А так же ссылку на путь ответа ```answerPath``` и путь к файлу запроса ```requestsPath```. При отсутствии любого из путей выбрасывается исключение и изменения не будут внесены.

Исключение из правила - путь ответа (если тот отсутствует устанавливается значение по умолчанию ```".exe/answer.json"```).

Если в файле указано недопустимое значение ```maxResponse``` устанавливается значение по умолчанию равно ```5```.

<br>
<br>

# Интерфейс приложения 

__Интерфейс приложения поделён на четыре основные вкладки:__

### 1. [Options](#Options)
### 2. [List Of Requests](#list-of-requests)
### 3. [Docs Paths](#docs-paths)
### 4. [Answer](#answer)


<br>


## Options

**Данная вкладка, отвечает за базовые настройки приложения.**

![](https://sun9-72.userapi.com/s/v1/if2/x7MoY2CufHnvnJBaRCbjWy7awouXrm4GaB2yoorjzGScSeHaW5-Qk_MDUO-4SaDBCiSyFE-rgiI0R4PbohiPrlB4.jpg?quality=95&as=32x25,48x38,72x57,108x85,160x126,240x189,360x283,480x378,540x425,640x504,720x567,800x630&from=bu&cs=800x0)

**В верхней - левой части приложения под вкладкой** ```Options``` расположены кнопки загрузки и сохранения пресета. Данные кнопки открывают диалоговое окно файловой системы позволяя указать путь сохранения или загрузки. __ВАЖНО ОТМЕТИТЬ ЧТО ВСЕ ИЗМЕНЕНИЯ С ЗАПРОСАМИ, ОТВЕТАМИ И СПИСКАМИ ДОКУМЕНТОВ СОХРАНЯЮТСЯ В РУЧНУЮ ЧЕРЕЗ ДАННУЮ ОПЦИЮ, НЕ ЗАБЫВАЙТЕ СОХРАНЯТЬ СВОИ ИЗМЕНЕНИЯ.__

![](https://sun9-26.userapi.com/s/v1/if2/DN5NueR04vvRelui4Hf2K2VlQHZiBecgvq4aVq8bHeK5ORuQZMMYq8_-ArcuNxgcEdfpfrOIHQqP6HTN5X8c9UX0.jpg?quality=95&as=32x14,48x21,72x32,108x48,160x71,240x106,360x159,366x162&from=bu&u=SiNEts2KeHRTXqFVw98EpbEx73VhQRinY-wL4mK9314&cs=360x0)

    Краткая справка о том что сохраняется в пресете. В пресете сохраняется путь самого файла конфигурации "config.json". В данный файл записываются пути к документам, путь к файлу запроса "requests.json" и путь к файлу ответа "answer.json" (если такой был сформирован).

**Правей кнопок сохранения и загрузки пресета** расположен путь к загруженному файла пресета.

![](https://sun9-58.userapi.com/s/v1/if2/Sq-FZWhqb1U7DHdezlfgzTN441oZtt0pBAWJ2ErR3Coaiprclue7_uDncuDziCgwqGz_jN3E1kw7h1mAaPxJtjEv.jpg?quality=95&as=32x6,48x9,72x14,108x21,160x31,240x46,360x69,480x92,489x94&from=bu&cs=489x0)

**Правей описания пути** - выбор языка приложения, открывается вываливающийся список. ```(в данной версии поддерживает только английский)```

![](https://sun9-5.userapi.com/s/v1/if2/IPwuwgGqtYqSP0GQIukwSxWd0UB20TlFIHvOlMWXJ6Kc25nmDWG-G63oxHaGp67PeXRYgVSsqoC5yJ8WxVn42-5h.jpg?quality=95&as=32x22,48x34,72x51,108x76,160x112,240x169,299x210&from=bu&cs=299x0)

<br>

### Ниже расположилась группа опций ```"Answer options"```.

![](https://sun9-68.userapi.com/s/v1/if2/BQBmjOFP6izC8WjajfqVDn8hnesGTM9DqNuPkGE_hsSM49ZEvQQ_Thd7FvMWVB1v7HznUDBxDVGFHFGUkVMdlaRc.jpg?quality=95&as=32x14,48x21,72x32,108x48,160x70,240x106,360x158,480x211,540x238,543x239&from=bu&cs=543x0)


Как понятно из названия отвечает за настройки формирования ответа. В данной версии опции поддерживают: 

1. Включение исключения номеров ```"Enable numbers"```. ```(по умолчанию приложение игнорирует цифры)```

![](https://sun9-36.userapi.com/s/v1/if2/FT7skF9LCq6-BuQtbTVTrMqtbj616Ztou8SvcRD5feTZCPA0ro1_DTcWP5oG5U6gEkbaek3HZFiVZuDyHYimxxlt.jpg?quality=95&as=32x14,48x21,72x32,108x48,160x70,240x106,360x158,480x211,540x238,543x239&from=bu&cs=543x0)

2. Сохранять ли файл ответа по указанному пути или пути по умолчанию.```"Save result in to answer.json"```. Когда изменение будет применено и сохранено текст изменит цвет на зелёный, если изменения не сохранены текст будет чёрным. ```(тут же присутствует кнопка для указания через диалоговое окно файловой системы куда именно сохранять ответ помимо ручного ввода, рекомендую использовать файловую систему)```

![](https://sun9-79.userapi.com/s/v1/if2/MBt0QqJeORpOy2sIkTe_9ZYSpTQ3tuJM7_L50NLJJLXzeEg3YgDPVytS7sPC7hiu1C7NKEBOQus5ijTUQsJAhhQx.jpg?quality=95&as=32x14,48x21,72x32,108x48,160x70,240x106,360x158,480x211,540x238,543x239&from=bu&cs=543x0)

![](https://sun9-43.userapi.com/s/v1/if2/iIvXFGgS6e-kaHLxsw_JQqqUlJAtawi9rWw2auenqqGKIrPlumBTEPjxNg8MRyJWWCZxwXYUh040qKo8zVG6dBYf.jpg?quality=95&as=32x6,48x8,72x12,108x19,160x28,240x42,360x63,480x83,501x87&from=bu&cs=501x0)

![](https://sun9-44.userapi.com/s/v1/if2/bXoumFBfWy9ndmk-IMS5JYytTqfZEasmXqpqkI6qqruJW28bQd-UwZzidpZArDk1LhvSuq-HAqTLNChEZsjwUdTa.jpg?quality=95&as=32x6,48x8,72x12,108x19,160x28,240x42,360x63,480x83,501x87&from=bu&cs=501x0)

3. Обозначение максимального количества ответов на запрос ```"Max responses"```. ```(если оставить пустым будет применено  значение по умолчанию, по аналогии с путём цвет отображает статус внесённых изменений)```

![](https://sun9-3.userapi.com/s/v1/if2/eq7EJCgEUzOOhjb_TJA6xqSgKqcqdkHqdAPg3cG7kkvHXlyWIqDDUiRlltJivjFXcyxzCXqSBCS5emmFlI1ib8hu.jpg?quality=95&as=32x14,48x21,72x32,108x48,160x70,240x106,360x158,480x211,540x238,543x239&from=bu&cs=543x0)

<br>

### Под группой ```"Answer options"```. Расположена группа опций ```"Requests options"```. 

![](https://sun9-14.userapi.com/s/v1/if2/mqU4hGFtUqsDR_FczWHYdkQx5ggy7F_BhSVrZERxzzy8qukengBsOEbhb5vlI3BPSPdUkjY23AyzHJm74jwmnfpw.jpg?quality=95&as=32x10,48x16,72x24,108x35,160x52,240x79,360x118,480x158,540x177,579x190&from=bu&cs=579x0)

Отвечает за настройки запросов. В данной версии опции поддерживают: 

- Сохранять ли файл запроса по указанному пути или пути по умолчанию. ```"Save request in to requests.json"```. Когда изменение будет применено и сохранено текст изменит цвет на зелёный, если изменения не сохранены текст будет чёрным, как и в описанных выше опциях. ```(тут же присутствует кнопка для указания через диалоговое окно файловой системы куда именно сохранять ответ помимо ручного ввода, рекомендую использовать файловую систему)```

<br>

## List Of Requests

**Данная вкладка, отвечает за настройки запросов.**

![](https://sun9-77.userapi.com/s/v1/if2/fxw23HsdHVi6MY_iHI7Sv9Oyc1-eZf8egKCK6bTJ0ZoWhHRUeRbNrqleIXb910XQLAlZya5IAhZ32y3td_h96-0Q.jpg?quality=95&as=32x25,48x38,72x57,108x85,160x126,240x189,360x283,480x378,540x425,640x504,720x567,800x630&from=bu&cs=800x0)

**Начнём с левой-верхней части вкладки**, маленькая кнопка ввиду документа со стрелочкой отвечает за открытие диалогового окна файловой системы и загрузки запроса из документа типа ```"requests.json"```. После добавления новые запросы заполняют пропуски id если таковые имеются и помещаются в конец списка.

![](https://sun9-34.userapi.com/s/v1/if2/2vR2_T_FP-_nP0Ky_eaZQx_lkMFnV_sslOjuhykkg93-PabXr2eFgsf580ko3bdsFmkqx5Ag3oFZWSQEati3Fspy.jpg?quality=95&as=32x25,48x38,72x57,108x85,160x126,240x188,298x234&from=bu&cs=298x0)

**Правей от кнопки добавления документа** - расположены четыре опции взаимодействия со списком запросов:

![](https://sun9-58.userapi.com/s/v1/if2/sek000Ec4wl9cge1ce7aV76jIQ9VAKhymMgdb6ZwCnQ1-v80TAJlwyALFWYpCXL6UKXHo7_tMpjnwV8DNgBksqHz.jpg?quality=95&as=32x14,48x21,72x32,108x48,160x71,240x106,360x159,423x187&from=bu&cs=423x0)

- Add Request - Добавить запрос. Для данного действия нужно ввести желаемое id (оно не должно быть занято иначе программа выберет ближайший доступный). После через пробелы указать слова , к примеру: ```5 some words add here```. Данный ввод добавит запрос из четырёх слов, с id = 5;

- Alter Request - Редактировать запрос. Данное действие работает по принципу схожему с добавлением запроса: ```id word word word word```. При вводе id программа отобразить в списке есть ли такой запрос для редактирования и если нет ничего не случится. Редактирование происходит по принципу полной замены списка слов на новый, указанный в строке по данному id. К примеру:

    ```
    Было: 5 some words
    Стало: 5 here we alter some words and get result
    ```

- Delete Requests - Удалить запрос. Для удаления запроса достаточно просто ввести id данного запроса, ```id указан в поле "Request id: id" ```.

![](https://sun9-50.userapi.com/s/v1/if2/3UFu7YRcsGjYg_KzK7COCgrCIYpO3rATuwaAAns_I1ab4YdkomcmuWZaqk29uRujETBBNZw-5Qkb-VWCQLU4WPsZ.jpg?quality=95&as=32x7,48x11,72x16,108x24,160x36,240x54,360x81,480x108,502x113&from=bu&cs=502x0)

- Search - Поиск запроса. Поиск происходит как по id так и по подстроке в словах

Одновременно может быть активна лишь одна опция, ```активная опция будет выделена синим```, она и отвечает за то что именно будет делать приложение во время ввода и подтверждения ввода в строке.

<br>

**Чуть правей выбора взаимодействия с запросом**, расположена сама строка ввода.

![](https://sun9-81.userapi.com/s/v1/if2/-Vfm5vF_unkoayxOe-jZ0yYRRTY2O2JFKMc0ZBAcyzylf_yUUwjD1Yh6Q8MRAgObVod8_njWV6I42E2EgNTqsa1r.jpg?quality=95&as=32x12,48x18,72x26,108x40,160x59,240x88,360x132,480x177,503x185&from=bu&cs=503x0)

<br>

**Под всеми виджетами, в центре -** ```"Current requests.json:"```. Перечень всех загруженных запросов. __Каждый отдельный запрос отделён пунктирной линией__

<br>

## Docs Paths

**Данная вкладка, отвечает за настройки базы данных документов.**

![](https://sun9-56.userapi.com/s/v1/if2/z-EjlhHA7wcqjdpKIaLdOjikrU-g9azPYTvoAGEWf4tTqpsDWtEv2X2oiq0Bwo1j13DLDgSk9rUrkii7rWEMTZZv.jpg?quality=95&as=32x25,48x38,72x57,108x85,160x126,240x189,360x283,480x378,540x425,640x504,720x567,800x630&from=bu&cs=800x0)

<br>

**Начнём с левой-верхней части**. Тут расположена кнопка диалога файловой системы. В самой файловой системе можно выбрать целый список и тот будет автоматически добавлен в базу данных документов а его путь выведен полностью на экран.

![](https://sun9-56.userapi.com/s/v1/if2/Z2WoMtzfEH_pckiJDbF-LTlnrMWlWmHlKyHoV2Mraj1Vg6CvhxOjn5WZfI281XkJH-3y53qv-K8mZs2bM5EsUH65.jpg?quality=95&as=32x19,48x28,72x43,108x64,160x95,240x142,318x188&from=bu&cs=318x0)

<br>

**Правей от кнопки диалога файловой системы**, расположены опции взаимодействия с перечнем документов: 

![](https://sun9-61.userapi.com/s/v1/if2/Ab4vD4upI8HjyzJa42enlrUP2oqPGCosACiNTr6mESHkqbvKsOnvyRkq-obKh7bSKT3g8UXR5-xve65hif-Xzl_C.jpg?quality=95&as=32x13,48x20,72x30,108x45,160x66,240x100,360x149,383x159&from=bu&cs=383x0)

- Add Dock - Добавить документ. Для добавление требуется ввести полный путь,добавляет всего один документ за раз. При ошибке ввода пути - документ не будет добавлен.

- Delete Dock - Удалить документ. Удаляет документ из перечня по его id, ```id написан рядом с самим документом "doc_id: id"```.

- Search - Поиск. Ищет по id документа или подстроке в описании пути к файлу.

Одновременно может быть активна лишь одна опция, ```активная опция будет выделена синим```, она и отвечает за то что именно будет делать приложение во время ввода и подтверждения ввода в строке.

**Правей от опций взаимодействия с документами** - Строка ввода.

![](https://sun9-21.userapi.com/s/v1/if2/J0Y7VBWkSAUMOtbE_UZFHxtBOEZz81z6XxI280kT6h78b69wAKEm8KkMGH9rfyS5pYz0HC0RFSkWk4aYwS6SXnW6.jpg?quality=95&as=32x11,48x17,72x25,108x38,160x56,240x84,360x127,480x169,529x186&from=bu&cs=529x0)

<br>

**Под всеми виджетами, центре экрана -** ```"All docs paths:"```. Перечень всех загруженных документов в приложение. Каждый отдельный документ разделён пунктирной линией.

<br>

## Answer

**Данная вкладка отвечает за формирование ответа на поисковый запрос и его сортировку.**

![](https://sun9-25.userapi.com/s/v1/if2/F4MT9c5baebeM3V6l-PWv7FO1mei0CRf3IfFsleBIBKul-_uWgl91bSPdfe3RT9nTW1MSYHMTw7q8N4ineyS-32q.jpg?quality=95&as=32x25,48x38,72x57,108x85,160x126,240x189,360x283,480x378,540x425,640x504,720x567,800x630&from=bu&cs=800x0)

<br>

**Слева-вверху**, расположена кнопка ```"Form Answer"``` она отвечает за формирование ответа по записанным ранее данным: Запросам, перечню документов.

![](https://sun9-8.userapi.com/s/v1/if2/EIF6hhdNBVlTHswiihSIaJmnA5MbF9pVyyavOGWORf8Bmwz3vrgF-ArilGqBNKPylAcdKL0U74oQIU5rIy1Zc_S1.jpg?quality=95&as=32x22,48x33,72x50,108x75,160x111,222x154&from=bu&cs=222x0)

Принцип формирования ответа был описан в [SearchEngine - Файл с ответами на запросы "answers.json"](https://github.com/ApostlShizy/SearchEngine?tab=readme-ov-file#%D1%84%D0%B0%D0%B9%D0%BB-%D1%81-%D0%BE%D1%82%D0%B2%D0%B5%D1%82%D0%B0%D0%BC%D0%B8-%D0%BD%D0%B0-%D0%B7%D0%B0%D0%BF%D1%80%D0%BE%D1%81%D1%8B-answersjson).

<br>

**Чуть правей кнопки** ```"Form Answer"```, выпадающий список с опциями сортировки.

![](https://sun9-55.userapi.com/s/v1/if2/oFy1L9gg6mg3H6BZvZMf-HNuN6e0KnDxQcRZk-Uaub4bvfe8EHVonQtkJH3WDPgEOAHY5oudf47XPm0Ogrv3Jjkh.jpg?quality=95&as=32x16,48x24,72x36,108x54,160x80,240x120,355x178&from=bu&cs=355x0)

**- Sort by rank** - Сортировать по рангу запроса, то есть по его валидности. Сортирует по диапазону. То есть для корректно сортировки нужно выбрать данную сортировку, затем ввести в строку ввода начало диапазона и конец через тире. Пример ```0.5 - 0.8```

**- Sort by doc_id** - Сортировать по id документов. Сортирует по конкретному id документа.Выбираем данную опцию, вводим id в строку поиска и в списке выводятся все ответы где встречается данный документ.

**- Sort by responses** - Сортировать по количеству ответов. Выбираем данную опцию, вводим количество интересующих нас ответов в строку поиска, и получаем все ответы в которых количество вхождений в документы равно заданному числу.

**- Sort by requests** - Сортировать по конкретному id запроса. Выбираем данную опцию, вводим id запроса, и получаем все ответы в которые входит данный запрос.

__Все сортировки работают через ввод в строку поиска. Которая расположена правей опций сортировки.__

![](https://sun9-17.userapi.com/s/v1/if2/5Kl9_-SsiEyXHhWsyWlcyL_qvhcFYOVmqBF52h7khvhO9JxjptjslSvL1ZjFzNF2QRUAUrENqjHV8Rob6CeIKs26.jpg?quality=95&as=32x12,48x19,72x28,108x42,160x62,240x93,360x139,480x185,518x200&from=bu&cs=518x0)

**Так же рядом с опциями сортировки**, есть опция сортировки **сверху-вниз/снизу-вверх** , изображена в виде стрелочек ```↑/↓```. В данной версии приложения работает только с **"Sort by rank"** опцией.

<br>
<br>

# Код приложения

Алгоритм поиска уже был описан в документации [SearchEngine](https://github.com/ApostlShizy/SearchEngine). В данном заголовке будут описаны объекты для работы с интерфейсом приложения.

### 1. optionWindow - Интерфейс приложения (Описание в работе)
### 2. searchEngine - Прослойка связывающая интерфейс приложения и поисковый алгоритм (Описание в работе)