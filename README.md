# TermiNet Backend

Backend repository for [TermiNet](https://github.com/ibrahimamam1/termiNet), a reddit inspired networking application written in c++.

## Prerequisites

Before running the backend, ensure you have the following installed:
- **C++ Compiler** (supports C++17 or higher)
- **CMake** (version 3.10+)
- **PostgreSQL** (version 12+)
- **libpqxx** (PQXX library for PostgreSQL)

---

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/ibrahimamam1/termiNet_backend.git
   cd termiNet_backend

2. **Build the project**:
    ```bash
    mkdir -p build && cd build
    cmake ..
    make

## Database Setup

1. **Create the Database**:
    ```bash
    sudo -iu postgres 
    createdb <database_name> -O <your_username>  # Replace with your details
    exit

2. **Run the Database script**:
   ```bash
   psql -U <your_username> -d <database_name> -h localhost -p 5432 -f ../sql/run_all.sql

3. **Edit your .ENV FILE**:
   Edit the .env file found in the project root directory to contain your database information you just created.

## Run the Application
1. Execute the App
    ```bash
      ./terminet
