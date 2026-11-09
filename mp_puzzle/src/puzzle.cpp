/**
 * @file puzzle.cpp
 * Implementation of puzzle class.
 */
#include "puzzle.h"

#include <queue>
#include <map>

using namespace std;

namespace {
    const array<char, 16> SOLVED = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

    const array<char, 16> ZERO_STATE = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
}

PuzzleState::PuzzleState() {
    state_ = SOLVED;
}

PuzzleState::PuzzleState(const std::array<char, 16> state) {
    state_ = state;
}

std::array<char, 16> PuzzleState::asArray() const {
    return state_;
}

bool PuzzleState::operator==(const PuzzleState &rhs) const {
    return state_ == rhs.state_;
}

bool PuzzleState::operator!=(const PuzzleState &rhs) const {
    return !(*this == rhs);
}

bool PuzzleState::operator<(const PuzzleState &rhs) const {
    return state_ < rhs.state_;
}

PuzzleState PuzzleState::getNeighbor(Direction direction) const {
    int idx = 0;
    for (int i = 0; i < 16; i++) {
        if (state_[i] == 0) {
            idx = i;
            break;
        }
    }

    int row = idx / 4;
    int col = idx % 4;

    int s = -1;

    if (direction == Direction::UP) {
        //up, down
        if (row == 3) {
            return PuzzleState(ZERO_STATE);
        }
        s = idx + 4;
    } else if (direction == Direction::DOWN) {
        //down up
        if (row == 0) {
            return PuzzleState(ZERO_STATE);
        }
        s = idx - 4;
    } else if (direction == Direction::LEFT) {
        //left right
        if (col == 3) {
            return PuzzleState(ZERO_STATE);
        }
        s = idx + 1;
    } else if (direction == Direction::RIGHT) {
        //right left
        if (col == 0) {
            return PuzzleState(ZERO_STATE);
        }
        s = idx - 1;
    }

    array<char, 16> next = state_;

    char tmp = next[idx];
    next[idx] = next[s];
    next[s] = tmp;

    return PuzzleState(next);
}

std::vector<PuzzleState> PuzzleState::getNeighbors() const {
    vector<PuzzleState> result;

    PuzzleState u = getNeighbor(Direction::UP);
    PuzzleState d = getNeighbor(Direction::DOWN);
    PuzzleState l = getNeighbor(Direction::LEFT);
    PuzzleState r = getNeighbor(Direction::RIGHT);

    if (u.asArray() != ZERO_STATE) {
        result.push_back(u);
    }

    if (d.asArray() != ZERO_STATE) {
        result.push_back(d);
    }

    if (l.asArray() != ZERO_STATE) {
        result.push_back(l);
    }

    if (r.asArray() != ZERO_STATE) {
        result.push_back(r);
    }

    return result;
}

int PuzzleState::manhattanDistance(const PuzzleState desiredState) const {
    array<char, 16> goal = desiredState.asArray();
    int dist = 0;

    for (int i = 0; i < 16; i++) {
        char val = state_[i];
        if (val == 0) {
            continue;
        }

        int idx = 0;
        for (int j = 0; j < 16; j++) {
            if (goal[j] == val) {
                idx = j;
                break;
            }
        }

        int r1 = i / 4;
        int c1 = i % 4;
        int r2 = idx / 4;
        int c2 = idx % 4;

        int dr = r1 - r2;
        if (dr < 0) {
            dr = -dr;
        }

        int dc = c1 - c2;
        if (dc < 0) {
            dc = -dc;
        }

        dist += dr + dc;
    }

    return dist;
}

// ————————————————————————————————————————


std::vector<PuzzleState> build(const PuzzleState &startState, const PuzzleState &desiredState, std::map<PuzzleState, PuzzleState> &parent) {
    std::vector<PuzzleState> path;
    PuzzleState cur = desiredState;

    while (cur != startState) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back(startState);

    for (size_t i = 0; i < path.size() / 2; i++) {
        PuzzleState tmp = path[i];
        path[i] = path[path.size() - 1 - i];
        path[path.size() - 1 - i] = tmp;
    }

    return path;
}


// std::vector<PuzzleState> solveBFS(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations) {
//     if (iterations != NULL) {
//         *iterations = 0;
//     }

//     if (startState == desiredState) {
//         std::vector<PuzzleState> path;
//         path.push_back(startState);
//         if (iterations != NULL) {
//             *iterations = 1;
//         }
//         return path;
//     }

//     std::queue<PuzzleState> q;
//     std::map<PuzzleState, bool> visit;
//     std::map<PuzzleState, PuzzleState> parent;

//     q.push(startState);
//     visit[startState] = true;

//     while (!q.empty()) {
//         PuzzleState cur = q.front();
//         q.pop();

//         if (iterations != NULL) {
//             (*iterations)++;
//         }

//         if (cur == desiredState) {
//             return build(startState, desiredState, parent);
//         }

//         std::vector<PuzzleState> n = cur.getNeighbors();
//         for (size_t i = 0; i < n.size(); i++) {
//             if (visit.find(n[i]) == visit.end()) {
//                 visit[n[i]] = true;
//                 parent[n[i]] = cur;
//                 q.push(n[i]);
//             }
//         }
//     }

//     return {};
// }

std::vector<PuzzleState> solveBFS(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations) {
    if (iterations != NULL) {
        *iterations = 0;
    }

    std::map<PuzzleState, PuzzleState> parent;
    std::queue<PuzzleState> q;

    parent[startState] = startState;
    q.push(startState);

    while (!q.empty()) {
        PuzzleState cur = q.front();
        q.pop();

        if (iterations != NULL) {
            *iterations = *iterations + 1;
        }

        if (cur == desiredState) {
            std::vector<PuzzleState> ans;
            PuzzleState temp = desiredState;

            while (!(temp == startState)) {
                ans.push_back(temp);
                temp = parent[temp];
            }
            ans.push_back(startState);

            for (size_t i = 0; i < ans.size() / 2; i++) {
                PuzzleState hold = ans[i];
                ans[i] = ans[ans.size() - 1 - i];
                ans[ans.size() - 1 - i] = hold;
            }

            return ans;
        }

        std::vector<PuzzleState> n = cur.getNeighbors();
        for (size_t i = 0; i < n.size(); i++) {
            if (parent.find(n[i]) == parent.end()) {
                parent[n[i]] = cur;
                q.push(n[i]);
            }
        }
    }

    return std::vector<PuzzleState>();
}

// std::vector<PuzzleState> solveAstar(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations) {
//     if (iterations != NULL) {
//         *iterations = 0;
//     }

//     if (startState == desiredState) {
//         std::vector<PuzzleState> path;
//         path.push_back(startState);
//         if (iterations != NULL) {
//             *iterations = 1;
//         }
//         return path;
//     }

//     std::vector<PuzzleState> open;
//     std::map<PuzzleState, int> g;
//     std::map<PuzzleState, PuzzleState> parent;
//     std::map<PuzzleState, bool> close;

//     open.push_back(startState);
//     g[startState] = 0;

//     while (!open.empty()) {
//         int idx = 0;
//         int bf = g[open[0]] + open[0].manhattanDistance(desiredState);

//         for (size_t i = 1; i < open.size(); i++) {
//             int cf = g[open[i]] + open[i].manhattanDistance(desiredState);

//             if (cf < bf) {
//                 bf = cf;
//                 idx = (int)i;
//             } else if (cf == bf) {
//                 if (open[i] < open[idx]) {
//                     idx = (int)i;
//                 }
//             }
//         }

//         PuzzleState cur = open[idx];

//         open[idx] = open[open.size() - 1];
//         open.pop_back();

//         if (close[cur]) {
//             continue;
//         }

//         close[cur] = true;

//         if (iterations != NULL) {
//             (*iterations)++;
//         }

//         if (cur == desiredState) {
//             std::vector<PuzzleState> path;
//             PuzzleState tmp = desiredState;

//             while (tmp != startState) {
//                 path.push_back(tmp);
//                 tmp = parent[tmp];
//             }
//             path.push_back(startState);

//             for (size_t i = 0; i < path.size() / 2; i++) {
//                 PuzzleState t = path[i];
//                 path[i] = path[path.size() - 1 - i];
//                 path[path.size() - 1 - i] = t;
//             }

//             return path;
//         }

//         std::vector<PuzzleState> n = cur.getNeighbors();
//         for (size_t i = 0; i < n.size(); i++) {
//             PuzzleState next = n[i];

//             if (close[next]) {
//                 continue;
//             }

//             int tg = g[cur] + 1;

//             if (g.find(next) == g.end() || tg < g[next]) {
//                 g[next] = tg;
//                 parent[next] = cur;

//                 bool o = false;
//                 for (size_t j = 0; j < open.size(); j++) {
//                     if (open[j] == next) {
//                         o = true;
//                         break;
//                     }
//                 }

//                 if (!o) {
//                     open.push_back(next);
//                 }
//             }
//         }
//     }

//     return {};
// }

std::vector<PuzzleState> solveAstar(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations) {
    if (iterations != NULL) {
        *iterations = 0;
    }

    if (startState == desiredState) {
        std::vector<PuzzleState> path;
        path.push_back(startState);
        if (iterations != NULL) {
            *iterations = 1;
        }
        return path;
    }

    struct Node {
        PuzzleState state;
        int f;
    };

    struct Compare {
        bool operator()(const Node &a, const Node &b) const {
            if (a.f != b.f) {
                return a.f > b.f;
            }
            return b.state < a.state;
        }
    };

    std::priority_queue<Node, std::vector<Node>, Compare> open;
    std::map<PuzzleState, int> g;
    std::map<PuzzleState, PuzzleState> parent;
    std::map<PuzzleState, bool> close;

    g[startState] = 0;
    open.push({startState, startState.manhattanDistance(desiredState)});

    while (!open.empty()) {
        PuzzleState cur = open.top().state;
        open.pop();

        if (close[cur]) {
            continue;
        }

        close[cur] = true;

        if (iterations != NULL) {
            (*iterations)++;
        }

        if (cur == desiredState) {
            std::vector<PuzzleState> path;
            PuzzleState temp = desiredState;

            while (temp != startState) {
                path.push_back(temp);
                temp = parent[temp];
            }
            path.push_back(startState);

            for (size_t i = 0; i < path.size() / 2; i++) {
                PuzzleState t = path[i];
                path[i] = path[path.size() - 1 - i];
                path[path.size() - 1 - i] = t;
            }

            return path;
        }

        std::vector<PuzzleState> n = cur.getNeighbors();
        for (size_t i = 0; i < n.size(); i++) {
            PuzzleState next = n[i];

            if (close[next]) {
                continue;
            }

            int tg = g[cur] + 1;

            if (g.find(next) == g.end() || tg < g[next]) {
                g[next] = tg;
                parent[next] = cur;
                int f = tg + next.manhattanDistance(desiredState);
                open.push({next, f});
            }
        }
    }

    return {};
}