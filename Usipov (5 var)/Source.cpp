#include "DBLibrary.h" // ����������� ���������� ����
#pragma comment(lib, "DBLibrary.lib") // ����������� ���������� ����
#include <iostream> // ������ ����� � ������
DBTableSet DB("DB"); // ���������� ��
void money_info(string, string, string); // ��������
int get_client_id(string); // id �������
vector <int> get_order_ids(int, string, string); // ������ id �������
void all_price(vector <int>); // �����
void main() { // ������� �������
	system("chcp 1251 > nul"); // ������� ����
	string client_name, data_start, data_finish; // ��� �������� ���������� ���
	cout << "������� ��� �������: ";
	cin >> client_name; // ���� ����� �������
	cout << "������� ���� ������: ";
	cin >> data_start; // ���� ���� ������ ���������
	cout << "������� ���� ������: ";
	cin >> data_finish; // ���� ���� ����� ���������
	money_info(client_name, data_start, data_finish); // ����� ��������
	system("pause"); // �����
}
void money_info(string client_name, string data_start, string data_finish) { // ��������
	all_price(get_order_ids(get_client_id(client_name), data_start, data_finish)); // ����
};
int get_client_id(string client_name) { // id �������
	vector <row> result; // ������ ��� ���������
	DB.db["Customers"].search("ContactName", client_name, result); // �����
	return *(int *)result[0]["CustomerID"]; // �������
};
vector <int> get_order_ids(int client_id, string data_start, string data_finish) { // ������ id �������
	vector <row> result; // ������ ��� ����������
	vector <int> result_2; // ������ ��� ��������
	string data; // ����
	DB.db["Orders"].search("CustomerID", client_id, result); // �����
	for (int i = 0; i < result.size(); i++) { // ���� �� ���������� �������
		data = value_to_string(DB.db["Orders"].get_data(result[i], "OrderDate")); // ������ ���� ������
		if ((data > data_start) && (data < data_finish)) // ���� ���� � ����������
			result_2.push_back(*(int *)result[i]["OrderID"]); // ������� � ������
	}
	return result_2; // �������
};
void all_price(vector <int> order_ids) { // �����
	int summ = 0; // �����
	for (int i = 0; i < order_ids.size(); i++) { // ���� �� ���� �������
		vector <row> result; // ������ ��� ����������
		DB.db["OrderDetails"].search("OrderID", order_ids[i], result); // �����
		summ += *(int*)DB.db["OrderDetails"].get_data(result[0], "UnitPrice") * *(int*)DB.db["OrderDetails"].get_data(result[0], "Quantity"); // ���������� �����
	}
	cout << summ << endl; // �����
};