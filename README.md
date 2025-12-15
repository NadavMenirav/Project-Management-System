# 💼 Project Management System

A command-line company and project management system written in C that allows workers to join companies, create projects, manage features, and track team assignments.

---

## 📋 Overview

This system provides a complete solution for managing workers, projects, and project features within a company structure. Workers can join the company, create projects, collaborate with other workers, add/remove features, and leave the company when needed.

## ✨ Features

- 👥 **Worker Management**: Add workers to the company and track their project assignments
- 📁 **Project Creation**: Create new projects and assign workers to them
- 🤝 **Team Collaboration**: Add multiple workers to existing projects (up to 4 per project)
- ⚙️ **Feature Tracking**: Add and remove features from projects using a linked list structure
- 📊 **Project Overview**: View all active projects with their workers and features
- 👤 **Worker Overview**: View all workers and their project assignments
- 💾 **Dynamic Memory Management**: Efficient memory allocation and deallocation

---

## 🔨 Compilation

```bash
gcc -o company ex_5.c -I.
```

## 🚀 Usage

```bash
./company
```

The program will display an interactive menu with 8 options.

---

## 📖 Menu Options

1. **Join the company** - Add a new worker to the company
2. **Open a new project** - Create a new project (requires at least one worker)
3. **Join an existing project** - Add a worker to an existing project
4. **See what projects are in work** - Display all projects with their workers and features
5. **See which people are in what projects** - Display all workers and their project assignments
6. **Work on existing project** - Add or remove features from a project
7. **Leave the company** - Remove a worker from the company and all their projects
8. **Exit** - Exit the program

---

## ⚙️ System Constraints

- 👥 **Maximum workers**: 10
- 📁 **Maximum projects**: Configurable (MAX_PROJECTS)
- 🤝 **Maximum workers per project**: 4
- 📝 **Maximum player name length**: 20 characters

---

## 🏗️ Data Structures

### Worker
```c
typedef struct {
    char* name;
    Project** projects;
    int projectCount;
} Worker;
```

### Project
```c
typedef struct {
    char* name;
    Worker** workers;
    int workerCount;
    FeatureNode* features;
} Project;
```

### Feature (Linked List)
```c
typedef struct FeatureNode {
    char* feature;
    struct FeatureNode* next;
} FeatureNode;
```

### Company
```c
typedef struct {
    Worker* workers[MAX_WORKERS];
    int workerCount;
    Project* projects[MAX_PROJECTS];
    int projectCount;
} Company;
```

---

## 🔧 Key Functionality

### Worker Management
- Workers must have unique names within the company
- Workers are automatically removed from all projects when they leave
- Empty projects (no workers) are automatically deleted

### Project Management
- Projects must have unique names within the company
- Only workers can create projects
- Projects can have 1-4 workers
- Projects maintain a linked list of features

### Feature Management
- Features are stored as a linked list
- Duplicate features within a project are not allowed
- Features can be added or removed by any worker on the project

---

## ⌨️ Input Handling

The system uses dynamic string allocation with `scanString()` function to handle variable-length input for:
- Worker names
- Project names
- Feature names
- User choices (add/remove)

---

## 💾 Memory Management

The program implements proper memory management:
- Dynamic allocation for workers, projects, and features
- Proper deallocation when workers quit
- Cleanup of empty projects
- Complete memory cleanup on exit via `freeCompany()`

---

## ⚠️ Error Handling

The system validates:
- ✅ Menu choices (1-8)
- ✅ Worker selections
- ✅ Project selections
- ✅ Duplicate names (workers and projects)
- ✅ Project capacity limits
- ✅ Feature operations
- ✅ Memory allocation failures

---

## 💡 Example Usage Flow

```
1. Worker "Alice" joins the company
2. Alice creates project "Website"
3. Worker "Bob" joins the company
4. Bob joins project "Website"
5. Alice adds feature "Login System" to Website
6. Bob adds feature "User Dashboard" to Website
7. View all projects (shows Website with Alice, Bob, and 2 features)
8. Alice leaves the company (removed from Website)
```

---

## 🛠️ Technical Details

- **Language**: C
- **Memory Model**: Dynamic allocation with manual memory management
- **Data Structure**: Array-based workers/projects with linked list features
- **Input Method**: Interactive menu-driven interface
- **String Handling**: Custom dynamic string scanning

---

## 📂 File Structure

- `ex_5.c` - Main implementation file
- `ex_5.h` - Header file with structure definitions and function declarations

---

## 📝 Notes

- The system uses recursive menu navigation
- All dynamically allocated memory is properly freed on exit
- Worker and project relationships are bidirectional (workers track projects, projects track workers)
- The system prevents invalid operations (e.g., creating projects without workers)

---

## 👨‍💻 Author

**Nadav Menirav**

---

## 📄 License

Educational project - Ex 5
