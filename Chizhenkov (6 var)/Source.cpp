#include "DBLibrary.h" // подключение библиотеки СУБД
#pragma comment(lib, "DBLibrary.lib") // подключение библиотеки СУБД
#include <iostream> // импорт ввода и вывода
DBTableSet DB("DB"); // глобальная БД
vector <int> mass; // подходящие книги из таблицы Abonements
void search_books(string, string); // функция поиска книг
int get_student_id(string); // функция поиска ID студента из таблицы Students
int get_subject_id(string); // функция поиска ID предмета из таблицы Subjects
void get_book_ids_1(int); // функция поиска подходящих книг
void get_book_ids_2(int); // функция поиска подходящих книг
bool compare(int); // функция сравнения
void main() { // главная функция
	system("chcp 1251 > nul"); // русский язык
	string student_name, subject_name; // имя студента и название предмета
	cout << "Введите имя студента: ";
	cin >> student_name; // ввод имени студента
	cout << "Введите название предмета: ";
	cin >> subject_name; // ввод названия предмета
	search_books(student_name, subject_name); // вызов функции поиска книг
	system("pause"); // пауза
}
void search_books(string student_name, string subject_name) { // функция поиска книг
	get_book_ids_2(get_student_id(student_name)); // вызов функции поиска выданных книг из таблицы Abonements, аргумент - функция поиска ID студента из таблицы Students
	get_book_ids_1(get_subject_id(subject_name)); // вызов функция поиска подходящих книг из таблицы Books (с учетом выданных студенту книг), аргумент - функция поиска ID предмета из таблицы Subjects
};
int get_student_id(string student_name) { // функция поиска ID студента из таблицы Students
	vector <row> result; // результат
	DB.db["Students"].search("Name", student_name, result); // поиск
	return *(int*)result[0]["StudentID"]; // возвращение результата
};
int get_subject_id(string subject_name) { // функция поиска ID предмета из таблицы Subjects
	vector <row> result; // результат
	DB.db["Subjects"].search("Subject", subject_name, result); // поиск
	return *(int*)result[0]["SubjectID"]; // возвращение результата
};
void get_book_ids_1(int subject_id) { // функция поиска подходящих книг
	vector <row> result; // результат
	DB.db["Books"].search("SubjectID", subject_id, result); // поиск по таблице
	for (int i = 0; i < result.size(); i++) // цикл по результатам
		if (compare(*(int*)result[i]["BookID"])) // вызов функции сравнения
			cout << value_to_string(result[i]["Title"]) << endl; // вывод результатов
};
void get_book_ids_2(int student_id) { // функция поиска подходящих книг
	vector <row> result; // результат
	DB.db["Abonements"].search("StudentID", student_id, result); // поиск по таблице
	for (int i = 0; i < result.size(); i++)  // цикл по результатам
		if (value_to_string(result[i]["InDate"]) == "NULL") // проверка условия
			mass.push_back(*(int*)result[i]["BookID"]); // видимость
};
bool compare(int BookID) { // функция сравнения
	for (int i = 0; i < mass.size(); i++) // по всем членам вектора
		if (BookID == mass[i]) { // проверка условия
			mass.erase(mass.begin() + i); // удаление 
			return true; // возвращает 1
		}
	return false; // возвращает 0
};