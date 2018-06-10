#include "DBLibrary.h" // ����������� ���������� ����
#pragma comment(lib, "DBLibrary.lib") // ����������� ���������� ����
#include <iostream> // ������ ����� � ������
DBTableSet DB("DB"); // ���������� ��
void info(); // ��������� ����������
void get_book_ids(vector <int> &); // id ����
void get_students(int, vector <int> &); // ������ ���������
void print_book(int); // ������ �����
void print_students(vector <int> &); // ������ ���������
void main() { // ������� �������
	system("chcp 1251 > nul"); // ������� ����
	info(); // ����� ������� ������ ����
	system("pause"); // �����
}
void info() { // ��������� ����������
	vector <int> book_ids; // id ����
	vector <int> student_ids; // id ���������
	get_book_ids(book_ids); // id ����
	for (int i = 0; i < book_ids.size(); i++) { // ���� �� ���� ������
		get_students(book_ids[i], student_ids); // id ���������
		print_book(book_ids[i]); // ������ �����
		print_students(student_ids); // ������ ��������
	}
};
void get_book_ids(vector <int> &book_ids) { // id ����
	vector <row> result; // ���������
	DB.db["Books"].compare_equal("Quantity|Number", result); // �����
	for (int i = 0; i < result.size(); i++) // ���� �� ����������
		book_ids.push_back(*(int *)result[i]["BookID"]); // ����������
};
void get_students(int book_id, vector <int> &student_ids) { // ��������
	student_ids.clear(); // �������
	vector <row> result; // ���������
	DB.db["Abonements"].compare("BookID|" + to_string(book_id) + "|InDate|NULL", result); // �����
	for (int i = 0; i < result.size(); i++) // ���� �� ����������
		student_ids.push_back(*(int *)result[i]["StudentID"]); // ����������
};
void print_book(int book_id) { // ������ �����
	vector <row> result; // ���������
	DB.db["Books"].search("BookID", book_id, result); // �����
	cout << value_to_string(result[0]["Title"]) << endl; // �����
};
void print_students(vector <int> &student_ids) { // ������ ���������
	for (int i = 0; i < student_ids.size(); i++) { // ���� �� ���������
		vector <row> result; // ���������
		DB.db["Students"].search("StudentID", student_ids[i], result); // �����
		cout << value_to_string(result[0]["Name"]) << endl; // �����
	}
};