# DSA Lab — Circular & Doubly Linked Lists

Three C++ programs exploring different applications of linked structures: a playlist manager, a classic elimination problem, and binary arithmetic — all built without using STL containers, so every operation is handled manually through pointers.

---

## 📁 Files

| File | Task |
|---|---|
| `playlist_dll.cpp` | Playlist Management System (DLL) |
| `josephus_cll.cpp` | Josephus Problem Simulation (CLL) |
| `binary_arithmetic_dll.cpp` | Binary Arithmetic Operations (DLL) |

---

## 1. Playlist Management System

A music playlist built on a **Doubly Linked List**, where each song is a node storing an ID, name, and duration. Since it's doubly linked, the playlist can be walked in both directions and supports a "currently playing" pointer that moves independently of insertions/deletions.

**Operations implemented:**
- Add a song to the end of the playlist
- Delete a song by ID
- Display the playlist forward and backward
- Search for a song by ID
- Play Next / Play Previous (simulating a music player's playback position)
- Reverse the entire playlist in place

**Core idea:** every insertion and deletion updates both the `next` and `prev` pointers of the surrounding nodes, and the "currently playing" pointer is protected so it never ends up pointing at a deleted song.

---

## 2. Josephus Problem Simulation

A classic elimination puzzle solved using a **Circular Linked List**. N people stand in a circle, and starting from person 1, every k-th person is eliminated until only one survivor remains.

**Operations implemented:**
- Build a circular list of N people
- Run the elimination process, updating links after each removal
- Print the order of elimination
- Print the final survivor

**Core idea:** the circle is maintained by unlinking each eliminated node directly — no shifting or array resizing needed, which is exactly where circular linked lists outperform arrays for this kind of problem.

**Reference test case:** N = 7, k = 3 → survivor is person 4.

---

## 3. Binary Arithmetic Using Doubly Linked List

Binary numbers represented bit-by-bit inside a **Doubly Linked List**, with each node holding a single 0 or 1. Numbers are stored in clean 8-bit blocks, padding with leading zeros as needed.

**Operations implemented:**
- Store a binary number (auto-padded to 8-bit blocks)
- Compute 1's Complement (flip every bit)
- Compute 2's Complement (1's complement + binary addition of 1)
- Add two binary numbers, handling carries through the list
- Multiply two binary numbers using the shift-and-add method
- Convert a stored binary number to its decimal equivalent

**Core idea:** addition works right-to-left (LSB to MSB) using the `prev` pointer chain — the one operation in this entire lab that actually *requires* the doubly linked structure, since carries propagate from the least significant bit upward while the result still needs to be built starting from the most significant bit.

---

## 🧠 Concepts Practiced

- Doubly linked list construction, traversal, and pointer rewiring
- Circular linked list construction and safe unlinking during traversal
- Maintaining auxiliary pointers (`current`, `tail`) alongside `head` without breaking list integrity
- Manual binary arithmetic (complement, addition, multiplication) without relying on built-in integer types
- Careful memory management — every `new` is matched with a `delete`

---

## ▶️ How to Run

```bash
g++ filename.cpp -o output
./output
```

Replace `filename.cpp` with any of the three source files listed above.
