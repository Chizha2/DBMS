#include "DBLibrary.h" // заголовок
#include <iostream> // ввод и вывод
#include <fstream> // потоки
#include <sstream> // потоки для строк
// #include <stdarg.h> для va_list функций
string value_to_string(void *value) { // преобразование из void *
	ostringstream tmp; // поток 
	tmp << (char *)value; // запись 
	return tmp.str(); // возвращение строки
};
void *DBTable::get_value(int value) { // преобразование в void *
	return new int(value); // выделение память под цифру и возвращение указателя
};
void *DBTable::get_value(string value, string columnName) { // преобразование в void *
	if (column_headers[columnName] == "Int32") // если цифра
		return new int(atoi(value.c_str())); // выделение памяти под цифру и возвращение указателя
	char *tmp = new char[value.size() + 1]; // массив символов
	sprintf(tmp, "%s", value.c_str()); // копирование в массив
	return tmp; // возвращение указателя
};
string DBTable::value_to_string(void *value, string columnName) { // преобразование из void *
	ostringstream tmp; // поток
	if (column_headers[columnName] == "Int32") // если цифра
		tmp << *(int *)value; // запись в поток как цифру
	else
		tmp << (char *)value; // запись в поток как массив символов
	return tmp.str(); // возвращение строки
};
bool DBTable::value_is_equal(string columnName, void* data, string value) { // сравнение значений
	if (value_to_string(data, columnName) == value) // условие равенства
		return true; // возвращение 1
	else
		return false; // возвращение 0
};
void DBTable::search(string column_name, void* void_value, vector <row> &rower) { // поиск
	string value = value_to_string(void_value, column_name); // преобразование в строку
	if (rower.size() == 0 || rower.size() == 1) // условие перезаписи
		rower = data; // присваивание данных
	for (int i = 0; i < rower.size(); i++) // цикл по записям таблицы
		if (!value_is_equal(column_name, rower[i][column_name], value)) { // проверка условия поиска
			rower.erase(rower.begin() + i); // удаление записи
			i--; // уменьшение счетчика
		}
};
void DBTable::read_table(string tab_name, string path) { // чтение
	int k = 1; // счетчик
	string tempToken; // временная строка
	table_name = tab_name; // присваивание имени
	char buf[500], *delim = "|\r", *token, *nextToken = buf; // буфер, разделители, лексема и остаток
	ifstream file(path + "\\" + tab_name + ".txt", ios::binary); // создание потока
	file.getline(buf, 500); // чтение первой строки
	while (token = strtok_s(nextToken, delim, &nextToken)) { // пока лексемы не кончатся
		if (k % 2 == 0) { // условие счетчика
			column_headers[tempToken] = token; // заполнение заголовка таблицы
			header_names.push_back(tempToken); // заполнение заголовка таблицы без типов
		}
		else
			tempToken = token; // присваивание значения временной строке
		k++; // увеличение счетчика
	}
	while (file.getline(buf, 500)) { // чтение остальных строк
		k = 0; // обнуление счетчика
		row tempRow; // временный контейнер
		char *nextToken_row = buf; // чистый nexttoken
		while (token = strtok_s(nextToken_row, delim, &nextToken_row)) { // пока лексемы не кончатся
			tempRow[header_names[k]] = get_value(token, header_names[k]); // заполнение данными
			k++; // увеличение счетчика
		}
		data.push_back(tempRow); // запись в вектор
	}
	file.close(); // закрытие потока
};
void DBTable::print_table() { // печать
	print_table(data); // вызов перегруженного метода печати таблицы
};
void DBTable::print_table(vector <row> &rower) { // печать таблицы
	cout << table_name << endl; // вывод названия 
	for (int k = 0; k < header_names.size(); k++) // цикл по заголовку 
		cout << header_names[k] << " "; // вывод заголовка 
	cout << endl; // пробел 
	for (int k = 0; k < rower.size(); k++) { // цикл по строкам 
		for (int i = 0; i < header_names.size(); i++) { // цикл по столбцам 
			cout << value_to_string(rower[k][header_names[i]], header_names[i]); // вывод данных 
			if (i != header_names.size() - 1) // условие конца строки
				cout << " ~ "; // печать разделителя 
		}
		cout << endl; // пробел 
	}
	cout << endl; // пробел 
};
void DBTable::write_table(string tab_name, string path) { // запись
	ofstream file(path + "\\" + tab_name + ".txt", ofstream::out | ofstream::trunc | ofstream::binary); // создание потока
	int k; // счетчик
	for (k = 0; k < header_names.size(); k++) { // цикл записи заголовка
		file << header_names[k] << "|" << column_headers[header_names[k]]; // запись 
		if (k != header_names.size() - 1) // условие конца строки
			file << "|"; // печать разделителя
	}
	file << endl; // перевод строки
	for (k = 0; k < data.size(); k++) { // цикл по строкам
		for (int i = 0; i < header_names.size(); i++) { // цикл по столбцам
			file << value_to_string(data[k][header_names[i]], header_names[i]); // запись данных
			if (i != header_names.size() - 1) // условие конца строки
				file << "|"; // печать разделителя
		}
		if (k != data.size() - 1) // условие конца записей
			file << endl; // перевод строки
	}
	file.close(); // закрытие файла
};
void DBTable::search(string column_name, int value, vector <row> &rower) { // поиск
	search(column_name, get_value(value), rower); // вызов метода поиска в таблице
};
void DBTable::search(string column_name, string value, vector <row> &rower) { // поиск
	search(column_name, get_value(value, column_name), rower); // вызов метода поиска в таблице
};
int DBTable::get_unic_id(string column_name) { // получение унакального id
	if (!column_ex(column_name)) { // проверка существования столбца
		cout << "[GET_UID]Столбец не найден\n";
		return -1; // возврат кода ошибки
	}
	if (column_headers[column_name] != "Int32") { // проверка типа входных данных
		cout << "[GET_UID]Столбец не содержит int\n";
		return -2; // возврат кода ошибки
	}
	int uid = 1; // обьявление начального id
	bool unic = 1; // флаг уникальности 
	while (uid != INT_MAX) { // цикл до максимального значения int системы
		unic = 1; // восстонавление флага
		for (int k = 0; k < data.size(); k++) // цикл сопоставления текущего id с существующими
			if (value_to_string(data[k][column_name], column_name) == to_string(uid)) {
				unic = 0; // сброс флага
				break; // выход из цикла
			}
		if (unic == 1) // если свободный id найден
			return uid; // возврат id из функции
		uid++; // увеличение id
	}
	/* Вывод ошибки при привышении максимального int */
	cout << "[GET_UID]Ошибка!\n Достигнуто максимальное количество ID в таблице\n";
	return -3; // возврат кода ошибки
};

bool DBTable::column_ex(string columnName) { // проверка существования заголовка
	for (int i = 0; i < header_names.size(); i++) // цикл по загаловкам таблици
		if (header_names[i] == columnName) // проверка
			return 1; // если существует
	return 0; // если не существует
}
bool DBTable::column_ex(vector <string> &columnName) { // проверка существования нескольких заголовков
	for (int i = 0; i < columnName.size(); i++) // цикл по по входным загаловкам
		if (column_ex(columnName[i]) == 0) // проверка
			return 0; // если существует
	return 1; // если не существует
}
/* 
void DBTable::compare_equal(vector <row> &rower, string type, ...) {
	vector <string> res_column;
	int k = -1, tmp = 0; // 2 счетчика
	char buf[50]; // буфер
	row Rower; // временный контейнер
	va_list ap;
	va_start(ap, type);
	if (type == "int")
	{
		int tmp = va_arg(ap, int);
		res_column.push_back(to_string(tmp));
	}
	if (type == "string")
	{
		string tmp = va_arg(ap, string);
		res_column.push_back(tmp);
	}
	va_end(ap);
	if (column_ex(res_column) == 0) {
		cout << "[CMP_EQL]Имя столбца не найдено" << endl;
		return;
	}
	for (int i = 0; i < data.size(); i++)
		for (int j = 0; j < res_column.size(); j++) {
			for (int k = 0; k < res_column.size(); k++) {
				if ((value_to_string(data[i][res_column[j]], res_column[j]) == value_to_string(data[i][res_column[k]], res_column[k])) && (j != k))
					rower.push_back(data[i]);
			}
		}
	clear_trash(rower);
}; 
*/
void DBTable::compare_equal(string intro, vector <row> &rower) {
	vector <string> res_column;
	int k = -1, tmp = 0; // 2 счетчика
	char buf[50]; // буфер
	row Rower; // временный контейнер
	for (int i = 0; i < intro.size(); i++) // цикл по введенной строке
		if (intro.c_str()[i] == '|') { // условие наличия разделителя
			k++; // увеличение счетчика
			intro.copy(buf, i - tmp, tmp); // копирование лексемы в буфер
			buf[i - tmp] = '\0'; // конец лексемы
			res_column.push_back(buf);
			tmp = i + 1; // изменение счетчика
		}
	intro.copy(buf, intro.size() - tmp, tmp); // копирование последней лексемы
	buf[intro.size() - tmp] = '\0'; // конец лексемы
	res_column.push_back(buf); // запись 
	if (column_ex(res_column) == 0) { // если ошибка
		cout << "[CMP_EQL]Имя столбца не найдено" << endl;
		return; // конец
	}
	for (int i = 0; i < data.size(); i++) // цикл по строкам
		for (int j = 0; j < res_column.size(); j++) // цикл по нужным столбцам
			for (int k = 0; k < res_column.size(); k++)  // повторный цикл
				if ((value_to_string(data[i][res_column[j]], res_column[j]) == value_to_string(data[i][res_column[k]], res_column[k])) && (j != k)) // условие равенства
					rower.push_back(data[i]); // запись
	clear_trash(rower); // очистка от повторений
};
void DBTable::clear_trash(vector <row> &rower) { // удаление парных записей
	for (int i = 0; i < rower.size(); i++) // цикл по i
		for (int j = 0; j < rower.size(); j++) // цикл по j
			if ((rower[j] == rower[i]) && (i != j)) // проверка совпадений
				rower.erase(rower.begin() + i); // удаление дубликата
}
void DBTable::compare(string intro, vector <row> &rower) { // сравнение
	vector <string> res_column; // заголовки
	vector <string> res_data; // данные
	int k = -1, tmp = 0; // 2 счетчика
	char buf[50]; // буфер
	row Rower; // временный контейнер
	for (int i = 0; i < intro.size(); i++) // цикл по введенной строке
		if (intro.c_str()[i] == '|') { // условие наличия разделителя
			k++; // увеличение счетчика
			intro.copy(buf, i - tmp, tmp); // копирование лексемы в буфер
			buf[i - tmp] = '\0'; // конец лексемы
			if (k % 2 == 0) // кратность
				res_column.push_back(buf); // запись
			else // иначе
				res_data.push_back(buf); // запись
			tmp = i + 1; // изменение счетчика
		}
	intro.copy(buf, intro.size() - tmp, tmp); // копирование последней лексемы
	buf[intro.size() - tmp] = '\0'; // конец лексемы
	res_data.push_back(buf); // запись последнего
	for (int i = 0; i < data.size(); i++) { // цикл по строкам
		int t = 0; // счетчик
		for (int j = 0; j < res_column.size(); j++) { // цикл по столбцам
			if (value_to_string(data[i][res_column[j]], res_column[j]) == res_data[j]) // если сходится
				t++; // увеличение счетчика
			if (t == res_column.size()) // если все сошлись
				rower.push_back(data[i]); // запись
		}
	}
};
void DBTable::clear(vector <row> &rower) { // удаление
	for (int k = 0; k < rower.size(); k++) // цикл по рабочему вектору
		for (int i = 0; i < data.size(); i++) // цикл по записям таблицы
			if (data[i] == rower[k]) // условие удаления
				data.erase(data.begin() + i); // удаление записи
}
void DBTable::replace(vector <row> &rower, vector <row> &replace) { // замена
	if (rower.size() != replace.size()) // проверка
		return; // прерывание
	for (int k = 0; k < rower.size(); k++) // цикл по рабочему вектору
		for (int i = 0; i < data.size(); i++) // цикл по записям таблицы
			if (data[i] == rower[k]) // условие замены
				data[i] = replace[k]; // замена записи
}
void DBTable::replace(row &rower, string column_name, int value) { // замена
	replace(rower, column_name, get_value(value)); // вызов метода поиска в таблице
}
void DBTable::replace(row &rower, string column_name, string value) { // замена
	replace(rower, column_name, get_value(value, column_name)); // вызов метода поиска в таблице
}
void DBTable::replace(row &rower, string column_name, void *value) { // замена
	rower[column_name] = value; // замена
}
void *DBTable::get_data(row &rower, string column_name) { // получение значения
	return rower[column_name]; // получение
};
void DBTable::add() { // добавление
	string answer; // ответ
	cout << "Введите строку запроса: ";
	cin >> answer; // ввод
	add(answer); // вызов метода добавления
};
void DBTable::add(string intro) { // добавление
	int k = -1, tmp = 0; // 2 счетчика
	char buf[50]; // буфер
	row Rower; // временный контейнер
	for (int i = 0; i < intro.size(); i++) // цикл по введенной строке
		if (intro.c_str()[i] == '|') { // условие наличия разделителя
			k++; // увеличение счетчика
			intro.copy(buf, i - tmp, tmp); // копирование лексемы в буфер
			buf[i - tmp] = '\0'; // конец лексемы
			Rower[header_names[k]] = get_value(buf, header_names[k]); // заполнение вектора
			tmp = i + 1; // изменение счетчика
		}
	intro.copy(buf, intro.size() - tmp, tmp); // копирование последней лексемы
	buf[intro.size() - tmp] = '\0'; // конец лексемы
	Rower[header_names[k + 1]] = get_value(buf, header_names[k + 1]); // заполнение вектора последней лексемой
	if ((k + 2) != header_names.size()) // условие сходимости количества лексем
		cout << "[ADD]Неправильные входные данные" << endl;
	else
		data.push_back(Rower); // добавление записи
};
void DBTable::add(vector <row> &rower) { // добавление
	for (int i = 0; i < rower.size(); i++)
		data.push_back(rower[i]);
}
DBTableSet::DBTableSet(string name) { // конструктор 
	readDB(name); // вызов метода чтения БД
};
DBTableSet::DBTableSet(string name, vector <string> intabname) { // конструктор
	DB_name = name; // присваивание имени
	tabNames = intabname; // присваивание имен таблиц
};
void DBTableSet::readDB(string name) { // чтение
	readDB(name, name); // вызов перегруженного метода чтения БД
};
void DBTableSet::readDB(string name, string in_path) { // чтение
	path = in_path; // присваивание пути
	DB_name = name; // присваивание имени
	char buf[500], *delim = "|\r", *token, *nextToken = buf; // буфер, разделитель, лексема и остаток
	ifstream file(path + "\\" + DB_name + ".txt", ios::binary); // создание потока
	if (!file.is_open()) { // проверка успешности открытия файла
		perror("[READ_DB]Error open file:"); // печать данных по ошибке
		cout << endl; // пробел
		system("pause"); // пауза
		return; // выход из функции
	}
	file.getline(buf, 500); // чтение по строкам
	while (token = strtok_s(nextToken, delim, &nextToken)) // цикл по лексемам
		tabNames.push_back(token); // заполнение вектора именами
	for (int i = 0; i < tabNames.size(); i++) // цикл по всем таблицам
		db[tabNames[i]].read_table(tabNames[i], path); // чтение таблицы
	file.close(); // закрытие потока
};
void DBTableSet::printDB() { // печать
	for (int i = 0; i < tabNames.size(); i++) // цикл по всем таблицам
		db[tabNames[i]].print_table(); // печать таблиц
};
void DBTableSet::writeDB() { // запись
	writeDB(DB_name, path); // вызов перегруженного оператора записи БД
};
void DBTableSet::writeDB(string name) { // запись
	writeDB(name, name); // вызов перегруженного оператора записи БД
};
void DBTableSet::writeDB(string name, string in_path) { // вызов перегруженного оператора записи БД
	system(string("mkdir \"" + in_path + "\" 2> NUL").c_str()); // создание папки для БД
	ofstream file; // создание потока вывода
	file.open(in_path + "\\" + name + ".txt", ofstream::trunc | ofstream::binary); // открытие файла на запись
	if (!file.is_open()) { // проверка успешности открытия файла
		perror("[WRITE_DB]Error open file:"); // печать данных по ошибке
		cout << endl; // перевод строки
		system("pause"); // пауза
		return; // выход из функции
	}
	for (int i = 0; i < tabNames.size(); i++) { // цикл записи по именам таблиц
		file << tabNames[i]; // запись имени таблицы
		if (i != tabNames.size() - 1) // проверка на конец строки
			file << "|"; // печать разделителя
	}
	file.close(); // закрытие потока
	for (int i = 0; i < tabNames.size(); i++) // цикл записи таблиц
		db[tabNames[i]].write_table(tabNames[i], in_path); // вызов метода записи таблиц
};