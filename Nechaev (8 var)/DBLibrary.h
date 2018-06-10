#include <string> // строки
#include <vector> // векторы
#include <map> // контейнеры
using namespace std; // пространство имен
typedef map <string, void*> row; // запись
typedef map <string, string> header; // заголовок
string value_to_string(void *); // преобразование из void *
class DBTable { // таблицы
private: // внутри класса
	vector <row> data; // записи
	string table_name; // имя
	header column_headers; // заголовок
	vector <string> header_names; // заголовок без типов
	void *get_value(int value); // преобразование в void * чисел
	void *get_value(string, string); // преобразование в void * чисел и строк
	string value_to_string(void *, string); // преобразование из void *
	bool value_is_equal(string, void *, string); // сравнение значений
	void replace(row &, string, void *); // замена значения
	bool column_ex(string);
	bool column_ex(vector <string> &);
public: // везде
	DBTable() {}; // конструктор по умолчанию
	int get_unic_id(string column_name);
	void clear_trash(vector <row> &);
	void read_table(string, string); // чтение
	void print_table(); // печать 
	void print_table(vector <row>); // печать 
	void write_table(string, string); // запись
	void search(string, void *, vector <row> &); // поиск
	void search(string, int, vector <row> &); // поиск
	void search(string, string, vector <row> &); // поиск
	void clear(vector <row> &); // удаление
	void replace(vector <row> &, vector <row> &); // замена строк
	void replace(row &, string, int); // замена значения
	void replace(row &, string, string); // замена значения
	void add(); //  добавление
	void add(string); // добавление
	void add(vector <row> &); // добавление
	void *get_data(row &, string); // получение значения
	void compare(string, vector <row> &);
	void compare_equal(string, vector <row> &);
	void compare_equal(vector <row> &rower, string, ...);
};
class DBTableSet { // БД
private: // внутри класса
	vector <string> tabNames; // таблицы
	string DB_name; // имя
	string path; // путь
public: // везде
	map <string, DBTable> db; // таблицы
	DBTableSet() {}; // конструктор по умолчанию
	DBTableSet(string); //  конструктор
	DBTableSet(string, vector <string>); // конструктор
	void readDB(string); // чтение
	void readDB(string, string); // чтение
	void printDB(); // печать
	void writeDB(); // запись
	void writeDB(string); // запись
	void writeDB(string, string); // запись
};