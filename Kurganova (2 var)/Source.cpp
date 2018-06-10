#include "DBLibrary.h" // ����������� ���������� ����
#pragma comment(lib, "DBLibrary.lib") // ����������� ���������� ����
#include <iostream> // ������ ����� � ������
DBTableSet DB("DB"); // ���������� ��
void give_book(string, string); // ������ �����
bool have_book(string); // ���� �� �����
int student_id(string); // id ��������
bool much_books(int); // ������� ����
int subject_id(int); // id ��������
int book_id(string); // id �����
void number_up(string); // �� 1 ����� ������ ������
void add(int, int); // ��������
void main() { // ������� �������
	system("chcp 1251 > nul"); // ������� ����
	string student_name, title; // ��� �������� � �������� �����
	cout << "������� ��� ��������: ";
	cin >> student_name; // ���� ����� ��������
	cout << "������� �������� �����: ";
	cin >> title; // ���� ��������
	give_book(student_name, title); // ����� ������� ������ ����
	system("pause"); // �����
}
void give_book(string student_name, string title) { // ������ �����
	if (have_book(title) == 0) { // ���� ����� ���
		cout << "����� ���" << endl;
		return; // �����
	}
	if (much_books(student_id(student_name)) == 0) { // ���� ���� � 2 ����� �� 1 ��������
		cout << "2 ����� �� 1 ��������" << endl;
		return; // �����
	}
	number_up(title); // ������ �� 1 ����� ������
	add(student_id(student_name), book_id(title)); // ���������� ����� ������ � Abonements
	cout << "����� ������" << endl;
	DB.writeDB("DBNew"); // ������ 
};
bool have_book(string title) { // ���� �� �����
	vector <row> result; // ���������
	DB.db["Books"].search("Title", title, result); // �����
	if (*(int *)result[0]["Number"] != *(int *)result[0]["Quantity"]) // ���� ����
		return true; // ��
	return false; // ���
};
int student_id(string student_name) { // id ��������
	vector <row> result; // ���������
	DB.db["Students"].search("Name", student_name, result); // �����
	return *(int*)result[0]["StudentID"]; // ����������� ����������
};
bool much_books(int student_id) { // ���� �� ����
	vector <int> mass; // ������ ���� ��� ����
	vector <row> result; // ���������
	DB.db["Abonements"].search("StudentID", student_id, result); // �����
	for (int i = 0; i < result.size(); i++) // ���� �� ���� ��� ������
		if (value_to_string(result[i]["InDate"]) == "NULL")  // ���� �� ������
			mass.push_back(subject_id(*(int *)result[i]["BookID"])); // �������� id �������� �����
	for (int i = 0; i < mass.size(); i++) // ���� �� ���� �����������
		for (int j = 0; j < mass.size(); j++) // ���� �� ���� �����������
			if ((i != j) && (mass[i] == mass[j])) // ���� ���� ����
				return false; // ���
	return true; // ��
};
int subject_id(int book_id) { // id �����
	vector <row> result; // ���������
	DB.db["Books"].search("BookID", book_id, result); // �����
	return *(int*)result[0]["SubjectID"]; // ����������� ����������
};
int book_id(string title) { // id �����
	vector <row> result; // ���������
	DB.db["Books"].search("Title", title, result); // �����
	int out = *(int *)result[0]["BookID"];
	return out;
	//return *(int *)result[0]["BookID"]; // ����������� ����������
};
void number_up(string title) { // �� 1 ����� ������ ������
	vector <row> result; // ���������
	DB.db["Books"].search("Title", title, result); // �����
	DB.db["Books"].replace(result[0], "Number", (*(int *)(DB.db["Books"].get_data(result[0], "Number")))++); // ���������
};
void add(int student_id, int book_id) { // ����������
	DB.db["Abonements"].add(to_string(student_id) + "|" + to_string(book_id) + "|07.05.2017|NULL"); // ����������
};