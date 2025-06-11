#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <set>
#include <algorithm>
#include <ctime>

struct Course {
    std::string code, name, roomType;
    int maxCapacity;
    std::set<int> students;
    std::vector<int> timeSlots;

    // Default constructor
    Course() : code(""), name(""), roomType(""), maxCapacity(0) {}

    // Parameterized constructor
    Course(const std::string& c, const std::string& n, const std::string& r, int cap)
        : code(c), name(n), roomType(r), maxCapacity(cap) {}
};

struct Student {
    int id, year;
    std::string major;
    std::vector<std::string> courses;

    // Default constructor
    Student() : id(0), year(0), major("") {}

    // Parameterized constructor
    Student(int i, const std::string& m, int y) : id(i), major(m), year(y) {}
};

struct Room {
    int number, capacity;
    std::string type, equipment;

    // Default constructor
    Room() : number(0), capacity(0), type(""), equipment("") {}

    // Parameterized constructor
    Room(int n, const std::string& t, int cap) : number(n), type(t), capacity(cap) {}
};

struct Request {
    int studentID;
    std::string courseCode;
    time_t timestamp;
    int priority;

    bool operator<(const Request& other) const {
        return priority < other.priority || (priority == other.priority && timestamp > other.timestamp);
    }
};

class Scheduler {
    std::unordered_map<std::string, Course> courses;
    std::unordered_map<int, Student> students;
    std::unordered_map<int, Room> rooms;
    std::priority_queue<Request> requestQueue;

public:
    void addCourse(const std::string& code, const std::string& name, const std::string& room, int capacity) {
        courses[code] = Course(code, name, room, capacity);
    }
    void addStudent(int id, const std::string& major, int year) {
        students[id] = Student(id, major, year);
    }
    void addRoom(int number, const std::string& type, int capacity) {
        rooms[number] = Room(number, type, capacity);
    }
    void requestRegistration(int studentID, const std::string& courseCode) {
        int priority = students[studentID].year + (students[studentID].major == courseCode.substr(0, 3) ? 1 : 0);
        requestQueue.push({studentID, courseCode, std::time(nullptr), priority});
    }
    void processRegistrations() {
        while (!requestQueue.empty()) {
            Request req = requestQueue.top();
            requestQueue.pop();
            auto& course = courses[req.courseCode];
            if (course.students.size() < course.maxCapacity) {
                course.students.insert(req.studentID);
                students[req.studentID].courses.push_back(req.courseCode);
            }
        }
    }
    void optimizeSchedule() {
        for (auto& [courseCode, course] : courses)
            for (auto& [roomNum, room] : rooms)
                if (room.type == course.roomType && room.capacity >= course.maxCapacity)
                    course.timeSlots = {9, 11, 13};  // Assign sample slots and stop on first match
    }
    void printSchedules() const {
        for (const auto& [id, student] : students) {
            std::cout << "Student " << id << " Schedule:\n";
            for (const auto& courseCode : student.courses) {
                std::cout << " - " << courses.at(courseCode).name << " at slots ";
                for (int slot : courses.at(courseCode).timeSlots) std::cout << slot << " ";
                std::cout << "\n";
            }
        }
    }
};

int main() {
    Scheduler scheduler;
    scheduler.addCourse("CS101", "Intro to CS", "Lab", 30);
    scheduler.addCourse("ENG202", "English Lit", "Lecture", 50);
    scheduler.addStudent(1, "CS", 2);
    scheduler.addStudent(2, "ENG", 3);
    scheduler.addRoom(101, "Lab", 30);
    scheduler.addRoom(102, "Lecture", 50);

    scheduler.requestRegistration(1, "CS101");
    scheduler.requestRegistration(2, "ENG202");

    scheduler.processRegistrations();
    scheduler.optimizeSchedule();
    scheduler.printSchedules();

    return 0;
}
