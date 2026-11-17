#include <iostream>
using namespace std;

const int MAX = 50;

//one operation like +3 or *2
struct Op {
    char type;
    int val;
};

//stack of operations array based
struct Stack {
    Op data[MAX];
    int top;

    Stack() {
        top = -1;
    }

    void push(Op o) {
        top++;
        data[top] = o;
    }

    void pop() {
        top--;
    }

    bool isEmpty() {
        return top == -1;
    }

    //print from bottom to top
    void print() {
        for (int i = 0; i <= top; i++) {
            cout << data[i].type << data[i].val << " ";
        }
        cout << endl;
    }
};

Op ops[10];          //allowed operations
int numOps;
int target;          //Starget
int K;               //max number of operations
long long Mmax;      //overflow bound
int countFound = 0;

//apply an operation on a value
long long applyOp(long long value, Op op) {
    if (op.type == '+') return value + op.val;
    if (op.type == '-') return value - op.val;
    if (op.type == '*') return value * op.val;
    return value / op.val;   //'/' integer division
}

void solve(long long value, int used, Stack& st) {
    //reached the target print the sequence
    if (value == target) {
        countFound++;
        if (st.isEmpty()) {
            cout << "(no operations needed)" << endl;
        } else {
            st.print();
        }
    }

    //already used K operations
    if (used == K) {
        return;
    }

    for (int i = 0; i < numOps; i++) {
        //cant divide by zero
        if (ops[i].type == '/' && ops[i].val == 0) {
            continue;
        }

        long long next = applyOp(value, ops[i]);

        //prune negative or more than Mmax
        if (next < 0 || next > Mmax) {
            continue;
        }

        st.push(ops[i]);            //choose
        solve(next, used + 1, st);
        st.pop();                   //undo
    }
}

int main() {
    long long start;

    cout << "Enter Sstart: ";
    cin >> start;
    cout << "Enter Starget: ";
    cin >> target;
    cout << "Enter K (max operations): ";
    cin >> K;
    if (K < 0 || K > MAX) {
        cout << "K must be between 0 and " << MAX << endl;
        return 0;
    }
    cout << "Enter Mmax: ";
    cin >> Mmax;

    cout << "Enter number of operations (max 10): ";
    cin >> numOps;
    if (numOps <= 0 || numOps > 10) {
        cout << "Number of operations must be between 1 and 10" << endl;
        return 0;
    }

    for (int i = 0; i < numOps; i++) {
        cout << "Enter operation " << i + 1 << " (symbol then number, e.g. + 3): ";
        cin >> ops[i].type >> ops[i].val;
        if (ops[i].type != '+' && ops[i].type != '-' &&
            ops[i].type != '*' && ops[i].type != '/') {
            cout << "Operation must be one of + - * /" << endl;
            return 0;
        }
    }

    cout << endl << "Valid sequences:" << endl;

    Stack st;
    solve(start, 0, st);

    if (countFound == 0) {
        cout << "No sequence found." << endl;
    } else {
        cout << "Total sequences: " << countFound << endl;
    }

    return 0;
}