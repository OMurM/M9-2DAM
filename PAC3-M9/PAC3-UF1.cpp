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

    // Define an array of phrases with variable positions for the name
    string frases[] = {
        "Hello, my name is ",
        "My name is ",
        "I am known as ",
        "You can call me ",
        "I am called ",
        "Greetings, I am ",
        "I go by the name of ",
        "People know me as ",
        "They call me "
    };

    // Generate a random index to select a phrase
    int phraseIndex = rand() % (sizeof(frases) / sizeof(frases[0]));

    // Print the selected phrase with the random name
    cout << frases[phraseIndex] << randomName << "." << endl;
}


int main() {
    int N;
    cout << "Enter the number of processes to create: ";
    cin >> N;

    // Seed to randomize time
    srand(time(nullptr));

    // Ask user for names
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
        } else if (pid == 0) {
            if (i == chosenProcess) {
                
                // Only the chosen process prints the phrase
                displayRandomPhrase();
            
            }
            exit(0); // Exit child proces
        }
    }

    // Wait for all child processes to finish
    for (int i = 0; i < N; i++) {
        wait(nullptr);
    }

    return 0; // Ensure the program returns here properly.
}
