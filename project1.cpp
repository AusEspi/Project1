#include <iostream>
#include <vector>
#include <queue>

using namespace std;

char puzzle[3][3];
int maxS = 0;
int exp = 0;

struct Node {
    char puzzle[3][3];
    int mov;
    int g;
    int h;
    Node *up, *down, *left, *right;
    Node() : mov(-1), up(nullptr), down(nullptr), left(nullptr), right(nullptr) {}
};

vector<Node> explored;
vector<Node> q;
Node init;

void outPuzzle(char arr[][3]) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            printf("%c ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int misplaced(char arr[][3]) {
    int m = 0;
    int num = 0;

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            num = (j+(3*(i%3)))+1;
            if((num != (int)arr[i][j] - '0') && (num != 9)) {
                m++;
            }
        }
    }

    return m;
}

int manhattan(char arr[][3]) {
    int m = 0;
    int num = 0;
    int x, y = 0;

    for(int a = 1; a < 9; a++) {
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if((a == (int)arr[i][j] - '0') && ((j+(3*(i%3)))+1 != a)) {
                    x = 0;
                    y = 0;
                    num = a;
                    num--;
                    x++;
                    while(num%3) {
                        num--;
                        x++;
                    }
                    x--;
                    y = num/3;

                    num = j - x;
                    if(num < 0) {
                        num *= -1;
                    }
                    m += num;

                    num = i - y;
                    if(num < 0) {
                        num *= -1;
                    }
                    m += num;
                }
            }
        }
    }

    return m;
}

bool move(char (&arr)[3][3], int dir) {
    int x, y = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j  < 3; j++) {
            if(arr[i][j] == '*') {
                x = i;
                y = j;
                break;
            }
        }
    }

    char temp = '*';
    if(!dir) {//up
        if(x-1 < 0) {
            return 0;
        }
        arr[x][y] = arr[x-1][y];//(*arr)
        arr[x-1][y] = temp;
    }
    else if(dir == 1) {//down
        if(x+1 > 2) {
            return 0;
        }
        arr[x][y] = arr[x+1][y];
        arr[x+1][y] = temp;
    }
    else if(dir == 2) {//left
        if(y-1 < 0) {
            return 0;
        }
        arr[x][y] = arr[x][y-1];
        arr[x][y-1] = temp;
    }
    else if(dir == 3) {//right
        if(y+1 > 2) {
            return 0;
        }
        // printf("here1\n");
        arr[x][y] = arr[x][y+1];
        // printf("here2\n");
        arr[x][y+1] = temp;
    }
    else {
        printf("Ivalid Direction\n");
        return 0;
    }

    return 1;
}

bool goal(char arr[3][3]) {
    int num = 0;

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            num = (j+(3*(i%3)))+1;
            if((num != (int)arr[i][j] - '0') && (num != 9)) {
                return 0;
            }
        }
    }

    return 1;
}

bool repeat(char arr[3][3]){
    bool t = 0;
    int n = 0;

    for(int a = 0; a < explored.size(); a++) {
        n = 0;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(arr[i][j] != explored[a].puzzle[i][j]) {
                    t = 1;
                    break;
                }
                else if(arr[i][j] == explored[a].puzzle[i][j]) {
                    n++;
                }
            }
            if(t) {
                // if(a+1 >= explored.size()) {
                //     return 0;
                // }
                t = 0;
                break;
            }
        }
        if(n == 9) {
            return 1;
        }
    }

    for(int a = 0; a < q.size(); a++) {
        n = 0;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                if(arr[i][j] != q[a].puzzle[i][j]) {
                    t = 1;
                    break;
                }
                else if(arr[i][j] == q[a].puzzle[i][j]) {
                    n++;
                }
            }
            if(t) {
                t = 0;
                break;
            }
        }
        if(n == 9) {
            return 1;
        }
    }

    return 0;
}

void expand(Node &node, int h) {
    // printf("Expanding this node: \n");
    // outPuzzle(node->puzzle);
    
    if(node.mov != 0) {
        Node *u = new Node;
        node.up = u;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                node.up->puzzle[i][j] = node.puzzle[i][j];
            }
        }
        if(!move(node.up->puzzle, 0)) {
            delete node.up;
            node.up = nullptr;
        }
        else if(repeat(node.up->puzzle)) {
            delete node.up;
            node.up = nullptr;
        }
        else {
            node.up->mov = 0;
            node.up->g = node.g + 1;
            if(!h) {
                node.up->h = 0;
            }
            else if(h == 1) {
                node.up->h = misplaced(node.up->puzzle);
            }
            else if(h == 2) {
                node.up->h = manhattan(node.up->puzzle);
            }

            q.push_back(*node.up);
        }
    }
    if(node.mov != 1) {
        Node *d = new Node;
        node.down = d;
        // printf("Prob node1:\n");
        // outPuzzle(node.puzzle);
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                node.down->puzzle[i][j] = node.puzzle[i][j];
            }
        }
        // printf("Prob node1 after:\n");
        // outPuzzle(node.puzzle);
        if(!move(node.down->puzzle, 1)) {
            delete node.down;
            node.down = nullptr;
        }
        else if(repeat(node.down->puzzle)) {
            delete node.down;
            node.down = nullptr;
        }
        else {
            // printf("Prob node2:\n");
            // outPuzzle(node.puzzle);
            node.down->mov = 1;
            node.down->g = node.g + 1;
            if(!h) {
                node.down->h = 0;
            }
            else if(h == 1) {
                node.down->h = misplaced(node.down->puzzle);
            }
            else if(h == 2) {
                node.down->h = manhattan(node.down->puzzle);
            }

            // printf("Prob node2: 1\n");
            // outPuzzle(node.puzzle);
            // outPuzzle(node.down->puzzle);
            q.push_back(*node.down);
            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    node.puzzle[i][j] = node.down->puzzle[i][j];
                }
            }
            // printf("Prob node2: 2\n");
            // outPuzzle(node.puzzle);
            // outPuzzle(node.down->puzzle);
            // printf("Element: %d\n", (int)node.puzzle[0][2] - '0');
        }
    }
    if(node.mov != 2) {
        Node *l = new Node;
        node.left = l;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                node.left->puzzle[i][j] = node.puzzle[i][j];
            }
        }
        if(!move(node.left->puzzle, 2)) {
            // printf("Prob node2 after:\n");
            // outPuzzle(node.puzzle);
            delete node.left;
            node.left = nullptr;
        }
        else if(repeat(node.left->puzzle)) {
            delete node.left;
            node.left = nullptr;
        }
        else {
            node.left->mov = 2;
            node.left->g = node.g + 1;
            if(!h) {
                node.left->h = 0;
            }
            else if(h == 1) {
                node.left->h = misplaced(node.left->puzzle);
            }
            else if(h == 2) {
                node.left->h = manhattan(node.left->puzzle);
            }

            q.push_back(*node.left);
        }
    } 
    if(node.mov != 3) {
        Node *r = new Node;
        node.right = r;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                node.right->puzzle[i][j] = node.puzzle[i][j];
            }
        }
        // printf("here: right\n");
        // printf("g: %d h: %d\n", node.g, node.h);
        // outPuzzle(node.puzzle);
        // outPuzzle(node.right->puzzle);
        if(!move(node.right->puzzle, 3)) {
            delete node.right;
            node.right = nullptr;
        }
        else if(repeat(node.right->puzzle)) {
            delete node.right;
            node.right = nullptr;
        }
        else {
            node.right->mov = 3;
            node.right->g = node.g + 1;

            if(!h) {
                node.right->h = 0;
            }
            else if(h == 1) {
                node.right->h = misplaced(node.right->puzzle);
            }
            else if(h == 2) {
                node.right->h = manhattan(node.right->puzzle);
            }

            q.push_back(*node.right);
        }
    }
}

void search(Node &init, int heuristic) {
    q.push_back(init);
    maxS = q.size();
    while(1) {
        if(q.empty()){
            printf("No solution found\n");
            break;
        }
        
        if(q.size() == 1) {
            if(goal(q[0].puzzle)) {
                printf("Found the Goal state!!\n");
                outPuzzle(q[0].puzzle);
                printf("\nThe algorithm expanded %d total nodes\n", exp);
                printf("The max number of nodes in the queue was %d\n", maxS);
                printf("The depth of the goal was %d\n", q[0].g);
                break;
            }
            printf("Expanding this state:\n");
            outPuzzle(q[0].puzzle);
            exp++;
            expand(q.front(), heuristic);
            explored.push_back(q.front());
            q.erase(q.begin());
            if(maxS < q.size()) {
                maxS = q.size();
            }
        }
        else {
            int m = q[0].g + q[0].h; 
            int x = 0;
            for(int i = 0; i < q.size(); i++) {
                printf("F(%d): %d ", i, q[i].g + q[i].h);//G: %d H: %d  //q[i].g, q[i].h,
                if(m > q[i].g + q[i].h) {
                    m = q[i].g + q[i].h;
                    x = i;
                    // printf("X: %d M: %d", x, m);
                }
            }
            printf("\n");

            if(goal(q[x].puzzle)) {
                printf("Found the Goal state!!\n");
                outPuzzle(q[x].puzzle);
                printf("\nThe algorithm expanded %d total nodes\n", exp);
                printf("The max number of nodes in the queue was %d\n", maxS);
                printf("The depth of the goal was %d\n", q[x].g);
                break;
            }
            printf("The best state to expand with g(n) = %d and h(n) = %d: \n", q[x].g, q[x].h);
            outPuzzle(q[x].puzzle);
            exp++;
            expand(q[x], heuristic);
            explored.push_back(q[x]);
            q.erase(q.begin()+x);
            if(maxS < q.size()) {
                maxS = q.size();
            }
        }

        // printf("----------------------\n");
        // int blank;
        // cin >> blank;
        // printf("----------------------\n");
    }
}

int main() {
    int opt = 0;

    while (!opt) {
        printf("\n\n1: for default puzzle or 2: for custom puzzle\n");
        cin >> opt;
        printf("\n");

        if(opt == 1) {
            // puzzle[0][0] = '1';
            // puzzle[0][1] = '2';
            // puzzle[0][2] = '3';
            // puzzle[1][0] = '4';
            // puzzle[1][1] = '5';
            // puzzle[1][2] = '6';
            // puzzle[2][0] = '*';
            // puzzle[2][1] = '8';
            // puzzle[2][2] = '7';
            puzzle[0][0] = '8';
            puzzle[0][1] = '5';
            puzzle[0][2] = '2';
            puzzle[1][0] = '1';
            puzzle[1][1] = '7';
            puzzle[1][2] = '4';
            puzzle[2][0] = '3';
            puzzle[2][1] = '*';
            puzzle[2][2] = '6';
            printf("Default puzzle:\n");
            outPuzzle(puzzle);
        }
        else if(opt == 2){
            printf("Enter your puzzle with a * for the blank\n");

            for(int i = 0; i < 3; i++) {
                for(int j = 0; j < 3; j++) {
                    cin >> puzzle[i][j];
                }
            }

            bool n;
            for(int a = 0; a < 9; a++) {
                n = true;
                for(int i = 0; i < 3; i++) {
                    for(int j = 0; j < 3; j++) {
                        if((int)puzzle[i][j] - '0' == a) {
                            n = false;
                        }
                        else if(!a && puzzle[i][j] == '*') {
                            n = false;
                        }
                    }
                }
                if(n) {
                    // printf("%d\n", a);
                    break;
                }
            }

            if(n) {
                opt = 0;
                printf("Invalid puzzle\n");
            }
            else {
                printf("Your puzzle is:\n");
                outPuzzle(puzzle);
                printf("\n");
            }
        }
        else {
            opt = 0;
            printf("Invalid entry\n");
        }
    }

    opt = 0;
    while(!opt) {
        printf("\nEnter the algorithm:\n");
        printf("1. Uniform Cost Search\n");
        printf("2. A* w/ Misplaced Heuristic\n");
        printf("3. A* w/ Euclidian Heuristic\n");
        cin >> opt;
        printf("\n");

        copy(&puzzle[0][0], &puzzle[0][0]+3*3, &init.puzzle[0][0]);
        init.g = 0;

        if(opt == 1){
            search(init, 0);
        }
        else if(opt == 2){
            search(init, 1);
        }
        else if(opt == 3){
            search(init, 2);
        }
        else {
            opt = 0;
            printf("Invalid entry\n");
        }
    }

    // while(!q.empty()) {
    //     int max = q[0].g;
    //     int x = 0;
    //     for(int i = 0; i < q.size(); i++) {
    //         if(max <= q[i].g) {
    //             max = q[i].g;
    //             x = i;
    //         }
    //     }

    //     delete q[x].up;
    //     q[x].up = nullptr;
    //     delete q[x].down;
    //     q[x].down = nullptr;
    //     delete q[x].left;
    //     q[x].left = nullptr;
    //     delete q[x].right;
    //     q[x].right = nullptr;

    //     q.erase(q.begin()+x);
    // }

    return 0;
}