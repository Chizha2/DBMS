#include "DBLibrary.h" // подключение библиотеки СУБД
#pragma comment(lib, "DBLibrary.lib") // подключение библиотеки СУБД
#include <iostream> // импорт ввода и вывода
DBTableSet DB("DB"); // глобальная БД
void give_book(string, string); // выдать книгу
bool have_book(string); // есть ли книга
int student_id(string); // id студента
bool much_books(int); // сколько книг
int subject_id(int); // id предмета
int book_id(string); // id книги
void number_up(string); // на 1 книгу больше выдано
void add(int, int); // добавить
void main() { // главная функция
	system("chcp 1251 > nul"); // русский язык
	string student_name, title; // имя студента и название книги
	cout << "Введите имя студента: ";
	cin >> student_name; // ввод имени студента
	cout << "Введите название книги: ";
	cin >> title; // ввод названия
	give_book(student_name, title); // вызов функции поиска книг
	system("pause"); // пауза
}
void give_book(string student_name, string title) { // выдача книги
	if (have_book(title) == 0) { // если книги нет
		cout << "Книги нет" << endl;
		return; // конец
	}
	if (much_books(student_id(student_name)) == 0) { // если долг в 2 книги по 1 предмету
		cout << "2 книги по 1 предмету" << endl;
		return; // конец
	}
	number_up(title); // выдано на 1 книгу больше
	add(student_id(student_name), book_id(title)); // добавление новой записи в Abonements
	cout << "Книга выдана" << endl;
	DB.writeDB("DBNew"); // запись 
};
bool have_book(string title) { // есть ли книга
	vector <row> result; // результат
	DB.db["Books"].search("Title", title, result); // поиск
	if (*(int *)result[0]["Number"] != *(int *)result[0]["Quantity"]) // если есть
		return true; // да
	return false; // нет
};
int student_id(string student_name) { // id студента
	vector <row> result; // результат
	DB.db["Students"].search("Name", student_name, result); // поиск
	return *(int*)result[0]["StudentID"]; // возвращение результата
};
bool much_books(int student_id) { // есть ли долг
	vector <int> mass; // вектор всех его книг
	vector <row> result; // результат
	DB.db["Abonements"].search("StudentID", student_id, result); // поиск
	for (int i = 0; i < result.size(); i++) // цикл по всем его книгам
		if (value_to_string(result[i]["InDate"]) == "NULL")  // если не вернул
			mass.push_back(subject_id(*(int *)result[i]["BookID"])); // добавить id предмета книги
	for (int i = 0; i < mass.size(); i++) // цикл по всем невозвратам
		for (int j = 0; j < mass.size(); j++) // цикл по всем невозвратам
			if ((i != j) && (mass[i] == mass[j])) // если есть пара
				return false; // нет
	return true; // да
};
int subject_id(int book_id) { // id книги
	vector <row> result; // результат
	DB.db["Books"].search("BookID", book_id, result); // поиск
	return *(int*)result[0]["SubjectID"]; // возвращение результата
};
int book_id(string title) { // id книги
	vector <row> result; // результат
	DB.db["Books"].search("Title", title, result); // поиск
	int out = *(int *)result[0]["BookID"];
	return out;
	//return *(int *)result[0]["BookID"]; // возвращение результата
};
void number_up(string title) { // на 1 кингу больше выдано
	vector <row> result; // результат
	DB.db["Books"].search("Title", title, result); // поиск
	DB.db["Books"].replace(result[0], "Number", (*(int *)(DB.db["Books"].get_data(result[0], "Number")))++); // изменение
};
void add(int student_id, int book_id) { // добавление
	DB.db["Abonements"].add(to_string(student_id) + "|" + to_string(book_id) + "|07.05.2017|NULL"); // добавление
};