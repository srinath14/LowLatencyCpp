#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

int ind = 0;
mutex mtx;
condition_variable cv;
int turn = 0;
int charSize = 0;
int num_threads = 0;

void read(int id, string str)
{
    while (true)
    {
        int size = str.size();
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]
                { return (turn == id); });
        int count = 0;
        while (ind >= 0 && count < charSize)
        {
            cout << str[ind];
            ind = (ind + 1) % size;
            count++;
        }
        cout << endl;
        turn = (turn + 1) % num_threads;
        lock.unlock();
        cv.notify_all();
    }
}

/*binary string charsize, numthreads*/
int main(int argc, char *argv[])
{
    cout << "argv[1] = " << argv[1] << endl;
    cout << "argv[2] = " << argv[2] << endl;
    cout << "argv[3] = " << argv[3] << endl;

    num_threads = stoi(argv[3]);
    int chars = stoi(argv[2]);
    charSize = chars;
    string str = argv[1];
    // int size = str.size();
    vector<thread> threads;

    for (int i = 0; i < num_threads; i++)
    {
        threads.emplace_back(read, i, str);
    }

    for (int i = 0; i < num_threads; i++)
    {
        threads[i].join();
    }

    return 0;
}