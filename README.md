# Cloud storage prototype

---

This repository describes a cloud storage prototype. The file from the client is sent to the server, where the file itself will be stored in the future.

---

**Stack used:**
- C++
- Boost ASIO module
- CMake build system (and Make build automation)
- Shell scripts for quick project assembly

---

**How use?:**
You can build it yourself or through shell scripts.
- Shell
```shell
cd server/scripts
bash build.sh
```

```shell
cd client/scripts
bash build.sh
```

- Self-assembly
```zsh
cd server
mkdir build
cd build
cmake ..
make
./server
```

```zsh
cd client
mkdir build
cd build
cmake ..
make
./client
```

---

**Note: It is recommended to check the boost library path inside CMakeLists.txt and if necessary change to your correct path**

---

Author: FloyzenCode