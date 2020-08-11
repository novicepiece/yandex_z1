#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <clocale>

#define min(a, b) (a <= b ? a : b)
#define max(a, b) (a >= b ? a : b)

using std::cout;
using std::cin;

std::ifstream fin("input.txt");
std::ofstream fout;

const uint32_t max_init = 100; // Максимальное количество ходов за игру

bool next_parting(std::vector<int> &v, int n) // Следующее разложение числа
{
    if (v[0] == n)
    {
        return false;
    }

    int min_num = v[0];
    int min_index = 0;

    for (size_t i = 0; i < v.size() - 1; i++)
    {
        if (v[i] < min_num)
        {
            min_num = v[i];
            min_index = i;
        }
    }

    v[min_index] += 1;
    v[v.size() - 1] -= 1;

    v.erase(v.begin() + min_index + 1, v.end());
    int array_sum = std::accumulate(v.begin(), v.end(), 0);

    for (int i = 0; i < n - array_sum; i++)
    {
        v.push_back(1);
    }

    return true;
}

bool next_step(std::vector<int> &v, uint16_t diff) // Следующий шаг в игре
{
    std::vector<int> prev_v = v;
    v.push_back(0);

    for (size_t i = 0; i < v.size() - 1; i++)
    {
        if (v[i] > diff)
        {
            v[v.size() - 1] += diff;
            v[i] -= diff;
        }
        else
        {
            v[v.size() - 1] += v[i];
            v.erase(v.begin() + i);
            i--;
        }
    }

    for (size_t i = 0; i < v.size(); i++)
    {
        fout << v[i] << ' ';
    }
    fout << '\n';

    if (prev_v == v)
    {
       fout << "Cycled! Stop the game!\n\n";
       return false;
    }
    else
    {
        return true;
    }
}

bool isCycled(int x, uint16_t diff) // Проверка на цикличность числа
{
    for (int i = 1; x > 0; x -= i * diff, i++);
    return x == 0;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    uint16_t n;
    cout << "Enter the number: ";
    cin >> n;

    uint16_t diff;
    cout << "Enter the difference between numbers: ";
    cin >> diff;


    bool cycled_number = isCycled(n, diff);
    cout << (cycled_number ?  "This number is cycled\n" : "This number is not cycled\n");

    std::vector<int> heap(n);
    std::fill(heap.begin(), heap.end(), 1);
    bool all_games_cycled = true;
    bool one_game_cycled = false;

    fout.open("games.txt");

    do // Сыграть все возможные игры с числом
    {
        fout << "---------------------------------------------\n\nNew game starts with ";
        for (size_t i = 0; i < heap.size(); i++)
        {
            fout << heap[i] << ' ';
        }
        fout << "...\n";

        std::vector<int> temp = heap;
        int init = max_init;
        while (next_step(temp, diff) && init > 0) // Играть либо до цикла, либо до таймаута
        {
            init--;
        }

        if (init <= 0)
        {
            fout << "Out of time and memory!\n\n";
            all_games_cycled = false;
        }
        else
        {
            one_game_cycled = true;
        }

    } while (next_parting(heap, n));

    fout.close();
    fout.open("output.txt");

    cout << "Checkout output.txt for results and games.txt for game logs!\n";
    fout << "\n=====================================\nResults\n=====================================\n\n";
    fout << "cycled_number = " << (cycled_number ? "true. //It is cycled!\n" : "false. //It is NOT cycled.\n");
    fout << "one_game_cycled = " << (one_game_cycled ? "true. //At least one game was cycled!\n" : "false. //Not a single game got cycled.\n");
    fout << "all_games_cycled = " << (all_games_cycled ? "true. //All possible games were cycled!\n" : "false. //NOT all possible games were cycled...\n");
    fout << "proofed_cycling = " << (cycled_number ? (all_games_cycled ? "true. //Proofed cycled number! Nice!\n" : "false. //Something gone wrong, maybe you should raise the max_init variable...\n") : (!one_game_cycled ? "false. As I said, it is not cycled.\n" : "???\n"));
    fout << "errors = " << (one_game_cycled ^ all_games_cycled ? "oh... //Something gone COMPLETELY WRONG!!!!! яыэгдесдмшдтнкфдк" : "no. //Everything is fine.");

    fin.close();
    fout.close();
    return 0;
}
