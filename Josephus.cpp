#include <iostream>
using namespace std;

// each node represents one person in the circle
struct Node {
    int id;
    Node* next;
};

// -----------------------------------------------------
// Create Circle - builds a circular linked list of N people
// -----------------------------------------------------
Node* createCircle(int n) {
    Node* head = nullptr;
    Node* last = nullptr;

    for (int i = 1; i <= n; i++) {
        Node* newNode = new Node();
        newNode->id = i;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
            last = newNode;
        } else {
            last->next = newNode;
            last = newNode;
        }
    }

    // completing the circle - last person points back to head
    last->next = head;
    return head;
}

// -----------------------------------------------------
// Elimination Process - eliminates every k-th person until one remains
// -----------------------------------------------------
void runJosephus(Node* head, int k) {
    Node* current = head;
    Node* prev = nullptr;

    // find the initial "prev" so we can unlink properly - prev starts as the
    // LAST node
    prev = head;
    while (prev->next != head) {
        prev = prev->next;
    }

    cout << "\nElimination Order: ";

    // keep eliminating until only one person is left (current points to itself)
    while (current->next != current) {
        // move (k-1) steps forward to reach the k-th person
        for (int i = 1; i < k; i++) {
            prev = current;
            current = current->next;
        }

        // current is now the person to eliminate
        cout << current->id << " ";

        prev->next = current->next;  // unlink current from the circle
        Node* toDelete = current;
        current = current->next;  // move on to the next person
        delete toDelete;          // free the eliminated person's memory
    }

    cout << endl;

    // current is now the only node left - the survivor
    cout << "Survivor: Person " << current->id << endl;

    delete current;  // clean up the last remaining node
}

int main() {
    int n, k;

    cout << "Enter number of people (N): ";
    cin >> n;
    cout << "Enter step count (k): ";
    cin >> k;

    Node* head = createCircle(n);
    runJosephus(head, k);

    return 0;
}