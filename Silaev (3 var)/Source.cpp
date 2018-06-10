#include "DBLibrary.h" // подключение библиотеки СУБД
#pragma comment(lib, "DBLibrary.lib") // подключение библиотеки СУБД
#include <iostream> // импорт ввода и вывода
DBTableSet DB("DB"); // глобальная БД
void info(); // получение информации
void get_book_ids(vector <int> &); // id книг
void get_students(int, vector <int> &); // список студентов
void print_book(int); // печать книги
void print_students(vector <int> &); // печать студентов
void main() { // главная функция
	system("chcp 1251 > nul"); // русский язык
	info(); // вызов функции поиска книг
	system("pause"); // пауза
}
void info() { // получение информации
	vector <int> book_ids; // id книг
	vector <int> student_ids; // id студентов
	get_book_ids(book_ids); // id книг
	for (int i = 0; i < book_ids.size(); i++) { // цикл по всем книгам
		get_students(book_ids[i], student_ids); // id студентов
		print_book(book_ids[i]); // печать книги
		print_students(student_ids); // печать студетов
	}
};
void get_book_ids(vector <int> &book_ids) { // id книг
	vector <row> result; // результат
	DB.db["Books"].compare_equal("Quantity|Number", result); // поиск
	for (int i = 0; i < result.size(); i++) // цикл по результату
		book_ids.push_back(*(int *)result[i]["BookID"]); // заполнение
};
void get_students(int book_id, vector <int> &student_ids) { // студенты
	student_ids.clear(); // очистка
	vector <row> result; // результат
	DB.db["Abonements"].compare("BookID|" + to_string(book_id) + "|InDate|NULL", result); // поиск
	for (int i = 0; i < result.size(); i++) // цикл по результату
		student_ids.push_back(*(int *)result[i]["StudentID"]); // заполнение
};
void print_book(int book_id) { // печать книги
	vector <row> result; // результат
	DB.db["Books"].search("BookID", book_id, result); // поиск
	cout << value_to_string(result[0]["Title"]) << endl; // вывод
};
void print_students(vector <int> &student_ids) { // печать студентов
	for (int i = 0; i < student_ids.size(); i++) { // цикл по студентам
		vector <row> result; // результат
		DB.db["Students"].search("StudentID", student_ids[i], result); // поиск
		cout << value_to_string(result[0]["Name"]) << endl; // вывод
	}
};