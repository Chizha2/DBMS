#include "DBLibrary.h" // ���������
#include <iostream> // ���� � �����
#include <fstream> // ������
#include <sstream> // ������ ��� �����
// #include <stdarg.h> ��� va_list �������
string value_to_string(void *value) { // �������������� �� void *
	ostringstream tmp; // ����� 
	tmp << (char *)value; // ������ 
	return tmp.str(); // ����������� ������
};
void *DBTable::get_value(int value) { // �������������� � void *
	return new int(value); // ��������� ������ ��� ����� � ����������� ���������
};
void *DBTable::get_value(string value, string columnName) { // �������������� � void *
	if (column_headers[columnName] == "Int32") // ���� �����
		return new int(atoi(value.c_str())); // ��������� ������ ��� ����� � ����������� ���������
	char *tmp = new char[value.size() + 1]; // ������ ��������
	sprintf(tmp, "%s", value.c_str()); // ����������� � ������
	return tmp; // ����������� ���������
};
string DBTable::value_to_string(void *value, string columnName) { // �������������� �� void *
	ostringstream tmp; // �����
	if (column_headers[columnName] == "Int32") // ���� �����
		tmp << *(int *)value; // ������ � ����� ��� �����
	else
		tmp << (char *)value; // ������ � ����� ��� ������ ��������
	return tmp.str(); // ����������� ������
};
bool DBTable::value_is_equal(string columnName, void* data, string value) { // ��������� ��������
	if (value_to_string(data, columnName) == value) // ������� ���������
		return true; // ����������� 1
	else
		return false; // ����������� 0
};
void DBTable::search(string column_name, void* void_value, vector <row> &rower) { // �����
	string value = value_to_string(void_value, column_name); // �������������� � ������
	if (rower.size() == 0 || rower.size() == 1) // ������� ����������
		rower = data; // ������������ ������
	for (int i = 0; i < rower.size(); i++) // ���� �� ������� �������
		if (!value_is_equal(column_name, rower[i][column_name], value)) { // �������� ������� ������
			rower.erase(rower.begin() + i); // �������� ������
			i--; // ���������� ��������
		}
};
void DBTable::read_table(string tab_name, string path) { // ������
	int k = 1; // �������
	string tempToken; // ��������� ������
	table_name = tab_name; // ������������ �����
	char buf[500], *delim = "|\r", *token, *nextToken = buf; // �����, �����������, ������� � �������
	ifstream file(path + "\\" + tab_name + ".txt", ios::binary); // �������� ������
	file.getline(buf, 500); // ������ ������ ������
	while (token = strtok_s(nextToken, delim, &nextToken)) { // ���� ������� �� ��������
		if (k % 2 == 0) { // ������� ��������
			column_headers[tempToken] = token; // ���������� ��������� �������
			header_names.push_back(tempToken); // ���������� ��������� ������� ��� �����
		}
		else
			tempToken = token; // ������������ �������� ��������� ������
		k++; // ���������� ��������
	}
	while (file.getline(buf, 500)) { // ������ ��������� �����
		k = 0; // ��������� ��������
		row tempRow; // ��������� ���������
		char *nextToken_row = buf; // ������ nexttoken
		while (token = strtok_s(nextToken_row, delim, &nextToken_row)) { // ���� ������� �� ��������
			tempRow[header_names[k]] = get_value(token, header_names[k]); // ���������� �������
			k++; // ���������� ��������
		}
		data.push_back(tempRow); // ������ � ������
	}
	file.close(); // �������� ������
};
void DBTable::print_table() { // ������
	print_table(data); // ����� �������������� ������ ������ �������
};
void DBTable::print_table(vector <row> &rower) { // ������ �������
	cout << table_name << endl; // ����� �������� 
	for (int k = 0; k < header_names.size(); k++) // ���� �� ��������� 
		cout << header_names[k] << " "; // ����� ��������� 
	cout << endl; // ������ 
	for (int k = 0; k < rower.size(); k++) { // ���� �� ������� 
		for (int i = 0; i < header_names.size(); i++) { // ���� �� �������� 
			cout << value_to_string(rower[k][header_names[i]], header_names[i]); // ����� ������ 
			if (i != header_names.size() - 1) // ������� ����� ������
				cout << " ~ "; // ������ ����������� 
		}
		cout << endl; // ������ 
	}
	cout << endl; // ������ 
};
void DBTable::write_table(string tab_name, string path) { // ������
	ofstream file(path + "\\" + tab_name + ".txt", ofstream::out | ofstream::trunc | ofstream::binary); // �������� ������
	int k; // �������
	for (k = 0; k < header_names.size(); k++) { // ���� ������ ���������
		file << header_names[k] << "|" << column_headers[header_names[k]]; // ������ 
		if (k != header_names.size() - 1) // ������� ����� ������
			file << "|"; // ������ �����������
	}
	file << endl; // ������� ������
	for (k = 0; k < data.size(); k++) { // ���� �� �������
		for (int i = 0; i < header_names.size(); i++) { // ���� �� ��������
			file << value_to_string(data[k][header_names[i]], header_names[i]); // ������ ������
			if (i != header_names.size() - 1) // ������� ����� ������
				file << "|"; // ������ �����������
		}
		if (k != data.size() - 1) // ������� ����� �������
			file << endl; // ������� ������
	}
	file.close(); // �������� �����
};
void DBTable::search(string column_name, int value, vector <row> &rower) { // �����
	search(column_name, get_value(value), rower); // ����� ������ ������ � �������
};
void DBTable::search(string column_name, string value, vector <row> &rower) { // �����
	search(column_name, get_value(value, column_name), rower); // ����� ������ ������ � �������
};
int DBTable::get_unic_id(string column_name) { // ��������� ����������� id
	if (!column_ex(column_name)) { // �������� ������������� �������
		cout << "[GET_UID]������� �� ������\n";
		return -1; // ������� ���� ������
	}
	if (column_headers[column_name] != "Int32") { // �������� ���� ������� ������
		cout << "[GET_UID]������� �� �������� int\n";
		return -2; // ������� ���� ������
	}
	int uid = 1; // ���������� ���������� id
	bool unic = 1; // ���� ������������ 
	while (uid != INT_MAX) { // ���� �� ������������� �������� int �������
		unic = 1; // �������������� �����
		for (int k = 0; k < data.size(); k++) // ���� ������������� �������� id � �������������
			if (value_to_string(data[k][column_name], column_name) == to_string(uid)) {
				unic = 0; // ����� �����
				break; // ����� �� �����
			}
		if (unic == 1) // ���� ��������� id ������
			return uid; // ������� id �� �������
		uid++; // ���������� id
	}
	/* ����� ������ ��� ���������� ������������� int */
	cout << "[GET_UID]������!\n ���������� ������������ ���������� ID � �������\n";
	return -3; // ������� ���� ������
};

bool DBTable::column_ex(string columnName) { // �������� ������������� ���������
	for (int i = 0; i < header_names.size(); i++) // ���� �� ���������� �������
		if (header_names[i] == columnName) // ��������
			return 1; // ���� ����������
	return 0; // ���� �� ����������
}
bool DBTable::column_ex(vector <string> &columnName) { // �������� ������������� ���������� ����������
	for (int i = 0; i < columnName.size(); i++) // ���� �� �� ������� ����������
		if (column_ex(columnName[i]) == 0) // ��������
			return 0; // ���� ����������
	return 1; // ���� �� ����������
}
/* 
void DBTable::compare_equal(vector <row> &rower, string type, ...) {
	vector <string> res_column;
	int k = -1, tmp = 0; // 2 ��������
	char buf[50]; // �����
	row Rower; // ��������� ���������
	va_list ap;
	va_start(ap, type);
	if (type == "int")
	{
		int tmp = va_arg(ap, int);
		res_column.push_back(to_string(tmp));
	}
	if (type == "string")
	{
		string tmp = va_arg(ap, string);
		res_column.push_back(tmp);
	}
	va_end(ap);
	if (column_ex(res_column) == 0) {
		cout << "[CMP_EQL]��� ������� �� �������" << endl;
		return;
	}
	for (int i = 0; i < data.size(); i++)
		for (int j = 0; j < res_column.size(); j++) {
			for (int k = 0; k < res_column.size(); k++) {
				if ((value_to_string(data[i][res_column[j]], res_column[j]) == value_to_string(data[i][res_column[k]], res_column[k])) && (j != k))
					rower.push_back(data[i]);
			}
		}
	clear_trash(rower);
}; 
*/
void DBTable::compare_equal(string intro, vector <row> &rower) {
	vector <string> res_column;
	int k = -1, tmp = 0; // 2 ��������
	char buf[50]; // �����
	row Rower; // ��������� ���������
	for (int i = 0; i < intro.size(); i++) // ���� �� ��������� ������
		if (intro.c_str()[i] == '|') { // ������� ������� �����������
			k++; // ���������� ��������
			intro.copy(buf, i - tmp, tmp); // ����������� ������� � �����
			buf[i - tmp] = '\0'; // ����� �������
			res_column.push_back(buf);
			tmp = i + 1; // ��������� ��������
		}
	intro.copy(buf, intro.size() - tmp, tmp); // ����������� ��������� �������
	buf[intro.size() - tmp] = '\0'; // ����� �������
	res_column.push_back(buf); // ������ 
	if (column_ex(res_column) == 0) { // ���� ������
		cout << "[CMP_EQL]��� ������� �� �������" << endl;
		return; // �����
	}
	for (int i = 0; i < data.size(); i++) // ���� �� �������
		for (int j = 0; j < res_column.size(); j++) // ���� �� ������ ��������
			for (int k = 0; k < res_column.size(); k++)  // ��������� ����
				if ((value_to_string(data[i][res_column[j]], res_column[j]) == value_to_string(data[i][res_column[k]], res_column[k])) && (j != k)) // ������� ���������
					rower.push_back(data[i]); // ������
	clear_trash(rower); // ������� �� ����������
};
void DBTable::clear_trash(vector <row> &rower) { // �������� ������ �������
	for (int i = 0; i < rower.size(); i++) // ���� �� i
		for (int j = 0; j < rower.size(); j++) // ���� �� j
			if ((rower[j] == rower[i]) && (i != j)) // �������� ����������
				rower.erase(rower.begin() + i); // �������� ���������
}
void DBTable::compare(string intro, vector <row> &rower) { // ���������
	vector <string> res_column; // ���������
	vector <string> res_data; // ������
	int k = -1, tmp = 0; // 2 ��������
	char buf[50]; // �����
	row Rower; // ��������� ���������
	for (int i = 0; i < intro.size(); i++) // ���� �� ��������� ������
		if (intro.c_str()[i] == '|') { // ������� ������� �����������
			k++; // ���������� ��������
			intro.copy(buf, i - tmp, tmp); // ����������� ������� � �����
			buf[i - tmp] = '\0'; // ����� �������
			if (k % 2 == 0) // ���������
				res_column.push_back(buf); // ������
			else // �����
				res_data.push_back(buf); // ������
			tmp = i + 1; // ��������� ��������
		}
	intro.copy(buf, intro.size() - tmp, tmp); // ����������� ��������� �������
	buf[intro.size() - tmp] = '\0'; // ����� �������
	res_data.push_back(buf); // ������ ����������
	for (int i = 0; i < data.size(); i++) { // ���� �� �������
		int t = 0; // �������
		for (int j = 0; j < res_column.size(); j++) { // ���� �� ��������
			if (value_to_string(data[i][res_column[j]], res_column[j]) == res_data[j]) // ���� ��������
				t++; // ���������� ��������
			if (t == res_column.size()) // ���� ��� �������
				rower.push_back(data[i]); // ������
		}
	}
};
void DBTable::clear(vector <row> &rower) { // ��������
	for (int k = 0; k < rower.size(); k++) // ���� �� �������� �������
		for (int i = 0; i < data.size(); i++) // ���� �� ������� �������
			if (data[i] == rower[k]) // ������� ��������
				data.erase(data.begin() + i); // �������� ������
}
void DBTable::replace(vector <row> &rower, vector <row> &replace) { // ������
	if (rower.size() != replace.size()) // ��������
		return; // ����������
	for (int k = 0; k < rower.size(); k++) // ���� �� �������� �������
		for (int i = 0; i < data.size(); i++) // ���� �� ������� �������
			if (data[i] == rower[k]) // ������� ������
				data[i] = replace[k]; // ������ ������
}
void DBTable::replace(row &rower, string column_name, int value) { // ������
	replace(rower, column_name, get_value(value)); // ����� ������ ������ � �������
}
void DBTable::replace(row &rower, string column_name, string value) { // ������
	replace(rower, column_name, get_value(value, column_name)); // ����� ������ ������ � �������
}
void DBTable::replace(row &rower, string column_name, void *value) { // ������
	rower[column_name] = value; // ������
}
void *DBTable::get_data(row &rower, string column_name) { // ��������� ��������
	return rower[column_name]; // ���������
};
void DBTable::add() { // ����������
	string answer; // �����
	cout << "������� ������ �������: ";
	cin >> answer; // ����
	add(answer); // ����� ������ ����������
};
void DBTable::add(string intro) { // ����������
	int k = -1, tmp = 0; // 2 ��������
	char buf[50]; // �����
	row Rower; // ��������� ���������
	for (int i = 0; i < intro.size(); i++) // ���� �� ��������� ������
		if (intro.c_str()[i] == '|') { // ������� ������� �����������
			k++; // ���������� ��������
			intro.copy(buf, i - tmp, tmp); // ����������� ������� � �����
			buf[i - tmp] = '\0'; // ����� �������
			Rower[header_names[k]] = get_value(buf, header_names[k]); // ���������� �������
			tmp = i + 1; // ��������� ��������
		}
	intro.copy(buf, intro.size() - tmp, tmp); // ����������� ��������� �������
	buf[intro.size() - tmp] = '\0'; // ����� �������
	Rower[header_names[k + 1]] = get_value(buf, header_names[k + 1]); // ���������� ������� ��������� ��������
	if ((k + 2) != header_names.size()) // ������� ���������� ���������� ������
		cout << "[ADD]������������ ������� ������" << endl;
	else
		data.push_back(Rower); // ���������� ������
};
void DBTable::add(vector <row> &rower) { // ����������
	for (int i = 0; i < rower.size(); i++)
		data.push_back(rower[i]);
}
DBTableSet::DBTableSet(string name) { // ����������� 
	readDB(name); // ����� ������ ������ ��
};
DBTableSet::DBTableSet(string name, vector <string> intabname) { // �����������
	DB_name = name; // ������������ �����
	tabNames = intabname; // ������������ ���� ������
};
void DBTableSet::readDB(string name) { // ������
	readDB(name, name); // ����� �������������� ������ ������ ��
};
void DBTableSet::readDB(string name, string in_path) { // ������
	path = in_path; // ������������ ����
	DB_name = name; // ������������ �����
	char buf[500], *delim = "|\r", *token, *nextToken = buf; // �����, �����������, ������� � �������
	ifstream file(path + "\\" + DB_name + ".txt", ios::binary); // �������� ������
	if (!file.is_open()) { // �������� ���������� �������� �����
		perror("[READ_DB]Error open file:"); // ������ ������ �� ������
		cout << endl; // ������
		system("pause"); // �����
		return; // ����� �� �������
	}
	file.getline(buf, 500); // ������ �� �������
	while (token = strtok_s(nextToken, delim, &nextToken)) // ���� �� ��������
		tabNames.push_back(token); // ���������� ������� �������
	for (int i = 0; i < tabNames.size(); i++) // ���� �� ���� ��������
		db[tabNames[i]].read_table(tabNames[i], path); // ������ �������
	file.close(); // �������� ������
};
void DBTableSet::printDB() { // ������
	for (int i = 0; i < tabNames.size(); i++) // ���� �� ���� ��������
		db[tabNames[i]].print_table(); // ������ ������
};
void DBTableSet::writeDB() { // ������
	writeDB(DB_name, path); // ����� �������������� ��������� ������ ��
};
void DBTableSet::writeDB(string name) { // ������
	writeDB(name, name); // ����� �������������� ��������� ������ ��
};
void DBTableSet::writeDB(string name, string in_path) { // ����� �������������� ��������� ������ ��
	system(string("mkdir \"" + in_path + "\" 2> NUL").c_str()); // �������� ����� ��� ��
	ofstream file; // �������� ������ ������
	file.open(in_path + "\\" + name + ".txt", ofstream::trunc | ofstream::binary); // �������� ����� �� ������
	if (!file.is_open()) { // �������� ���������� �������� �����
		perror("[WRITE_DB]Error open file:"); // ������ ������ �� ������
		cout << endl; // ������� ������
		system("pause"); // �����
		return; // ����� �� �������
	}
	for (int i = 0; i < tabNames.size(); i++) { // ���� ������ �� ������ ������
		file << tabNames[i]; // ������ ����� �������
		if (i != tabNames.size() - 1) // �������� �� ����� ������
			file << "|"; // ������ �����������
	}
	file.close(); // �������� ������
	for (int i = 0; i < tabNames.size(); i++) // ���� ������ ������
		db[tabNames[i]].write_table(tabNames[i], in_path); // ����� ������ ������ ������
};