#include "DBLibrary.h" // подключение библиотеки СУБД
#pragma comment(lib, "DBLibrary.lib") // подключение библиотеки СУБД
#include <iostream> // импорт ввода и вывода
#include <iomanip>


DBTableSet DB("DB"); // глобальная БД
void add_student();
int get_subject_id(string);
void print_books_by_sid(int sid);
void chk_exist(vector <row> &rower);
void give_book_by_subject();
void give_book(int, int);
string get_date();

int main() {
	system("chcp 1251 > nul");
	add_student(); // добавление студента
	give_book_by_subject(); // выдача книги, по предмету
	DB.writeDB(); // сохранение изменений
	system("pause");
	return 0;
}

void give_book_by_subject() { // диалог добавления студента
	vector <row> rower; // временный вектор
	int bid; // id книги
	string s_name; 
	cout << "Введите название предмета: ";
	cin >> s_name;
	cout << "В библиотеки имеется: " << endl;
	int sid = get_subject_id(s_name); // получение id предмета
	print_books_by_sid(sid); // вывод списка книг по предмету
	cout << "Введите ID книги для выдачи: "; 
	cin >> bid;
	give_book(sid,bid); // выдача книги
	cout << "Книга выдана\n";
}
void give_book(int sid, int bid) // выдача книги
{
	vector <row> rower; // временный вектор
	DB.db["Books"].search("BookID", bid, rower); // получение вектора книги 
	DB.db["Books"].replace(rower[0], "Number", (*(int *)(DB.db["Books"].get_data(rower[0], "Number")))--); // уменьщения количества книг в СУБД
	DB.db["Abonements"].add(to_string(sid)+"|"+to_string(bid)+"|"+get_date()+"|NULL"); // запись данных о выдаче
}
void print_books_by_sid(int sid) { // вывод книг по id предмета
	vector <row> rower; // временный вектор
	DB.db["Books"].search("SubjectID", sid ,rower); // поиск всех книг по предмету
	chk_exist(rower); // очистка отсутствующих
	if (rower.size() < 1) { // выход с ошибкой при отсутствии 
		cout << "Нет книг по заданному придмету\n";
		system("pause");
		exit(0);
	}
	else 
		DB.db["Books"].print_table(rower); // печать списка книг
}

void chk_exist(vector <row> &rower) { // проверка наличия книг
	for (int i = 0; i < rower.size(); i++) { // цикл по книгам
		if (*(int*)DB.db["Books"].get_data(rower[i], "Quantity") - *(int*)DB.db["Books"].get_data(rower[i], "Number") < 1) { // проверка наличия
			rower.erase(rower.begin() + i); // удаление отсутствующих
			i--; } // смещение
	}
}
int get_subject_id(string name) { // id предмета
	vector <row> rower; // временный вектор
	DB.db["Subjects"].search("Subject",name, rower); // поиск предмета
	return *(int*)rower[0]["SubjectID"]; // возврат id предмета
}


void add_student() { // добавление студента
	string name, group, date_now, date_end; // временные переменные
	int id = DB.db["Students"].get_unic_id("StudentID"); // выделение уникального id
	cout << "Введите имя студента:"; 
	cin >> name; 
	cout << "Введите группу студента:";
	cin >> group;
	DB.db["Students"].add(to_string(id)+"|"+name+"|"+group); // добавление в СУБД
	cout << "Студент добавлен" << endl;
}

string get_date() { // текущая дата в виде строки 
	char date[20]; // временная строка 
	time_t td = time(0); // получение текущего времени
	tm * tmdate = localtime(&td); // преобразование в локальное
	strftime(date,sizeof(date),"%d.%m.%y",tmdate); // запись в строку
	string sdate(date); // преобразование в string
	return sdate; // возврат строки
}