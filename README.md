# 3DES

Реализация алгоритмов шифрования DES и Triple DES

DES работает в режиме электронной кодовой книги (ECB)

Реализация основана на алгоритмах, описанных на сайте Википедия

[DES](https://ru.wikipedia.org/wiki/DES)

[3DES](https://ru.wikipedia.org/wiki/Triple_DES)

### ЗАПУСК

##### Режим шифрования

-e [file] [key1] [key2] [key3]


Т.к. шифр работает с блоками размером 64 бит (8 Байт), при необходимости к массиву данных добавляются дополнительные байты.
Кол-во добавленных байт показано в выводе программы.

##### Режим расшифровки

-d [file] [key1] [key2] [key3] [extra bytes]

Вызов программы похож, но необходимо дополнительно ввести кол-во байт, которые не будут записываться в выходной файл
