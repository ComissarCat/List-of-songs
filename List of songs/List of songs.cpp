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

void menu(path path_to_songs);
void add_song_by_hand(path path_to_songs, vector <song>& list_of_songs, int& length);
void add_song_by_file(vector <song>& list_of_songs, int& length);
void delete_song();
void edit_song();
void search_by_author();
void search_by_word();
void show_song(vector <song>& list_of_songs, int number);
void save_song(path path_to_songs, vector <song>& list_of_songs, int number);
void load_list_of_songs(path path_to_songs, vector <song>& list_of_songs, int& length);
void overwrite_save_file(path path_to_songs, vector <song>& list_of_songs, int& length);

int main()
{
    setlocale(LC_ALL, "Russian");

    int length = 0;
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
    else load_list_of_songs(path_to_songs, list_of_songs, length);
    //show_song(list_of_songs, 0);    
    //show_song(list_of_songs, 1);    
    add_song_by_file(list_of_songs, length);
    //show_song(list_of_songs, 2);
    overwrite_save_file(path_to_songs, list_of_songs, length);
    
    return 0;
}

void menu(path path_to_songs)
{
    
}

void add_song_by_hand(path path_to_songs, vector <song>& list_of_songs, int& length)
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
    list_of_songs[length].name = name;
    list_of_songs[length].author = author;
    list_of_songs[length].year = year;
    list_of_songs[length].text = text;
    save_song(path_to_songs, list_of_songs, length);
    length++;
}

void show_song(vector <song>& list_of_songs, int number)
{
    cout << "Название: " << list_of_songs[number].name << endl;
    cout << "Автор: " << list_of_songs[number].author << endl;
    if (list_of_songs[number].year != NULL) cout << "Год создания: " << list_of_songs[number].year;
    cout << endl;
    vector<string>::iterator it;
    int i = 1;
    for (it = list_of_songs[number].text.begin(); it != list_of_songs[number].text.end(); it++, i++)
    {
        cout << i << "\t" << *it << endl;
    }
    cout << endl;
}

void save_song(path path_to_songs, vector <song>& list_of_songs, int number)
{
    fstream file;
    file.open(path_to_songs, ios::app);
    file << "\n";
    file << list_of_songs[number].name << endl;
    file << list_of_songs[number].author << endl;
    file << list_of_songs[number].year << endl;
    vector<string>::iterator it;    
    for (it = list_of_songs[number].text.begin(); it != list_of_songs[number].text.end(); it++)
    {
        file << *it << endl;
    }    
    file.close();
}

void load_list_of_songs(path path_to_songs, vector <song>& list_of_songs, int& length)
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
        list_of_songs[length].name = name;
        list_of_songs[length].author = author;
        list_of_songs[length].year = year;
        list_of_songs[length].text = text;
        length++;
    }
}

void overwrite_save_file(path path_to_songs, vector <song>& list_of_songs, int& length)
{
    ofstream file;
    file.open(path_to_songs);
    file.close();
    length = 0;
    vector<song>::iterator song_it;
    for (song_it = list_of_songs.begin(); song_it != list_of_songs.end(); song_it++, length++)
    {
        save_song(path_to_songs, list_of_songs, length);
    }
}

void add_song_by_file(vector <song>& list_of_songs, int& length)
{
    path path_to_song = current_path() += "\\Songs";
    string bufer;
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
        cout << path_to_song << endl;
        if (not exists(path_to_song)) cout << "Файл не найден, проверьте правильность ввода\n\n";
    } while (not exists(path_to_song));
    if (bufer != "cancel\0") load_list_of_songs(path_to_song, list_of_songs, length);
}