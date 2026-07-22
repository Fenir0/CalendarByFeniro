A collaborative calendar. You can edit days and synchronize your notes with other people.

Features:

- Selecting and editing days

- Real-time synchronization of events between users

- Regulating access to the document you've shared

  
# Build

- Compiler: C++20 or higher
- Third-party libraries used:
	- Qt6
	- OpenSSL
	- Boost
	- nlohmann-json
# Debian 13:

1. Download necessary packages
```bash
# Both server and client!
sudo apt install build-essential cmake curl zip unzip tar git qt6-base-dev qt6-declarative-dev libssl-dev pkg-config

# Server:
sudo apt install libpqxx-dev libboost-system-dev nlohmann-json3-dev postgresql postgresql-contrib bison flex
```
2. Download [vcpkg](https://github.com/microsoft/vcpkg) from official Microsoft repository, run the bootstrap
```bash
git clone https://github.com/microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh 
```

3. Download the project files
```bash
git clone -b main https://github.com/Fenir0/CalendarByFeniro
cd CalendarByFeniro

# Continue to client folder:
cd client
# OR Continue to server foler:
cd server
```

3. Install required libraries (run inside client or server folder)
```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path/to/vcpkg_folder]/scripts/buildsystems/vcpkgcmake .cmake

# E.G. building for client if vcpkg and CalendarByFeniro are in the same folder:
	#  vcpkg/scripts/buildsystems/vcpkg.cmake
	#  CalendarByFeniro/client/...
	#  CalendarByFeniro/server/...
# cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/vcpkg.cmake

```

4. Build the project (run inside client or server folder)
```bash
cmake --build build
```
   
5.  If building the server: set up a database
   ```bash
# 1. Set the default postgres user password to match the server config
sudo -u postgres psql -c "ALTER USER postgres PASSWORD 'admin';"

# 2. Create the database
sudo -u postgres createdb calendarbyfeniro
   ```
6. Run
**Client**
```bash
./build/CalendarByFeniro
```
**Server**
```bash
./build/Server
```
