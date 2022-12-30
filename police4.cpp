#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// input data
int N, R, T, L;		//semaphores, skippable, half-period, length
vector<int> X;




int leaveTime(int arrival) {
	if((arrival / T) % 2 == 0) return arrival;
	else return (((arrival / T) + 1) * T);
}



int main() {
//  uncomment the following lines if you want to read/write from files
//  ifstream cin("input.txt");
//  ofstream cout("output.txt");

	ios::sync_with_stdio(false);

	////INPUT
    cin >> N >> R >> T >> L;
    X.resize(N + 1);
	X[0] = 0;
    for (int i = 1; i <= N; i++)
        cin >> X[i];

	////ALGORITHM
	vector<int> departures(N + 1);		//departure times at semaphores (updated increasing skips by 1 (dp))
	//init line zero (using 0 skips)
	departures[0] = 0;
	for(int i = 1; i <= N; i++) departures[i] = leaveTime(departures[i-1] + (X[i] - X[i-1]));
	//other lines (using skips)
	for(int i = 1; i <= R; i++) {
		int last = departures[i-1];
		for(int j = i; j <= N; j++) {
			int diff = X[j] - X[j-1];
			int no_skip = leaveTime(departures[j-1] + diff);
			int skip = last + diff;
			last = departures[j];
			departures[j] = min(skip, no_skip);
			if(departures[N] == X[N]) i = j;
		}
		if(departures[N] == X[N]) break;
	}

	////OUTPUT
    cout << departures[N] + (L - X[N]); // print the result
}