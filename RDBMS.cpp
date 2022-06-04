#include <bits/stdc++.h>
using namespace std;

fstream schema;

bool isTable(string table_name)
{
    schema.open("Schema.txt", ios::in);
    string line;
    bool flag = false;

    while(getline(schema, line))
    {
        if(line.substr(0, line.find('#')) == table_name)
        {
            flag = true;
            break;
        }
    }

    schema.close();
    return flag;
}

void create(vector<string> query)
{

    string table_name = query[2];
    if(isTable(table_name))
    {
        cout << "Table Already Exists!" << endl;
        return;
    }

    schema.open("Schema.txt", ios::app);

    int start = -1, end = -1;
    for (int i = 3; i < query.size(); i++)
    {
        if (query[i] == "(")
        {
            start = i;
        }
        if (query[i] == ")")
        {
            end = i;
        }
    }

    if (start == -1 || end == -1)
    {
        cout << "Syntax Error (Round Bracket Missing)!! Please Recheck" << endl;
        return;
    }

    schema << table_name;

    while (start < end - 1)
    {
        start++;
        if (query[start] != ",")
        {
            schema << "#" << query[start];
        }
    }
    schema << endl;
    cout << "Table created successfully!" << endl;

    schema.close();
}

void drop(vector<string> query)
{
    string table_name = query[2];
    if(!isTable(table_name))
    {
        cout<<"Table" << table_name << "doesn't exist"<<endl;
        return;
    }

    schema.open("Schema.txt", ios::in);

    fstream temp;
    temp.open("Temp.txt", ios::out);

    string line;

    while (getline(schema, line))
    {
        string l = line.substr(0, line.find('#'));
        if (table_name != l)
        {
            temp << line << endl;
        }
    }

    schema.close();
    temp.close();

    remove("Schema.txt");
    rename("Temp.txt", "Schema.txt");

    string f = table_name + ".txt";
    char fname[f.length()];

    for(int i = 0; i < f.length(); i++)
    {
        fname[i] = f[i];
    }

    fname[f.length()]='\0';

    remove(fname);

    cout << "Table dropped successfully" << endl;
}

void describe(vector<string> query)
{
    schema.open("Schema.txt", ios::in);

    if (query.size() > 1)
    {
        string table_name = query[1];
        string line;

        while (getline(schema, line))
        {
            string l1 = line.substr(0, line.find('#'));

            if (table_name == l1)
            {
                string l2 = line.substr(line.find('#') + 1, line.size());
                int f = 1;

                for (auto i : l2)
                {
                    if (i == '#')
                    {
                        if (f % 2 != 0)
                        {
                            cout << " - ";
                        }

                        else
                        {
                            cout << endl;
                        }
                        f++;
                    }

                    else
                    {
                        cout << i;
                    }
                }
            }
        }
    }

    else
    {
        cout << "Please mention the table name" << endl;
    }

    schema.close();
}

void fetchSchema(string tableName, vector<string> &schemafile);

bool isNumber(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

void getDatatype(string tableName, vector<string>& data)
{
    vector<string> schema;
    fetchSchema(tableName,schema);
    for(int i = 2; i < schema.size(); i+=2)
    {
        data.push_back(schema[i]);
    }
}

void insert(vector<string> query)
{
    string table_name = query[2];
    if(!isTable(table_name))
    {
        cout<<"Table does not Exist"<<endl;
        return;
    }

    fstream table;
    table.open(table_name + ".txt", ios::app);

    vector<string> data;
    getDatatype(table_name, data);

    int start = -1, end = -1;
    for (int i = 4; i < query.size(); i++)
    {
        if (query[i] == "(")
        {
            start = i;
        }

        if (query[i] == ")")
        {
            end = i;
        }
    }

    if (start == -1 || end == -1)
    {
        cout << "Syntax Error (Round Bracket Missing)!! Please Recheck" << endl;
        return;
    }

    int k=0;
    bool invalidType=false;
    int st=start;

    while (st < end - 1)
    {
        st++;
        if (query[st] != ",")
        {
            if( ( data[k]=="INT" && !isNumber(query[st]) ) || ( data[k]!="INT" && isNumber(query[st]) ))
            {
                cout << "Data Type of " << query[st] << " is invalid." << endl;
                return;
            }
            k++;
        }
    }

    while (start < end - 1)
    {
        start++;
        if (query[start] != ",")
        {
            table << query[start];
            if (start != end - 1)
            {
                table << "#";
            }
            else
            {
                table << "\n";
            }
        }
    }

    cout << "Record added successfully!" << endl;
}


void deletet(vector<string> query)
{
    vector<string> schema;
    if(!isTable(query[2]))
    {
        cout << "Table does not exist!" << endl;
        return;
    }

    fetchSchema(query[2], schema);

    if (!schema.empty())
    {
        if (query.size() == 3)
        {
            string table = query[2] + ".txt";
            char t[table.size() + 1];
            strcpy(t, table.c_str());
            remove(t);
            cout << "All the contents of table deleted!" << endl;
        }

        else if (query[3] == "WHERE")
        {
            string table_name = query[2];
            fstream table;
            table.open(table_name + ".txt", ios::in);
            string line;

            int i, j;
            j = 0;
            int flag = 0;

            vector<string> lineVec;

            fstream temp;
            temp.open("temp.txt", ios::out);

            while (getline(table, line))
            {
                stringstream ss(line);
                while (ss.good())
                {
                    string substr;
                    getline(ss, substr, '#');
                    lineVec.push_back(substr);
                }

                for (i = 1; i < schema.size(); i += 2)
                {
                    if (query[4] == schema[i])
                    {
                        if (query[5] == "=")
                        {
                            if (query[6] == lineVec[j])
                            {
                                flag = 1;
                            }
                        }
                        if (query[5] == ">")
                        {
                            if (lineVec[j] < query[6])
                            {
                                flag = 1;
                            }
                        }
                        if (query[5] == "<")
                        {
                            if (lineVec[j] < query[6] )
                            {
                                flag = 1;
                            }
                        }
                    }

                    j++;
                }

                if (flag != 1)
                {
                    temp << line << endl;
                }

                flag = 0;
            }

            table.close();
            temp.close();

            string table1 = table_name + ".txt";
            char t[table1.size() + 1];
            strcpy(t, table1.c_str());
            remove(t);

            rename("temp.txt", t);
            cout << "Content deleted" << endl;
        }
    }
}

void tableno(map<string, int>& table, vector<string> schema)
{
    int count = 0;
    for(int j = 1; j < schema.size(); j+=2)
    {
        table[schema[j]] = count;
        count++;
    }
}

void checkcmd(map<int, string>& column_change, vector<string> query, vector<string> schema)
{
    int i;
    map<string, int> table;
    tableno(table, schema);
    for(i=3; i<query.size() && query[i] != "WHERE"; i+=3)
    {
        int col = table[query[i]];
        column_change[col] = query[i+2];
    }
}


void update(vector<string> query)
{

    if(!isTable(query[1])){
        cout<<"Table does not exist"<<endl;
        return;
    }

    vector<string> schema;
    fetchSchema(query[1], schema);

    if(!schema.empty()){
        string table_name = query[1] + ".txt";
        fstream table;
        const char *c = table_name.c_str();
        table.open(c, ios::in);
        vector<string>::iterator it;
        it = find(query.begin(), query.end(), "WHERE");
        map<int, string> column_change;
        checkcmd(column_change, query, schema);

        string line;
        vector<string> lineVec;
        int i, j=0;
        int flag = 0;
        int count = 0;
        fstream temp;
        bool changed=false;
        temp.open("temp.txt", ios::out);

        if(it == query.end())
        {
            while(getline(table, line))
            {
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length())
                {
                    if(tab[i] == '#')
                    {
                        lineVec.push_back(curr);
                        curr = "";
                    }
                    else
                    {
                        curr += tab[i];
                    }

                    i++;
                }

                lineVec.push_back(curr);

                string new_line = "";
                map<int, string>::iterator itr;
                for(itr = column_change.begin(); itr != column_change.end(); itr++)
                {
                    lineVec[itr->first] = itr->second;
                }

                for(i = 0; i < lineVec.size()-1; i++)
                {
                    new_line += lineVec[i];
                    new_line += "#";
                }

                new_line += lineVec[i];
                temp << new_line << endl;
                lineVec.clear();
            }
        }

        else
        {
            while (getline(table, line))
            {
                string tab = line;
                i=0;
                string curr = "";
                while(i<tab.length())
                {
                    if(tab[i] == '#')
                    {
                        lineVec.push_back(curr);
                        curr = "";
                    }
                    else
                    {
                        curr += tab[i];
                    }
                    i++;
                }

                lineVec.push_back(curr);

                int idx = 0;
                idx = it - query.begin();
                j=0;
                for(i=1; i<schema.size(); i+=2)
                    {
                    if(query[idx+1] == schema[i])
                    {

                        if(query[idx+2] == "=")
                        {
                            if(lineVec[j] == query[idx+3])
                            {
                                flag = 1;
                                count++;
                            }
                        }

                        if(query[idx+2] == ">")
                        {
                            if(lineVec[j] > query[idx+3])
                            {
                                flag = 1;
                                count++;
                            }
                        }

                        if(query[idx+2] == "<")
                        {
                            if(lineVec[j] < query[idx+3])
                            {
                                flag = 1;
                                count++;
                            }
                        }
                    }
                    j++;
                }

                if(flag != 1)
                {
                    temp << line << endl;
                    lineVec.clear();
                }

                else{
                    changed=true;
                    string new_line = "";
                    map<int, string>::iterator itr;
                    for(itr=column_change.begin(); itr!=column_change.end(); itr++){
                        lineVec[itr->first] = itr->second;
                    }

                    for(i = 0; i < lineVec.size()-1; i++)
                    {
                        new_line += lineVec[i];
                        new_line += "#";
                    }
                    new_line += lineVec[i];
                    temp << new_line << endl;
                    lineVec.clear();
                }

                flag = 0;
            }
        }

        table.close();
        temp.close();
        remove(c);
        rename("temp.txt", c);
        if(changed)
        {
            cout << "Content updated" << endl;
        }
        else
        {
            cout << "Content not found!!" <<endl;
        }
    }
}

void fetchTable(string& tableName,vector<string>& att, unordered_map<string,int>& mp, vector<vector<string>>& d)
{
    fstream table(tableName+".txt",ios::in);
    string line;

    cout << "\n";
    for(auto x:att)
        cout << x << "\t";
    cout << "\n\n";

    while(getline(table,line))
    {
        stringstream ss(line);
        vector<string> data;

        while (ss.good())
        {
            string substr;
            getline(ss, substr, '#');
            data.push_back(substr);
        }

        d.push_back(data);

    }

}

void select(vector<string> query)
{

    vector<string> attr_list;

    auto it = find(query.begin(), query.end(),"FROM");
    int index = 0;

    if(it != query.end())
    {
        index = it - query.begin();
    }
    else
    {
        cout << "Syntax error" << endl;
    }

     int tablename_pos = index + 1;

    if(!isTable(query[tablename_pos]))
    {
        cout << "Table does not exist" << endl;
        return;
    }

    //string all = query[2];

    if(query[1] == "*")
    {
        schema.open("Schema.txt", ios::in);
        string line;
        int flag=0;
        while(getline(schema, line))
        {
            if(line.substr(0, line.find('#')) == query[tablename_pos])
            {
                for(int i=0;i<line.size();i++)
                {
                    if(line[i]=='#')
                    {
                        flag++;
                        cout<<" ";
                        continue;
                    }
                    if(flag%2==0)
                    {
                        continue;
                    }
                    cout<<line[i];

                }
                cout<<"\n";
            }
        }

        schema.close();
        ifstream myfile (query[tablename_pos]+".txt");
        if (myfile.is_open())
        {
            while (! myfile.eof() )
            {
                cout<<" ";
                getline (myfile,line);
                for(int i=0;i<line.size();i++)
                {
                    if(line[i]=='#')
                    {
                        cout<<" ";
                        continue;
                    }
                    cout<<line[i];
                }
                cout<<"\n";
            }
        }
          else
            cout << "Unable to open file";
    }

    else
    {
        for(int i = 1; i < index; i+=2)
            attr_list.push_back(query[i]);
    }



    unordered_map<string,int> table_attr;
    vector<string> schema;

    fetchSchema(query[tablename_pos],schema);

    int k=0;
    for(int i = 1; i < schema.size(); i+=2)
    {
        table_attr[schema[i]] = k++;
    }

    bool atterror=false;
    bool attError=false;
    for(auto x:attr_list){
        if(table_attr.find(x) == table_attr.end())
        {
            //cout << "Attribute Error" << endl;
            attError = true;
            break;
        }
    }

    if(attError)
    {
        cout << "Incorrect attribute entered!" << endl;
        return;
    }

    vector<vector<string>> data;
    fetchTable(query[tablename_pos],attr_list,table_attr,data);

    auto itw = find(query.begin(), query.end(),"WHERE");
    int whereidx = 0;
    if (itw != query.end())
    {
        whereidx = itw - query.begin();
        string cnd_att = query[whereidx+1];
        string sign = query[whereidx+2];
        string atr_value = query[whereidx+3];

        int att_ind = table_attr[cnd_att];

        for(int k=0; k < data.size(); k++){
            if((sign == "=" && data[k][att_ind] == atr_value) || (sign == ">" && data[k][att_ind] > atr_value)|| (sign == "<" && data[k][att_ind] < atr_value)
               || (sign == "!=" && data[k][att_ind] != atr_value))
            {
                for(int i = 0; i < attr_list.size(); i++)
                {
                    cout << data[k][table_attr[attr_list[i]]] << "\t";
                }
                cout << endl;
            }
        }

    }

    else
    {
        for(int k = 0; k < data.size(); k++)
        {
            for(int i = 0; i < attr_list.size(); i++)
            {
                cout << data[k][table_attr[attr_list[i]]] << "\t";
            }
            cout << endl;
        }
    }
}

void help_tables()
{
    string line;

    schema.open("Schema.txt");

    int count = 0;
    while (getline(schema, line))
    {
        string l = line.substr(0, line.find('#'));
        cout << l << endl;
        count++;
    }

    if (!count)
    {
        cout << "No tables found" << endl;
    }

    schema.close();
}

void help_cmd(vector<string> query)
{
    cout << "\n------------HELP----------------" << endl;
    map<string, int> help{{"CREATETABLE", 1}, {"DROPTABLE", 2}, {"DESCRIBE", 3}, {"INSERT", 4}, {"DELETE", 5}, {"UPDATE", 6}, {"SELECT", 7}, {"QUIT", 8}};
    map<string, int>::iterator i;
    string command = query[1] + (query.size() > 2 ? query[2] : "");
    i = help.find(command);

    switch (i->second)
    {
    case 1:

        cout << "\nCREATE TABLE: Creates a new table" << endl;
        cout << "\nCommand Structure: \n\tCREATE TABLE table_name (attr_name1 attr_type1 , attr_name2 attr_type2, ...)" << endl;
        break;

    case 2:
        cout << "\nDROP TABLE: Deletes a table" << endl;
        cout << "\nCommand Structure: \n\tDROP TABLE table_name;" << endl;
        break;

    case 3:
        cout << "\nDESCRIBE T_NAME" << endl;
        cout << "\nCommand Structure: \n\tDescribes the schema of table T_NAME." << endl;
        break;

    case 4:
        cout << "\nINSERT: Inserts new data into a database" << endl;
        cout << "\nCommand Structure: \n\tINSERT INTO table_name VALUES(attr_value1, attr_value2, ...); \n\t*Note that NULL is not permitted for any attribute" << endl;
        break;

    case 5:
        cout << "\nDELETE: Deletes data from a database" << endl;
        cout << "\nCommand Structure: \n\tDELETE FROM table_name WHERE condition_list;" << endl;
        break;

    case 6:
        cout << "\nUPDATE: Updates data in a database" << endl;
        cout << "\nCommand Structure: \n\tUPDATE table_name SET attr1_name = attr1_value WHERE condition_list;" << endl;
        break;

    case 7:
        cout << "\nSELECT: Extracts data from a database" << endl;
        cout << "\nCommand Structure: \n\tSELECT attr_list FROM table_list WHERE condition_list;" << endl;
        break;

    case 8:
        cout << "\nQUIT: Extracts data from a database" << endl;
        cout << "\nCommand Structure: \n\tQ" << endl;
        break;

    default:
        cout << "Syntax Error";
    }
}

void convert(string input, vector<string> &v)
{
    string s = "";
    for (auto x : input)
    {
        if (x == ' ')
        {
            v.push_back(s);
            s = "";
        }
        else
        {
            s = s + x;
        }
    }
    v.push_back(s);
}

void fetchSchema(string tableName, vector<string> &schemafile){
    schema.open("Schema.txt", ios::in);
    string line;
    int flag = 0;

    while (getline(schema, line))
    {
        string l1 = line.substr(0, line.find('#'));
        if (tableName == l1)
        {
            flag = 1;
            stringstream ss(line);

            while (ss.good())
            {
                string substr;
                getline(ss, substr, '#');
                schemafile.push_back(substr);
            }
            break;
        }
    }
    if (flag == 0){
        cout << "Table not found" << endl;
    }

    schema.close();
}

void cmd(vector<string> query){
    if (query[0] == "CREATE" && query[1] == "TABLE")
    {
        create(query);
    }
    else if (query[0] == "DROP" && query[1] == "TABLE")
    {
        drop(query);
    }
    else if (query[0] == "INSERT" && query[1] == "INTO")
    {
        insert(query);
    }
    else if (query[0] == "DESCRIBE")
    {
        describe(query);
    }
    else if (query[0] == "DELETE" && query[1] == "FROM")
    {
        deletet(query);
    }
    else if (query[0] == "UPDATE")
    {
        update(query);
    }
    else if (query[0] == "SELECT")
    {
        select(query);
    }
    else if (query[0] == "HELP" && query[1] == "TABLE")
    {
        help_tables();
    }
    else if (query[0] == "HELP" && query[1] != "TABLE")
    {
        help_cmd(query);
    }
    else
    {
        cout << "Syntax Error";
    }
}

void quit(vector<string> query, string input)
{
    while (input != "Q")
    {

        convert(input, query);
        cmd(query);

        query.clear();
        cout << "\n ";


        getline(cin, input);
    }
}

int main()
{
    cout<<"\n                           Group 49        \n";
    cout<<"-------------------------------------------------------------------------";
    vector<string> query;
    string input;

    cout << "\n ";

    getline(cin, input);

    quit(query, input);

    return 0;

}
