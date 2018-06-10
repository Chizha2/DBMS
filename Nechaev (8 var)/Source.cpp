#include "DBLibrary.h" // ����������� ���������� ����
#pragma comment(lib, "DBLibrary.lib") // ����������� ���������� ����
#include <iostream> // ������ ����� � ������
#include <iomanip>


DBTableSet DB("DB"); // ���������� ��
void add_student();
int get_subject_id(string);
void print_books_by_sid(int sid);
void chk_exist(vector <row> &rower);
void give_book_by_subject();
void give_book(int, int);
string get_date();

int main() {
	system("chcp 1251 > nul");
	add_student(); // ���������� ��������
	give_book_by_subject(); // ������ �����, �� ��������
	DB.writeDB(); // ���������� ���������
	system("pause");
	return 0;
}

void give_book_by_subject() { // ������ ���������� ��������
	vector <row> rower; // ��������� ������
	int bid; // id �����
	string s_name; 
	cout << "������� �������� ��������: ";
	cin >> s_name;
	cout << "� ���������� �������: " << endl;
	int sid = get_subject_id(s_name); // ��������� id ��������
	print_books_by_sid(sid); // ����� ������ ���� �� ��������
	cout << "������� ID ����� ��� ������: "; 
	cin >> bid;
	give_book(sid,bid); // ������ �����
	cout << "����� ������\n";
}
void give_book(int sid, int bid) // ������ �����
{
	vector <row> rower; // ��������� ������
	DB.db["Books"].search("BookID", bid, rower); // ��������� ������� ����� 
	DB.db["Books"].replace(rower[0], "Number", (*(int *)(DB.db["Books"].get_data(rower[0], "Number")))--); // ���������� ���������� ���� � ����
	DB.db["Abonements"].add(to_string(sid)+"|"+to_string(bid)+"|"+get_date()+"|NULL"); // ������ ������ � ������
}
void print_books_by_sid(int sid) { // ����� ���� �� id ��������
	vector <row> rower; // ��������� ������
	DB.db["Books"].search("SubjectID", sid ,rower); // ����� ���� ���� �� ��������
	chk_exist(rower); // ������� �������������
	if (rower.size() < 1) { // ����� � ������� ��� ���������� 
		cout << "��� ���� �� ��������� ��������\n";
		system("pause");
		exit(0);
	}
	else 
		DB.db["Books"].print_table(rower); // ������ ������ ����
}

void chk_exist(vector <row> &rower) { // �������� ������� ����
	for (int i = 0; i < rower.size(); i++) { // ���� �� ������
		if (*(int*)DB.db["Books"].get_data(rower[i], "Quantity") - *(int*)DB.db["Books"].get_data(rower[i], "Number") < 1) { // �������� �������
			rower.erase(rower.begin() + i); // �������� �������������
			i--; } // ��������
	}
}
int get_subject_id(string name) { // id ��������
	vector <row> rower; // ��������� ������
	DB.db["Subjects"].search("Subject",name, rower); // ����� ��������
	return *(int*)rower[0]["SubjectID"]; // ������� id ��������
}


void add_student() { // ���������� ��������
	string name, group, date_now, date_end; // ��������� ����������
	int id = DB.db["Students"].get_unic_id("StudentID"); // ��������� ����������� id
	cout << "������� ��� ��������:"; 
	cin >> name; 
	cout << "������� ������ ��������:";
	cin >> group;
	DB.db["Students"].add(to_string(id)+"|"+name+"|"+group); // ���������� � ����
	cout << "������� ��������" << endl;
}

string get_date() { // ������� ���� � ���� ������ 
	char date[20]; // ��������� ������ 
	time_t td = time(0); // ��������� �������� �������
	tm * tmdate = localtime(&td); // �������������� � ���������
	strftime(date,sizeof(date),"%d.%m.%y",tmdate); // ������ � ������
	string sdate(date); // �������������� � string
	return sdate; // ������� ������
}