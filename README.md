# Seat Allocation Simulator

This is a C++ program that simulates a seat allocation system for students based on rank, category, and college preferences. It mimics how real-world entrance exam seat allocation (like JEE) works with multiple rounds, FLOAT/FREEZE choices, and seat withdrawals.

---

## Features

- Supports **multiple allocation rounds** (4 rounds).
- Category-based reservation (GEN, OBC, SC, ST).
- User can:
  - **Freeze** seat to lock it.
  - **Float** to try upgrading in next round.
  - **Withdraw** from further allocation.
- Real-time interaction through the console.
- Displays complete **allocation history** for the user.

---

## How It Works

1. The program has some pre-loaded students with ranks and preferences.
2. A new user is asked to:
   - Enter their name, rank, category.
   - Enter college preferences (e.g., `IITB IITD IITK`).
3. Seat allocation happens over **4 rounds**:
   - Top-ranked students are prioritized.
   - Reservations are honored per category.
   - Users interactively choose to **FREEZE**, **FLOAT**, or **WITHDRAW**.
4. At the end, a detailed report shows **allocation progress for each round**.

---

## Technologies Used

- C++
- Standard Template Library (STL)
  - `vector`
  - `map`
  - `algorithm`
  - `sstream`

---

## How to Run

1. Copy the code to a `.cpp` file, e.g., `seat_allocation.cpp`
2. Compile it using any C++ compiler, e.g., g++:

   ```bash
   g++ seat_allocation.cpp -o seat_allocation
