#include <string> // ������
#include <vector> // �������
#include <map> // ����������
using namespace std; // ������������ ����
typedef map <string, void *> row; // ������
typedef map <string, string> header; // ���������
string value_to_string(void *); // �������������� �� void *
class DBTable { // �������
private: // ������ ������
	vector <row> data; // ������
	string table_name; // ���
	header column_headers; // ���������
	vector <string> header_names; // ��������� ��� �����
	void *get_value(int value); // �������������� � void * �����
	void *get_value(string, string); // �������������� � void * ����� � �����
	string value_to_string(void *, string); // �������������� �� void *
	bool value_is_equal(string, void *, string); // ��������� ��������
	void replace(row &, string, void *); // ������ ��������
	bool column_ex(string); // �������� ������� ���������
	bool column_ex(vector <string> &); // �������� ������� ����������
public: // �����
	DBTable() {}; // ����������� �� ���������
	int get_unic_id(string); // ������ id
	void clear_trash(vector <row> &); // �������� ����������
	void read_table(string, string); // ������
	void print_table(); // ������ 
	void print_table(vector <row> &); // ������ 
	void write_table(string, string); // ������
	void search(string, void *, vector <row> &); // �����
	void search(string, int, vector <row> &); // �����
	void search(string, string, vector <row> &); // �����
	void clear(vector <row> &); // ��������
	void replace(vector <row> &, vector <row> &); // ������ �����
	void replace(row &, string, int); // ������ ��������
	void replace(row &, string, string); // ������ ��������
	void add(); //  ����������
	void add(string); // ����������
	void add(vector <row> &); // ����������
	void *get_data(row &, string); // ��������� ��������
	void compare(string, vector <row> &); // ���������
	void compare_equal(string, vector <row> &); // ��������� ���������
	// void compare_equal(vector <row> &, string, ...); ���������
};
class DBTableSet { // ��
private: // ������ ������
	vector <string> tabNames; // �������
	string DB_name; // ���
	string path; // ����
public: // �����
	map <string, DBTable> db; // �������
	DBTableSet() {}; // ����������� �� ���������
	DBTableSet(string); //  �����������
	DBTableSet(string, vector <string>); // �����������
	void readDB(string); // ������
	void readDB(string, string); // ������
	void printDB(); // ������
	void writeDB(); // ������
	void writeDB(string); // ������
	void writeDB(string, string); // ������
};