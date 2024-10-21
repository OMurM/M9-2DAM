#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <string>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Global vector for names
vector<string> names;

// Function to add a name to the global vector
void addname(const string& name) {
    names.push_back(name);
}

// Function to display a random name with a fixed phrase
void displayRandomPhrase() {
    if (names.empty()) return;

    // Select a random name from the vector
    int randomIndex = rand() % names.size();
    string randomName = names[randomIndex];

    // Print the selected name with a fixed phrase
    cout << "Hello, my name is " << randomName << "." << endl;
}

int main() {
    int N;
    cout << "Enter the number of processes to create: ";
    cin >> N;

    // Seed random number generator
    srand(time(nullptr));

    // Ask user for names to add
    for (int i = 0; i < N; i++) {
        string name;
        cout << "Enter a name for process " << i + 1 << ": ";
        cin >> name;
        addname(name);
    }

    // Randomly choose one process to print the phrase
    int chosenProcess = rand() % N;
    
    // Create N processes
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            cerr << "Error creating process" << endl;
        } else if (pid == 0) {  // Child process
            if (i == chosenProcess) {
                // Only the chosen process prints the phrase
                displayRandomPhrase();
            }
            exit(0); // Ensure child process exits after handling
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < N; i++) {
        wait(nullptr);
    }

    return 0; // Ensure the program returns here properly, no extra output
}
