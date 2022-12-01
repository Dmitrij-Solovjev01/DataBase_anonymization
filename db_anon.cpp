#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;

vector <string> name;
int asdasd=0;

string Read_CSV(int RowNumber, fstream* file, int index) {
    string row, line, word;

    if (file->is_open()) {
        uint32_t count = 0;
        file->clear();
        file->seekg(0);

        while (count+1 < RowNumber and getline(*file, line)) {
            count++;
        }

        getline(*file, line);



        stringstream str(line);
        int count2=0;

        while (getline(str, word, ',')) {
            if ((index == count2 or index == -1 or index == -2) and !word.empty()) {
                if (not(count2 == 0 and( index == -1 or index == -2))){
                    if (index == -2){
                        row += word + ",";
                    } else
                        row += word;
                }
            }

            count2++;
        }
    } else cout<<"Could not open the file\n";
    return row;
}

void Hash_name(int row, fstream *file, ofstream *fileOUT) {
    name.push_back(Read_CSV(row, file, 0));
//    return to_string(std::hash<std::string>{}(Read_CSV(0, file, 0)));
    *fileOUT <<  to_string(asdasd++);
}

void depers_IP(int row, fstream *file, ofstream *fileOUT) {
    stringstream str(Read_CSV(row, file, 1));
    int count2=0;
    string ip="";
    string word;
    while (getline(str, word, '.')) {
        if (!word.empty() and count2 < 1) ip+=word+".";
        count2++;
    }
    ip+="X.X.X";
//    cout << ip << endl;
    *fileOUT << ""; // ip;
}



void depers_URL(int row, fstream *file, ofstream *fileOUT) {
    fstream URL("../URL2.csv", ios::in);

    string strURL = Read_CSV(row, file, 2);
    int i;
    for (i=0; i < 50 and Read_CSV(i, &URL, 1) != strURL; ++i);
    //cout << i << " " << Read_CSV(i, URL, 1) << " " <<  Read_CSV(i, URL, 3) << endl;
    *fileOUT <<  Read_CSV(i, &URL, 3);
}

void depers_DATA(int row, fstream *file, ofstream *fileOUT) {
    stringstream str(Read_CSV(row, file, 3));
    int count2=0;
    string date="";
    string word;
    while (getline(str, word, '/')) {
        if (!word.empty() and count2 == 0) {
            date = "1";
            //date=to_string((31+atoi( word.c_str())+10-rand()%5)%28+1);
        } else {
            date+="." +word;
        }
        count2++;
    }
    //cout << date << endl;
    *fileOUT <<  date;
}

void depers_NUM (int row, fstream *file, ofstream *fileOUT) {
    *fileOUT <<  to_string((atoi(Read_CSV(row, file, 4).c_str())/25+1)*25);
}

void depers_TIME (int row, fstream *file, ofstream *fileOUT) {
    string A[]={"мало", /*"средне",*/ "много" /*, "очень много" */};

    stringstream str(Read_CSV(row, file, 5));
    int count2=0;
    int time;
    string word;
    while (getline(str, word, ':')) {
        if (!word.empty() and count2 == 0) {
            time = atoi( word.c_str())*60;
        } else {
            time+= atoi( word.c_str());
        }
        count2++;
    }

    *fileOUT << A[time/3000/2];
}

string RM_parenthesis(string asd){
    for (int i=0; i<asd.size(); ++i){
        if (asd[i] == '('){
            int j;
            for (j=i+1; j<asd.size(); ++j){
                if (asd[j] == ')') break;
            }
            asd.erase(i,j-i+1);
            i--;
        }
    }
    return asd;
}

void depers_TOVAR (int row, fstream *file, ofstream *fileOUT) {
    stringstream str(RM_parenthesis(Read_CSV(row, file, 6)));

    string answer = "";
    string word;
    while (getline(str, word, ' ')) {
        if (!word.empty()) {
            answer = word;
        }
    }

    *fileOUT <<  answer;
}

void streamcopy(string from, string to) {
    fstream InputDB(from, ios::in);
    ofstream OutputDB(to);

    int counter=0;
    string row;
    while(getline(InputDB, row)) counter++;

    for(int i=0; i<counter; ++i) {
        string stroke = Read_CSV(i, &InputDB, -2);
        OutputDB << stroke << "\n";
    }

    InputDB.close();
    OutputDB.close();
}

void streamcopyanddelete(string from, string to, int threshold = 5) {
    map<string, int> check;
    fstream InputDB(from, ios::in);
    ofstream OutputDB(to);

    int counter=0;

    string row;

    while (getline(InputDB, row)) counter++;

    for (int i=0; i<=counter; ++i) {
        string stroke = Read_CSV(i, &InputDB, -2);
        //cout << stroke << endl;

        if (check.find(stroke)!=check.end()) {
            check[stroke] += 1;
        } else {
            check[stroke] =  1;
        }
    }

    for (auto it = check.begin(); it != check.end(); ++it){
        if (it->second > threshold){
            OutputDB <<it->first<<endl;
        }
    }

    InputDB.close();
    OutputDB.close();
}


int main() {
    fstream InputDB("../BD.csv", ios::in);                              //открываем потоки
    map<string, int> check;                                                      //словарь для проверки, чтобы одному человеку не попаласть та же реклама

    cout << "Ведите функцию ( 1 - анонимизировать, 2 - вывести k-анонимити)" << endl;

    uint8_t a = 0;  cin >> a;
    bool b = a % 2;
    if (b == 0) {
        cout << "Вы выбрали: 2 - вывести k-анонимити"<< endl;
        cout << "Выберите значение отсечения(вывести все элементы, k-анонимити которых < threshold"<< endl;

        fstream OutputDB("../OutputDB.csv", ios::in);                              //открываем потоки

        int threshold;
        int MIN = 100000;
        int counter_of_unicle =0;
        int counter_of_all = 0;
        int counter_of_bad = 0;
        int counter = 0;
        string row;

        cin >> threshold;

        while(OutputDB >> row) counter++;

        //counter = 5000;

        OutputDB.clear();
        OutputDB.seekg(0, ios::beg);

        for (int i=0; i<=counter; ++i) {
            string column = Read_CSV(i, &OutputDB, -1);
            counter_of_all++;

            if (check.find(column)!=check.end()){
                check[column] += 1;
            } else {
                check[column] =  1;
            }
        }


        for (auto it = check.begin(); it != check.end(); ++it){
            if (it->second < threshold){
                cout <<it->first<<endl;
                counter_of_bad++;
            }
            if (it->second < MIN) {
                MIN = it->second;
            }
            counter_of_unicle++;
        }

        cout << "k-анонимити = " << MIN << endl << "Количество уникальных элементов в БД = " << counter_of_unicle << endl;
        cout << "количество \"плохих записей = " << counter_of_bad << endl << "Их процент " << 100 * counter_of_bad / counter << endl;

    } else {
        cout << "Вы выбрали: 1 - анонимизировать"<< endl;
        cout << "Введите количество квази-идентификаторов, по которым вы хотите провести анонимизацию(счет от 1 до 7)"<< endl;
        int n;
        cin >> n;
        if (n < 1 or n >7 ) { cout << "Больше так не делайте"<< endl; return 0; }
        cout << "Введите номера стоблцов квази-идентификаторов, по которым вы хотите провести анонимизацию(счет от 1 до 7)"<< endl;
        vector<int> rows(n);
        for (int i=0; i<n; ++i){
            cin>>rows[i];
            if (rows[i] < 1 or rows[i] > 7) { cout << "Больше так не делайте"<< endl; return 0; }
            rows[i]--;
        }
        cout << "Использовать отсечение? (0 - нет, >0 - да)" << endl;
        uint8_t k;
        cin >> k;
        ofstream OutputDB("../OutputDB.csv");                              //открываем потоки

        int counter=0;
        string row;
        while(getline(InputDB, row)) counter++;

        //counter = 5000;

        InputDB.clear();
        InputDB.seekg(0, ios::beg);

        cout << endl << counter << endl;

        for (int i=0; i<counter; ++i) {
            void (*(functions[7]))(int A, fstream *B, ofstream *C) = {Hash_name, depers_IP, depers_URL, depers_DATA, depers_NUM, depers_TIME, depers_TOVAR};

            for (int j=0; j<n; ++j) {
                functions[rows[j]](i, &InputDB, &OutputDB);
                OutputDB << ",";
            }

            OutputDB << "\n";
        }

        OutputDB.close();

        if (k != 0)
            streamcopyanddelete("../OutputDB.csv", "../OutputDB2.csv", k);
        else
            streamcopy("../OutputDB.csv", "../OutputDB2.csv");
    }

    InputDB.close();

    return 0;
}