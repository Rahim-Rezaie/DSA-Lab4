#include <iostream>
#include <string>
#include <vector>
using namespace std;

// each node stores ONE bit, plus links both ways
struct Node {
    int bit;
    Node* next;
    Node* prev;
};

// -----------------------------------------------------
// Helper: get the tail (LSB / rightmost node) of a DLL
// -----------------------------------------------------
Node* getTail(Node* head) {
    if (head == nullptr) return nullptr;
    Node* temp = head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    return temp;
}

// -----------------------------------------------------
// Helper: insert a bit at the HEAD (used while building numbers MSB-first)
// -----------------------------------------------------
Node* insertAtHead(Node*& head, int bitValue) {
    Node* newNode = new Node();
    newNode->bit = bitValue;
    newNode->prev = nullptr;
    newNode->next = head;

    if (head != nullptr) {
        head->prev = newNode;
    }
    head = newNode;
    return head;
}

// -----------------------------------------------------
// Helper: insert a bit at the TAIL (used for left-shifting during
// multiplication)
// -----------------------------------------------------
void insertAtTail(Node*& head, int bitValue) {
    Node* newNode = new Node();
    newNode->bit = bitValue;
    newNode->next = nullptr;

    if (head == nullptr) {
        newNode->prev = nullptr;
        head = newNode;
        return;
    }

    Node* tail = getTail(head);
    tail->next = newNode;
    newNode->prev = tail;
}

// -----------------------------------------------------
// Helper: count how many bit-nodes are in the list
// -----------------------------------------------------
int countBits(Node* head) {
    int count = 0;
    Node* temp = head;
    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }
    return count;
}

// -----------------------------------------------------
// Helper: pad the number with leading zero-nodes until length is a multiple of
// 8
// -----------------------------------------------------
void padToByteBoundary(Node*& head) {
    int len = countBits(head);
    int remainder = len % 8;
    if (remainder == 0) return;  // already a clean multiple of 8

    int zerosNeeded = 8 - remainder;
    for (int i = 0; i < zerosNeeded; i++) {
        insertAtHead(head, 0);
    }
}

// -----------------------------------------------------
// Helper: deep copy an entire DLL (needed for multiplication so we don't
// destroy the original numbers while shifting/adding)
// -----------------------------------------------------
Node* copyList(Node* head) {
    Node* newHead = nullptr;
    Node* temp = head;
    while (temp != nullptr) {
        insertAtTail(newHead, temp->bit);
        temp = temp->next;
    }
    return newHead;
}

// -----------------------------------------------------
// 1. Store Binary Number - builds an 8-bit-grouped DLL from user input
// -----------------------------------------------------
Node* storeBinary(string bits) {
    Node* head = nullptr;
    for (char c : bits) {
        insertAtTail(head, c - '0');  // convert char '0'/'1' to actual int 0/1
    }
    padToByteBoundary(head);  // extend with leading zeros to full byte blocks
    return head;
}

// -----------------------------------------------------
// Display - prints bits grouped into bytes (8 bits at a time)
// -----------------------------------------------------
void displayNumber(Node* head) {
    Node* temp = head;
    int count = 0;
    while (temp != nullptr) {
        cout << temp->bit;
        count++;
        if (count % 8 == 0 && temp->next != nullptr) {
            cout << " ";  // space between each 8-bit block
        }
        temp = temp->next;
    }
    cout << endl;
}

// -----------------------------------------------------
// 2. 1's Complement - traverses the DLL and flips every bit
// -----------------------------------------------------
Node* onesComplement(Node* head) {
    Node* copy = copyList(head);  // work on a copy so original stays intact
    Node* temp = copy;
    while (temp != nullptr) {
        temp->bit = (temp->bit == 0) ? 1 : 0;  // flip 0 to 1, or 1 to 0
        temp = temp->next;
    }
    return copy;
}

// -----------------------------------------------------
// 4. Binary Addition - adds two DLL binary numbers, carry handled via
// traversal from LSB (tail) backward using prev pointers
// -----------------------------------------------------
Node* addBinary(Node* a, Node* b) {
    Node* copyA = copyList(a);
    Node* copyB = copyList(b);

    // pad both to the SAME length so we can add bit by bit safely
    while (countBits(copyA) < countBits(copyB)) insertAtHead(copyA, 0);
    while (countBits(copyB) < countBits(copyA)) insertAtHead(copyB, 0);

    Node* pa = getTail(copyA);
    Node* pb = getTail(copyB);
    int carry = 0;
    Node* result = nullptr;

    // walk backward from LSB to MSB using prev pointers, building result at
    // head each time
    while (pa != nullptr && pb != nullptr) {
        int sum = pa->bit + pb->bit + carry;
        int resultBit = sum % 2;
        carry = sum / 2;

        insertAtHead(result, resultBit);

        pa = pa->prev;
        pb = pb->prev;
    }

    if (carry == 1) {
        insertAtHead(result, 1);  // leftover carry becomes a new leading bit
    }

    padToByteBoundary(result);
    return result;
}

// -----------------------------------------------------
// 3. 2's Complement - 1's complement, then add binary "1"
// -----------------------------------------------------
Node* twosComplement(Node* head) {
    Node* flipped = onesComplement(head);
    Node* one = storeBinary("1");  // represents the number 1 as its own DLL
    Node* result = addBinary(flipped, one);
    return result;
}

// -----------------------------------------------------
// Helper: shift a binary DLL left by 1 (multiply by 2) - appends a zero at tail
// -----------------------------------------------------
Node* shiftLeft(Node* head) {
    Node* copy = copyList(head);
    insertAtTail(copy, 0);  // appending 0 at the LSB end shifts everything left
    return copy;
}

// -----------------------------------------------------
// 5. Binary Multiplication - shift-and-add method
// -----------------------------------------------------
Node* multiplyBinary(Node* a, Node* b) {
    Node* result = storeBinary("0");
    Node* shifted = copyList(a);

    // collect B's bits from LSB to MSB so we process the right-most bit first
    vector<int> bBitsLSBFirst;
    Node* temp = getTail(b);
    while (temp != nullptr) {
        bBitsLSBFirst.push_back(temp->bit);
        temp = temp->prev;
    }

    for (int i = 0; i < (int)bBitsLSBFirst.size(); i++) {
        if (bBitsLSBFirst[i] == 1) {
            result = addBinary(result, shifted);
        }
        shifted = shiftLeft(
            shifted);  // move to next bit position (multiply by 2 each time)
    }

    return result;
}

// -----------------------------------------------------
// 6. Conversion to Decimal - traverses MSB to LSB, doubling each step
// -----------------------------------------------------
long long toDecimal(Node* head) {
    long long value = 0;
    Node* temp = head;
    while (temp != nullptr) {
        value =
            value * 2 + temp->bit;  // classic binary-to-decimal accumulation
        temp = temp->next;
    }
    return value;
}

// -----------------------------------------------------
// Cleanup - frees an entire DLL
// -----------------------------------------------------
void deleteList(Node* head) {
    Node* temp = head;
    while (temp != nullptr) {
        Node* nextNode = temp->next;
        delete temp;
        temp = nextNode;
    }
}

// -----------------------------------------------------
// Main - menu driven interface
// -----------------------------------------------------
int main() {
    Node* numA = nullptr;
    Node* numB = nullptr;
    int choice;
    string input;

    do {
        cout << "\n===== Binary Arithmetic (DLL) =====" << endl;
        cout << "1. Store Binary Number A" << endl;
        cout << "2. Store Binary Number B" << endl;
        cout << "3. Display A and B" << endl;
        cout << "4. 1's Complement of A" << endl;
        cout << "5. 2's Complement of A" << endl;
        cout << "6. Add A + B" << endl;
        cout << "7. Multiply A * B" << endl;
        cout << "8. Convert A to Decimal" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "Enter bits for A (e.g. 1011): ";
            cin >> input;
            if (numA != nullptr) deleteList(numA);
            numA = storeBinary(input);
            cout << "Stored A: ";
            displayNumber(numA);
        } else if (choice == 2) {
            cout << "Enter bits for B (e.g. 0110): ";
            cin >> input;
            if (numB != nullptr) deleteList(numB);
            numB = storeBinary(input);
            cout << "Stored B: ";
            displayNumber(numB);
        } else if (choice == 3) {
            cout << "A: ";
            displayNumber(numA);
            cout << "B: ";
            displayNumber(numB);
        } else if (choice == 4) {
            if (numA == nullptr) {
                cout << "Store A first." << endl;
                continue;
            }
            Node* result = onesComplement(numA);
            cout << "1's Complement of A: ";
            displayNumber(result);
            deleteList(result);
        } else if (choice == 5) {
            if (numA == nullptr) {
                cout << "Store A first." << endl;
                continue;
            }
            Node* result = twosComplement(numA);
            cout << "2's Complement of A: ";
            displayNumber(result);
            deleteList(result);
        } else if (choice == 6) {
            if (numA == nullptr || numB == nullptr) {
                cout << "Store both A and B first." << endl;
                continue;
            }
            Node* result = addBinary(numA, numB);
            cout << "A + B = ";
            displayNumber(result);
            deleteList(result);
        } else if (choice == 7) {
            if (numA == nullptr || numB == nullptr) {
                cout << "Store both A and B first." << endl;
                continue;
            }
            Node* result = multiplyBinary(numA, numB);
            cout << "A * B = ";
            displayNumber(result);
            deleteList(result);
        } else if (choice == 8) {
            if (numA == nullptr) {
                cout << "Store A first." << endl;
                continue;
            }
            cout << "Decimal value of A: " << toDecimal(numA) << endl;
        } else if (choice == 9) {
            cout << "Exiting. Cleaning up memory..." << endl;
            if (numA != nullptr) deleteList(numA);
            if (numB != nullptr) deleteList(numB);
        } else {
            cout << "Invalid choice, try again." << endl;
        }

    } while (choice != 9);

    return 0;
}