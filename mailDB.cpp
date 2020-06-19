
#include "./mailDB.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
/////////////////////    Class Mail member functions    /////////////////////
/////////////////////////////////////////////////////////////////////////////

bool 
Mail::searchContent(string& keyword)
{
    return false;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////   Class MailDB member functions   /////////////////////
/////////////////////////////////////////////////////////////////////////////
string 
MailDB::add(string& path)
{
	// ID 須先轉為五位數
	// 假設已經有一個大的vector 分別裝 From / Date / ID / Subject / To / Content 叫做 new_mail

	if (database.find(new_mail[2]) != database.end()) // 已經有這封信了
		return "-" ;
	// 還沒有這封信
	vector<stirng> data(5, "data");
	data[0] = new_mail[0];
	data[1] = new_mail[1];
	data[2] = new_mail[3];
	data[3] = new_mail[4];
	data[4] = new_mail[5];
	pair <string, vector<string>> database_data(new_mail[2], data);
	database.insert(mail_data); // 新增這筆資料

	string id_date = new_mail[2] + new_mail[1]; // 要放進 unordered_set 的資料
	unordered_set< string > id_date_set({id_date});
	unordered_set< string > current_set;

	// from_dict
	if (from_dict.find(new_mail[0]) == from_dict.end()) // 新的寄件人
	{
		pair < string, unordered_set<string> > from_dict_data(new_mail[0], id_date_set);
		from_dict.insert(from_dict_data); 
	}
	else // 已出現過的寄件人
	{
		current_set = from_dict[new_mail[0]];
		current_set.insert(id_date);
		from_dict[new_mail[0]] = current_set; // 在原有的 unordered_set 補上 
	}

	// to_dict
	if (to_dict.find(new_mail[4]) == from_dict.end()) // 新的收件人
	{
		pair < string, unordered_set<string> > to_dict_data(new_mail[4], id_date_set);
		to_dict.insert(from_dict_data); 
	}
	else // 已出現過的收件人
	{
		current_set = to_dict[new_mail[4]];
		current_set.insert(id_date);
		from_dict[new_mail[4]] = current_set; // 在原有的 unordered_set 補上 
	}

	// word_dict
	int first_index = 0;
	string content = new_mail[5];
	string word;
	int word_count = 1;
	for (int index = 0; index < content.size(); index++) // 把 id_date 加到每個字的 unordered_set
	{
		if (content[index] == ' ' or index == content.size() - 1) // space 
		{
			word = content.substr(first_index, index - first_index + 1);
			if (word_dict[word] == word_dict.end()) // new word
			{
				pair < string, unordered_set<string> > word_dict_data(word, id_date_set);
				word_dict.insert(word_dict_data); 
			}
			else
			{
				current_set = word_dict[word];
				current_set.insert(id_date);
				word_dict[word] = current_set;
			}
			word_count++;
			first_index = index + 1 // first index of the next word
		}
	}
	vector<int> word_count_id(2, 0);
	word_count_id[0] = word_count;
	word_count_id[1] = stoi(new_mail[2]);
	longest_set.insert(word_count_id);

	// subject
	word = new_mail[3];
	if (word_dict[word] == word_dict.end()) // new word
	{
		pair < string, unordered_set<string> > word_dict_data(word, id_date_set);
		word_dict.insert(word_dict_data); 
	}
}

void 
MailDB::remove(unsigned id)
{
	if (database.find(new_mail[2]) != database.end()) // 信件不存在
		return "-" ;
	// 信件存在
	vector<string> stored_data = database[to_string(id)];
	string id_date = stored_data[2] + stored_data[1];
	database.erase[to_string(id)];

	unordered_set< string > current_set;

	// from_dict
	current_set = from_dict[stored_data[0]];
	current_set.erase(id_date);
	from_dict[stored_data[0]] = current_set;

	// to_dict
	current_set = to_dict[stored_data[4]];
	current_set.erase(id_date);
	to_dict[stored_data[4]] = current_set;

	// word_dict
	int first_index = 0, word_count = 1;
	string content = stored_data[5];
	string word;
	for (int index = 0; index < content.size(); index++) // 把 id_date 加到每個字的 unordered_set
	{
		if (content[index] == ' ' or index == content.size() - 1) // space 
		{
			word = content.substr(first_index, index - first_index + 1);
			current_set = word_dict[word];
			current_set.erase(id_date);
			word_dict[word] = current_set;
			first_index = index + 1 // first index of the next word
		}
		word_count++;
	}
	// subject
	word = stored_data[3];
	current_set = word_dict[word];
	current_set.erase(id_date);
	word_dict[word] = current_set;

	vector<int> word_count_id(2, 0);
	word_count_id[0] = word_count;
	word_count_id[1] = stoi(stored_data[2]);
	longest_set.erase(word_count_id);
}

void 
MailDB::longest()
{
	return (*longest_set.rbegin())[1];
}

void 
MailDB::queryOnlyExpr(string& expr)
{
}

void 
MailDB::queryWithCond(vector<string>& args)
{
	string start_date = "0", end_date = "202100000000"; // 一定會在這個範圍內
	if (args[1] != "NULL") // 假設存為一字串（包含兩日期）
	{
		start_date = args[1].substr(0, 8); 
		end_date = args[1].substr(8, 8);
	}
	// expression  
	int first_index = 0;
	string word;
	unordered_set<string> current_set, difference_set, next_set, universal_set, id_set; // note: int
	stack< unordered_set<string> > id_stack;

	for (int i = 1; i<= 10000; i++) // 差集
		difference_set.insert(to_string(i));

	for (int i = 0; i < args[3].size() - 1; i++)
	{
		// find a variable: 該字元不是運算符號，且下一個字元是運算符號（確定有i+1）
		if (args[3][i] != '!' and args[3][i] != '&' and args[3][i] != '|' and (args[3][i+1] == '!' or args[3][i+1] == '&' or args[3][i+1] == '|'))
		{
			word = args[3].substr(first_index, i - first_index + 1);
			current_set = word_dict[word]; // id + date
			for (unordered_set<string>::iterator element = current_set.begin(); element != current_set.end(); element++)
			{
				if ((*element).substr(5, 8) >= start_date and (*element).substr(5, 8) <= end_date) // 檢查是否符合時間
					id_set.insert((*element).substr(0, 5))
			}
			id_stack.push(id_set);
			id_set.clear(); // 清空
		}
		else if (args[3][i] == '!')
		{
			current_set = id_stack.top(); // only id
			id_stack.pop();

			for (unordered_set<int>::iterator element = current_set.begin(); element != current_set.end(); element++) // pop these elements
			{
				difference_set.erase(stoi(*element));
			}
			id_stack.push(difference_set);
			for (unordered_set<int>::iterator element = current_set.begin(); element != current_set.end(); element++) // push these elements back
			{
				difference_set.insert(stoi(*element));
			}
			first_index = i + 1; // next variable
		}
		else if (args[3][i] == '&')
		{
			current_set = id_stack.top();
			id_stack.pop();
			next_set = id_stack.top();
			id_stack.pop();
			for (unordered_set<int>::iterator element = current_set.begin(); element != current_set.end(); element++) // insert these elements
			{
				if (next_set.find(stoi(*element)) != next_set.end()) // not a common element
					universal_set.insert(*element);
			}
			id_stack.push(universal_set);
			universal_set.clear(); // 清空
		}
		else if (args[3][i] == '|')
		{
			current_set = id_stack.top();
			id_stack.pop();
			next_set = id_stack.top();
			id_stack.pop();
			for (unordered_set<int>::iterator element = current_set.begin(); element != current_set.end(); element++) // insert these elements
				next_set.insert(*element);
			id_stack.push(next_set);
		}
	}

	// from
	unordered_set<int> from_id;
	if (from_dict.find(args[0]) != from_dict.end()) // sender exists
	{
		for (unordered_set<string>::iterator element = from_dict[args[0]].begin(); element != from_dict[args[0]].end(); element++)
		{
			if ((*element).substr(5, 8) >= start_date and (*element).substr(5, 8) <= end_date)
				from_id.insert((*element).substr(0, 5));
		}
	}

	current_set = id_stack.top(); // only id
	id_stack.pop()
	for (unordered_set<int> element = current_set.begin(); element != current_set.end(); element++)
	{
		if (from_id.find(*element) != from_id.end())
			universal_set.insert(*element);
	}
	id_stack.push(universal_set);
	universal_set.clear();

	// to
	unordered_set<int> to_id;
	if (to_dict.find(args[1]) != to_dict.end()) // receiver exists
	{
		for (unordered_set<string>::iterator element = to_dict[args[1]].begin(); element != to_dict[args[1]].end(); element++)
		{
			if ((*element).substr(5, 8) >= start_date and (*element).substr(5, 8) <= end_date)
				to_id.insert((*element).substr(0, 5));
		}
	}

	current_set = id_stack.top(); // only id
	id_stack.pop()
	for (unordered_set<int> element = current_set.begin(); element != current_set.end(); element++)
	{
		if (to_id.find(*element) != to_id.end())
			universal_set.insert(*element);
	}
	return universal_set.size();
}

















