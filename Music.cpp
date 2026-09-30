#include <iostream>
#include <string>
using namespace std;

// each node holds one song's info plus links to next/prev songs
struct Node {
    int songID;
    string songName;
    int minutes;
    int seconds;
    Node* next;
    Node* prev;
};

Node* head = nullptr;
Node* tail = nullptr;
Node* current =
    nullptr;  // keeps track of "currently playing" song for next/previous

// ---------------------------------------------------------
// 1. Add Song - inserts a new song at the END of the playlist
// ---------------------------------------------------------
void addSong(int id, string name, int mins, int secs) {
    Node* newNode = new Node();
    newNode->songID = id;
    newNode->songName = name;
    newNode->minutes = mins;
    newNode->seconds = secs;
    newNode->next = nullptr;

    if (tail == nullptr) {
        // playlist was empty, new song becomes both head and tail
        newNode->prev = nullptr;
        head = newNode;
        tail = newNode;
        current = newNode;  // first song added becomes the "currently playing"
                            // pointer
    } else {
        newNode->prev = tail;  // new node points back to old tail
        tail->next = newNode;  // old tail points forward to new node
        tail = newNode;        // new node becomes the new tail
    }

    cout << "Added: " << name << " (ID " << id << ")" << endl;
}

// ---------------------------------------------------------
// 2. Delete Song - removes a song by its ID
// ---------------------------------------------------------
void deleteSong(int id) {
    Node* temp = head;

    // walk through the list looking for matching ID
    while (temp != nullptr && temp->songID != id) {
        temp = temp->next;
    }

    if (temp == nullptr) {
        cout << "Song with ID " << id << " not found." << endl;
        return;
    }

    // if the song being deleted is the one currently "playing", move current
    // forward first
    if (current == temp) {
        if (temp->next != nullptr) {
            current = temp->next;
        } else {
            current = temp->prev;
        }
    }

    // reconnect the node BEFORE temp
    if (temp->prev != nullptr) {
        temp->prev->next = temp->next;
    } else {
        head = temp->next;  // temp was the head
    }

    // reconnect the node AFTER temp
    if (temp->next != nullptr) {
        temp->next->prev = temp->prev;
    } else {
        tail = temp->prev;  // temp was the tail
    }

    cout << "Deleted: " << temp->songName << " (ID " << id << ")" << endl;
    delete temp;
}

// ---------------------------------------------------------
// 3. Display Playlist Forward
// ---------------------------------------------------------
void displayForward() {
    if (head == nullptr) {
        cout << "Playlist is empty." << endl;
        return;
    }

    Node* temp = head;
    cout << "\n--- Playlist (Forward) ---" << endl;
    while (temp != nullptr) {
        cout << temp->songID << ". " << temp->songName << " - " << temp->minutes
             << ":" << (temp->seconds < 10 ? "0" : "") << temp->seconds << endl;
        temp = temp->next;
    }
}

// ---------------------------------------------------------
// 4. Display Playlist Backward
// ---------------------------------------------------------
void displayBackward() {
    if (tail == nullptr) {
        cout << "Playlist is empty." << endl;
        return;
    }

    Node* temp = tail;
    cout << "\n--- Playlist (Backward) ---" << endl;
    while (temp != nullptr) {
        cout << temp->songID << ". " << temp->songName << " - " << temp->minutes
             << ":" << (temp->seconds < 10 ? "0" : "") << temp->seconds << endl;
        temp = temp->prev;
    }
}

// ---------------------------------------------------------
// 5. Search Song - finds song by ID and shows its details
// ---------------------------------------------------------
void searchSong(int id) {
    Node* temp = head;
    while (temp != nullptr) {
        if (temp->songID == id) {
            cout << "\nFound: " << temp->songName << " (ID " << temp->songID
                 << ") - " << temp->minutes << ":"
                 << (temp->seconds < 10 ? "0" : "") << temp->seconds << endl;
            return;
        }
        temp = temp->next;
    }
    cout << "Song with ID " << id << " not found." << endl;
}

// ---------------------------------------------------------
// 6. Play Next / Previous - moves the "current" pointer
// ---------------------------------------------------------
void playNext() {
    if (current == nullptr) {
        cout << "Playlist is empty." << endl;
        return;
    }
    if (current->next == nullptr) {
        cout << "Already at the last song." << endl;
        return;
    }
    current = current->next;
    cout << "Now playing: " << current->songName << " (ID " << current->songID
         << ")" << endl;
}

void playPrevious() {
    if (current == nullptr) {
        cout << "Playlist is empty." << endl;
        return;
    }
    if (current->prev == nullptr) {
        cout << "Already at the first song." << endl;
        return;
    }
    current = current->prev;
    cout << "Now playing: " << current->songName << " (ID " << current->songID
         << ")" << endl;
}

// ---------------------------------------------------------
// 7. Reverse Playlist - reverses the DLL in place
// ---------------------------------------------------------
void reversePlaylist() {
    Node* temp = head;
    Node* swapPtr = nullptr;

    // walk through every node, swapping its next and prev pointers
    while (temp != nullptr) {
        swapPtr = temp->prev;
        temp->prev = temp->next;
        temp->next = swapPtr;
        temp =
            temp->prev;  // move forward using the OLD next (now stored in prev)
    }

    // after the loop, swap head and tail since the list is now flipped
    swapPtr = head;
    head = tail;
    tail = swapPtr;

    current = head;  // reset "currently playing" to the new first song
    cout << "Playlist reversed." << endl;
}

// ---------------------------------------------------------
// Cleanup - frees all nodes before program exits
// ---------------------------------------------------------
void deleteAllSongs() {
    Node* temp = head;
    while (temp != nullptr) {
        Node* nextNode = temp->next;
        delete temp;
        temp = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    current = nullptr;
}

// ---------------------------------------------------------
// Main - menu driven interface
// ---------------------------------------------------------
int main() {
    int choice;

    do {
        cout << "\n===== Playlist Manager =====" << endl;
        cout << "1. Add Song" << endl;
        cout << "2. Delete Song" << endl;
        cout << "3. Display Forward" << endl;
        cout << "4. Display Backward" << endl;
        cout << "5. Search Song" << endl;
        cout << "6. Play Next" << endl;
        cout << "7. Play Previous" << endl;
        cout << "8. Reverse Playlist" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, mins, secs;
            string name;
            cout << "Enter Song ID: ";
            cin >> id;
            cout << "Enter Song Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Duration (minutes seconds): ";
            cin >> mins >> secs;
            addSong(id, name, mins, secs);
        } else if (choice == 2) {
            int id;
            cout << "Enter Song ID to delete: ";
            cin >> id;
            deleteSong(id);
        } else if (choice == 3) {
            displayForward();
        } else if (choice == 4) {
            displayBackward();
        } else if (choice == 5) {
            int id;
            cout << "Enter Song ID to search: ";
            cin >> id;
            searchSong(id);
        } else if (choice == 6) {
            playNext();
        } else if (choice == 7) {
            playPrevious();
        } else if (choice == 8) {
            reversePlaylist();
        } else if (choice == 9) {
            cout << "Exiting. Cleaning up memory..." << endl;
            deleteAllSongs();
        } else {
            cout << "Invalid choice, try again." << endl;
        }

    } while (choice != 9);

    return 0;
}