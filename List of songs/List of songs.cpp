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
    int year;
    vector <string> text;
};

void menu(path path_to_songs);
void add_song_by_hand(vector <song>& list_of_songs, int& length);
void delete_song();
void edit_song();
void search_by_author();
void search_by_word();
void show_song(vector <song>& list_of_songs, int number);

int main()
{
    setlocale(LC_ALL, "Russian");

    path path_to_songs = current_path() += "\\Songs";
    if (not exists(path_to_songs)) create_directory(path_to_songs);
    path_to_songs += "\\Songs.txt";
    if (not exists(path_to_songs))
    {
        ofstream file;
        file.open(path_to_songs);
        file.close();
    }
    vector <song> list_of_songs;
    int length = 0;
    add_song_by_hand(list_of_songs, length);
    show_song(list_of_songs, 0);
    
    return 0;
}

void menu(path path_to_songs)
{
    
}

void add_song_by_hand(vector <song>& list_of_songs, int& length)
{
    string name, bufer;
    int year = NULL;
    vector <string> text;
    cout << "Введите название песни: ";
    (cin >> name).get();
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
    list_of_songs[length].year = year;
    list_of_songs[length].text = text;
    length++;
}

void show_song(vector <song>& list_of_songs, int number)
{
    cout << "Название: " << list_of_songs[number].name << endl;
    if (list_of_songs[number].year != NULL) cout << "Год создания: " << list_of_songs[number].year;
    cout << endl;
    vector<string>::iterator it;
    int i = 1;
    for (it = list_of_songs[number].text.begin(); it != list_of_songs[number].text.end(); it++, i++)
    {
        cout << i << "\t" << *it << endl;
    }
}