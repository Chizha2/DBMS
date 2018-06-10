#include "DBLibrary.h" // ����������� ���������� ����
#pragma comment(lib, "DBLibrary.lib") // ����������� ���������� ����
#include <iostream> // ������ ����� � ������
DBTableSet DB("DB"); // ���������� ��
vector <int> mass; // ���������� ����� �� ������� Abonements
void search_books(string, string); // ������� ������ ����
int get_student_id(string); // ������� ������ ID �������� �� ������� Students
int get_subject_id(string); // ������� ������ ID �������� �� ������� Subjects
void get_book_ids_1(int); // ������� ������ ���������� ����
void get_book_ids_2(int); // ������� ������ ���������� ����
bool compare(int); // ������� ���������
void main() { // ������� �������
	system("chcp 1251 > nul"); // ������� ����
	string student_name, subject_name; // ��� �������� � �������� ��������
	cout << "������� ��� ��������: ";
	cin >> student_name; // ���� ����� ��������
	cout << "������� �������� ��������: ";
	cin >> subject_name; // ���� �������� ��������
	search_books(student_name, subject_name); // ����� ������� ������ ����
	system("pause"); // �����
}
void search_books(string student_name, string subject_name) { // ������� ������ ����
	get_book_ids_2(get_student_id(student_name)); // ����� ������� ������ �������� ���� �� ������� Abonements, �������� - ������� ������ ID �������� �� ������� Students
	get_book_ids_1(get_subject_id(subject_name)); // ����� ������� ������ ���������� ���� �� ������� Books (� ������ �������� �������� ����), �������� - ������� ������ ID �������� �� ������� Subjects
};
int get_student_id(string student_name) { // ������� ������ ID �������� �� ������� Students
	vector <row> result; // ���������
	DB.db["Students"].search("Name", student_name, result); // �����
	return *(int*)result[0]["StudentID"]; // ����������� ����������
};
int get_subject_id(string subject_name) { // ������� ������ ID �������� �� ������� Subjects
	vector <row> result; // ���������
	DB.db["Subjects"].search("Subject", subject_name, result); // �����
	return *(int*)result[0]["SubjectID"]; // ����������� ����������
};
void get_book_ids_1(int subject_id) { // ������� ������ ���������� ����
	vector <row> result; // ���������
	DB.db["Books"].search("SubjectID", subject_id, result); // ����� �� �������
	for (int i = 0; i < result.size(); i++) // ���� �� �����������
		if (compare(*(int*)result[i]["BookID"])) // ����� ������� ���������
			cout << value_to_string(result[i]["Title"]) << endl; // ����� �����������
};
void get_book_ids_2(int student_id) { // ������� ������ ���������� ����
	vector <row> result; // ���������
	DB.db["Abonements"].search("StudentID", student_id, result); // ����� �� �������
	for (int i = 0; i < result.size(); i++)  // ���� �� �����������
		if (value_to_string(result[i]["InDate"]) == "NULL") // �������� �������
			mass.push_back(*(int*)result[i]["BookID"]); // ���������
};
bool compare(int BookID) { // ������� ���������
	for (int i = 0; i < mass.size(); i++) // �� ���� ������ �������
		if (BookID == mass[i]) { // �������� �������
			mass.erase(mass.begin() + i); // �������� 
			return true; // ���������� 1
		}
	return false; // ���������� 0
};