#include <bits/stdc++.h>

using namespace std;

class Food {
public:
  int index;
  int p, f, s, v, c;
  int totalNutr;
  
  Food(int _i=0, int _p=0, int _f=0, int _s=0, int _v=0, int _c=0) : index(_i), p(_p), f(_f), s(_s), v(_v), c(_c) {
    totalNutr = p+f+s+v;
  }

  Food& operator+= (const Food& rhs) {
    p += rhs.p;
    f += rhs.f;
    s += rhs.s;
    v += rhs.v;
    c += rhs.c;
    totalNutr += rhs.totalNutr;
    
    return *this;
  }

  const Food operator+ (const Food& other) {
    Food result = *this;
    result += other;
    
    return result;
  }

  bool operator< (const Food& rhs) {
    return (p<rhs.p) || (f<rhs.f) || (s<rhs.s) || (v<rhs.v);
  }

  bool operator<= (const Food& rhs) {
    return (p<=rhs.p) || (f<=rhs.f) || (s<=rhs.s) || (v<=rhs.v);
  }

  const bool isFasterThan(const Food& rhs) const {
    return index < rhs.index;
  }

  const bool isExpensiveThan(const Food& rhs) const {
    return c > rhs.c;
  }

  const bool isBetterThan(const Food& rhs) const {
    if(c != rhs.c) return (c < rhs.c);
    else if(totalNutr != rhs.totalNutr) return (totalNutr > rhs.totalNutr);
    return (index < rhs.index);
  }
};

bool foodIndexComp (const Food& lhs, const Food& rhs) {
    return lhs.isFasterThan(rhs);
}

bool foodPriorityComp (const Food& lhs, const Food& rhs) {
    return lhs.isBetterThan(rhs);
}

int N;
vector<Food> foods;
Food minFood;
Food currentAns;
vector<int> visited;
vector<int> ans;

void dfs(int foodOrder, Food trace) {
  for(int i=foodOrder+1; i<N; i++) {
    Food nextFood = trace + foods[i];
    if(nextFood.isExpensiveThan(currentAns)) {
      break;
    }

    visited.push_back(foods[i].index);
    if(nextFood < minFood) {
      dfs(i, nextFood);
    } else {
      if(nextFood.isBetterThan(currentAns)) {
        ans = visited;
        currentAns = nextFood;
      }
    }
    visited.pop_back();
  }
}

int main(){
  ifstream in("food.inp");
  ofstream out("food.out");

  in >> N;
  int prot, fat, carb, vit;
  in >> prot >> fat >> carb >> vit;
  minFood = Food(0, prot, fat, carb, vit, 0);

  foods.resize(N);
  for(int i=0; i<N; i++) {
    int prot, fat, carb, vit, cost;
    in >> prot >> fat >> carb >> vit >> cost;
    foods[i] = Food(i+1, prot, fat, carb, vit, cost);
  }

  sort(foods.begin(), foods.end(), foodPriorityComp);
  
  int i=0;
  while(currentAns < minFood) {
    currentAns += foods[i];
    ans.push_back(foods[i].index);
    i++;
  }

  Food emptyFood(0,0,0,0,0,0);
  dfs(-1, emptyFood);

  sort(ans.begin(), ans.end());

  for(int i : ans) {
    out << i << " ";
  }
  out << "\n";

  in.close();
  out.close();

  return 0;
}