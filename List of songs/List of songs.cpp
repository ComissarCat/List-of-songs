#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <algorithm>

using namespace std;
using namespace experimental::filesystem;

struct song
{
    string name;
    string author;
    int year;
    vector <string> text;
};

void menu(path path_to_songs, vector <song>& list_of_songs);

void add_song_by_hand(path path_to_songs, vector <song>& list_of_songs);
void add_song_by_file(path path_to_songs, vector <song>& list_of_songs);

void delete_song(path path_to_songs, vector <song>& list_of_songs, vector<song>::iterator it);
void edit_song(path path_to_songs, vector <song>& list_of_songs, vector<song>::iterator it);

void search_by_author();
void search_by_word();
void search_by_name(path path_to_songs, vector <song>& list_of_songs);

void show_song(vector<song>::iterator it);
void show_all_songs(vector <song>& list_of_songs);

void save_song(path path_to_songs, vector<song>::iterator song_it);
void load_list_of_songs(path path_to_songs, vector <song>& list_of_songs);
void overwrite_save_file(path path_to_songs, vector <song>& list_of_songs);

int main()
{
    setlocale(LC_ALL, "Russian");
    
    vector <song> list_of_songs;
    path path_to_songs = current_path() += "\\Songs";
    if (not exists(path_to_songs)) create_directory(path_to_songs);
    path_to_songs += "\\Songs.txt";
    if (not exists(path_to_songs))
    {
        ofstream file;
        file.open(path_to_songs);
        file.close();
    }
    else load_list_of_songs(path_to_songs, list_of_songs);    
    
    return 0;
}

void menu(path path_to_songs, vector <song>& list_of_songs)
{
    
}

void add_song_by_hand(path path_to_songs, vector <song>& list_of_songs)
{
    string name, bufer, author;
    int year = NULL;
    vector <string> text;
    cout << "Введите название песни: ";
    getline(cin, name);
    cout << "Введите автора песни: ";
    getline(cin, author);
    cout << "Введите год создания песни (если известен): ";
    getline(cin, bufer);
    if (bufer.length()) year = (int)atof(bufer.c_str());
    cout << "Введите текст песни (для завершения дважды нажмите enter):\n\n";
    do
    {
        getline(cin, bufer);
        if (bufer != "\0") text.push_back(bufer);
    } while (bufer != "\0");
    list_of_songs.push_back(song());
    vector<song>::iterator it = list_of_songs.end() - 1;
    it->name = name;
    it->author = author;
    it->year = year;
    it->text = text;
    save_song(path_to_songs, it);
}

void show_song(vector<song>::iterator song_it)
{
    cout << "Название: " << song_it->name << endl;
    cout << "Автор: " << song_it->author << endl;
    if (song_it->year != NULL) cout << "Год создания: " << song_it->year;
    cout << endl;
    vector<string>::iterator text_it;
    int i = 1;
    for (text_it = song_it->text.begin(); text_it != song_it->text.end(); text_it++, i++)
    {
        cout << i << "\t" << *text_it << endl;
    }
    cout << endl;
}

void save_song(path path_to_songs, vector<song>::iterator song_it)
{
    fstream file;
    file.open(path_to_songs, ios::app);
    file << "\n";
    file << song_it->name << endl;
    file << song_it->author << endl;
    file << song_it->year << endl;
    vector<string>::iterator text_it;
    for (text_it = song_it->text.begin(); text_it != song_it->text.end(); text_it++)
    {
        file << *text_it << endl;
    }    
    file.close();
}

void load_list_of_songs(path path_to_songs, vector <song>& list_of_songs)
{
    ifstream file;
    file.open(path_to_songs);
    while (not file.eof())
    {
        string name, bufer, author;
        int year = NULL;
        vector <string> text;
        getline(file, name);
        if (name == "\0") getline(file, name);
        getline(file, author);
        getline(file, bufer);
        if (bufer != "0\0") year = (int)atof(bufer.c_str());
        do
        {
            getline(file, bufer);
            if (bufer != "\0") text.push_back(bufer);
        } while (bufer != "\0");        
        list_of_songs.push_back(song());
        vector<song>::iterator it = list_of_songs.end() - 1;
        it->name = name;
        it->author = author;
        it->year = year;
        it->text = text;        
    }
}

void overwrite_save_file(path path_to_songs, vector <song>& list_of_songs)
{
    ofstream file;
    file.open(path_to_songs);
    file.close();    
    vector<song>::iterator it;
    for (it = list_of_songs.begin(); it != list_of_songs.end(); it++)
    {
        save_song(path_to_songs, it);
    }
}

void add_song_by_file(path path_to_songs, vector <song>& list_of_songs)
{
    path path_to_song = current_path() += "\\Songs";
    string bufer;
    ifstream file;
    cout << "Положите файл с песней в папку Songs в папке с программой. Требования к файлу: расширение .txt, пустая первая и последняя строчка,\n";
    cout << "со второй строки название песни, затем автор, затем год создания песни (0, если неизвестен), затем текст песни без пустых строк.\n";
    cout << "В одном файле может быть несколько песен, разделённых пустой строкой.\n\n";
    do
    {
        path_to_song = current_path() += "\\Songs";
        cout << "Введите название файла (cancel для выхода):\n";
        getline(cin, bufer);
        if (bufer == "cancel\0") break;
        path_to_song += path::preferred_separator;
        path_to_song += bufer;        
        file.open(path_to_song);
        if (!file) cout << "Файл не найден, проверьте правильность ввода\n\n";
    } while (!file);
    if (bufer != "cancel\0") load_list_of_songs(path_to_song, list_of_songs);
    overwrite_save_file(path_to_songs, list_of_songs);
}

void search_by_name(path path_to_songs, vector <song>& list_of_songs)
{
    string name, target_name, command;
    bool found = false;
    cout << "Введите название: ";
    getline(cin, target_name);
    transform(target_name.begin(), target_name.end(), target_name.begin(), ::tolower);
    vector<song>::iterator it;
    for (it = list_of_songs.begin(); it != list_of_songs.end(); it++)
    {
        name = it->name;
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.find(target_name) != name.npos)
        {
            found = true;
            show_song(it);
            break;
        }
    }
    if (found)
    {
        do
        {
            cout << "Что следует сделать с записью? (edit, delete, cancel)\n";
            getline(cin, command);
        } while (command != "edit\0" and command != "delete\0" and command != "cancel\0");
        if (command == "delete\0") delete_song(path_to_songs, list_of_songs, it);
    }
    else cout << "Песня с таким названием не найдена\n\n";
}

void delete_song(path path_to_songs, vector <song>& list_of_songs, vector<song>::iterator it)
{
    list_of_songs.erase(it);
    overwrite_save_file(path_to_songs, list_of_songs);
}

void show_all_songs(vector <song>& list_of_songs)
{
    vector<song>::iterator it;
    for (it = list_of_songs.begin(); it != list_of_songs.end(); it++)
    {
        show_song(it);
    }
}

void edit_song(path path_to_songs, vector <song>& list_of_songs, vector<song>::iterator it)
{
    string command, bufer;    
    do
    {
        cout << "Что нужно изменить? (name, author, year, text, cancel)\n";
        getline(cin, command);
    } while (command != "name\0" and command != "author\0" and command != "year\0" and command != "text\0" and command != "cancel\0");
    if (command == "name\0")
    {
        cout << "Введите название песни: ";
        getline(cin, bufer);
        it->name = bufer;
        overwrite_save_file(path_to_songs, list_of_songs);
    }
    else if (command == "author\0")
    {
        cout << "Введите автора песни: ";
        getline(cin, bufer);
        it->author = bufer;
        overwrite_save_file(path_to_songs, list_of_songs);
    }
    else if (command == "year\0")
    {
        int year = NULL;
        cout << "Введите год создания песни: ";        
        getline(cin, bufer);
        if (bufer.length()) year = (int)atof(bufer.c_str());
        it->year = year;
        overwrite_save_file(path_to_songs, list_of_songs);
    }
    else if (command == "text\0")
    {
        do
        {
            cout << "Перезаписать текст полностью или одну строчку? (all, line, cancel)\n";
            getline(cin, command);
        } while (command != "all\0" and command != "line\0" and command != "cancel\0");
        if (command == "all\0")
        {        
            vector <string> text;
            cout << "Введите текст песни (для завершения дважды нажмите enter):\n\n";
            do
            {
                getline(cin, bufer);
                if (bufer != "\0") text.push_back(bufer);
            } while (bufer != "\0");
            it->text = text;
            overwrite_save_file(path_to_songs, list_of_songs);
        }
        else if (command == "line\0")
        {
            int number = 0;
            do
            {
                cout << "Какую строчку переписать? ";
                (cin >> number).get();
            } while (number > it->text.size() or number < 1);
            cout << "Введите новый текст строчки:\n";
            getline(cin, bufer);
            it->text[number - 1] = bufer;
            overwrite_save_file(path_to_songs, list_of_songs);
        }
    }
}