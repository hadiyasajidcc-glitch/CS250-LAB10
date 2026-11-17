#include <iostream>
using namespace std;

const int MAX_TRUCKS = 20;
const int MAX_LOAD = 50;

//one container in the linked list
struct Container {
    int id;
    int weight;
    Container* next;
    Container(int i, int w) {
        id = i;
        weight = w;
        next = NULL;
    }
};

//one truck
struct Truck {
    int id;
    int remaining;
    int loaded[MAX_LOAD];
    int count;
};

//queue of trucks array based
struct Queue {
    Truck trucks[MAX_TRUCKS];
    int front;
    int rear;

    Queue() {
        front = 0;
        rear = 0;
    }

    void enqueue(Truck t) {
        trucks[rear] = t;
        rear++;
    }

    bool isEmpty() {
        return front == rear;
    }

    Truck* frontTruck() {
        return &trucks[front];
    }

    //truck leaves we only move front so the truck data is not lost
    void dequeue() {
        front++;
    }

    //used when backtracking to bring the truck back
    void undoDequeue() {
        front--;
    }

    //total capacity left in the trucks still waiting in the queue
    int totalCapacity() {
        int total = 0;
        for (int i = front; i < rear; i++) {
            total += trucks[i].remaining;
        }
        return total;
    }
};

//add a container at the end of the list
void addContainer(Container*& head, Container*& tail, int id, int weight) {
    Container* n = new Container(id, weight);
    if (head == NULL) {
        head = tail = n;
    } else {
        tail->next = n;
        tail = n;
    }
}

//remWeight = weight of all containers from cur till the end of the list
bool loadContainers(Container* cur, Queue& q, int remWeight) {
    //all containers are placed
    if (cur == NULL) {
        return true;
    }

    //no trucks left
    if (q.isEmpty()) {
        return false;
    }

    //prune
    if (remWeight > q.totalCapacity()) {
        return false;
    }

    Truck* t = q.frontTruck();

    //put it on the front truck
    if (t->remaining >= cur->weight) {
        t->remaining -= cur->weight;
        t->loaded[t->count] = cur->id;
        t->count++;

        if (loadContainers(cur->next, q, remWeight - cur->weight)) {
            return true;
        }

        //undo
        t->count--;
        t->remaining += cur->weight;
    }

    //send this truck away and try with the next truck
    if (t->count > 0) {
        q.dequeue();

        if (loadContainers(cur, q, remWeight)) {
            return true;
        }

        //undo
        q.undoDequeue();
    }

    return false;
}

int main() {
    int n, numTrucks, W;

    cout << "Enter number of containers: ";
    cin >> n;
    if (n <= 0 || n > MAX_LOAD) {
        cout << "Number of containers must be between 1 and " << MAX_LOAD << endl;
        return 0;
    }

    Container* head = NULL;
    Container* tail = NULL;
    int totalWeight = 0;

    for (int i = 0; i < n; i++) {
        int id, weight;
        cout << "Enter id and weight of container " << i + 1 << ": ";
        cin >> id >> weight;
        addContainer(head, tail, id, weight);
        totalWeight += weight;
    }

    cout << "Enter number of trucks: ";
    cin >> numTrucks;
    if (numTrucks <= 0 || numTrucks > MAX_TRUCKS) {
        cout << "Number of trucks must be between 1 and " << MAX_TRUCKS << endl;
        return 0;
    }

    cout << "Enter capacity of each truck: ";
    cin >> W;

    Queue q;
    for (int i = 1; i <= numTrucks; i++) {
        Truck t;
        t.id = i;
        t.remaining = W;
        t.count = 0;
        q.enqueue(t);
    }

    cout << endl;
    if (loadContainers(head, q, totalWeight)) {
        for (int i = 0; i < numTrucks; i++) {
            cout << "Truck " << q.trucks[i].id << ": ";
            if (q.trucks[i].count == 0) {
                cout << "no containers";
            }
            for (int j = 0; j < q.trucks[i].count; j++) {
                cout << "C" << q.trucks[i].loaded[j] << " ";
            }
            cout << endl;
        }
    } else {
        cout << "Insufficient truck capacity." << endl;
    }

    //free the list
    while (head != NULL) {
        Container* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}