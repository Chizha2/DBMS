#include "DBLibrary.h" // подключение библиотеки СУБД
#pragma comment(lib, "DBLibrary.lib") // подключение библиотеки СУБД
#include <iostream> // импорт ввода и вывода
DBTableSet DB("DB"); // глобальная БД
void money_info(string, string, string); // сценарий
int get_client_id(string); // id клиента
vector <int> get_order_ids(int, string, string); // список id заказов
void all_price(vector <int>); // сумма
void main() { // главная функция
	system("chcp 1251 > nul"); // русский язык
	string client_name, data_start, data_finish; // имя клиентаи промежуток дат
	cout << "Введите имя клиента: ";
	cin >> client_name; // ввод имени клиента
	cout << "Введите дату начала: ";
	cin >> data_start; // ввод даты начала интервала
	cout << "Введите дату начала: ";
	cin >> data_finish; // ввод даты конца интервала
	money_info(client_name, data_start, data_finish); // вызов сценария
	system("pause"); // пауза
}
void money_info(string client_name, string data_start, string data_finish) { // сценарий
	all_price(get_order_ids(get_client_id(client_name), data_start, data_finish)); // каеф
};
int get_client_id(string client_name) { // id клиента
	vector <row> result; // строки для рзультата
	DB.db["Customers"].search("ContactName", client_name, result); // поиск
	return *(int *)result[0]["CustomerID"]; // возврат
};
vector <int> get_order_ids(int client_id, string data_start, string data_finish) { // список id заказов
	vector <row> result; // строки для результата
	vector <int> result_2; // вектор для возврата
	string data; // дата
	DB.db["Orders"].search("CustomerID", client_id, result); // поиск
	for (int i = 0; i < result.size(); i++) { // цикл по полученным строкам
		data = value_to_string(DB.db["Orders"].get_data(result[i], "OrderDate")); // читаем дату заказа
		if ((data > data_start) && (data < data_finish)) // если дата в промежутке
			result_2.push_back(*(int *)result[i]["OrderID"]); // заносим в вектор
	}
	return result_2; // возврат
};
void all_price(vector <int> order_ids) { // сумма
	int summ = 0; // сумма
	for (int i = 0; i < order_ids.size(); i++) { // цикл по всем заказам
		vector <row> result; // строки для результата
		DB.db["OrderDetails"].search("OrderID", order_ids[i], result); // поиск
		summ += *(int*)DB.db["OrderDetails"].get_data(result[0], "UnitPrice") * *(int*)DB.db["OrderDetails"].get_data(result[0], "Quantity"); // увеличение суммы
	}
	cout << summ << endl; // вывод
};