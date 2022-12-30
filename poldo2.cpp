/*
struttura con booleano, valori ripetuti messi nello stesso nodo dell'albero
*/

#include <iostream>
#include <queue>
#include <ctime>
using namespace std;


#define N_MAX 10000 

int N;
int P[N_MAX];



struct Node {		//AVL
	int p;
	int max;
	int height;
	bool others;	//if max comes from others
	Node *left;
	Node *right;
	Node *parent;
};
typedef Node *Tree;

//DEBUG
void printTree(Tree T) {
	if(T != NULL) {
		queue<Tree> level;
		level.push(T);
		printf("(%d,%d,%d)\t", T->p, T->max, T->height);
		Tree new_line = T;
		bool pointerInNewLine = true;
		while(!level.empty()) {
			Tree tmp = level.front();
			level.pop();
			if(tmp == new_line) {
				if(pointerInNewLine) cout << endl;
				if(tmp->left == NULL && tmp->right == NULL) {
					new_line = level.front();
					pointerInNewLine = false;
				}
				else {
					pointerInNewLine = true;
					if(tmp->left != NULL) new_line = tmp->left;
					else if(tmp->right != NULL) new_line = tmp->right;
				}
			}
			if(tmp->left != NULL) {
				level.push(tmp->left);
				printf("(%d,%d,%d)\t", tmp->left->p, tmp->left->max, tmp->left->height);
			} else printf("( , , )\t");
			if(tmp->right != NULL) {
				level.push(tmp->right);
				printf("(%d,%d,%d)\t", tmp->right->p, tmp->right->max, tmp->right->height);
			} else printf("( , , )\t");
		}
	}
	cout << "\n\n";
}
//
void deleteTree(Tree t) {
	if(t == NULL) return;
	else {
		deleteTree(t->left);
		deleteTree(t->right);
		delete t;
	}
}


Tree newNode(int p, int mx, Tree parent) {
	Tree t = new Node;
	t->p = p;
	t->max = mx;
	t->height = 1;
	t->others = false;
	t->parent = parent;
	if(parent != NULL) {
		if(parent->p <= p) parent->right = t;
		else parent->left = t;
	}
	t->left = NULL;
	t->right = NULL;
	return t;
}
Tree leftRotation(Tree t) {
	Tree new_T = t->right;

	t->right = new_T->left;
	if(t->right != NULL) {
		t->right->parent = t;
		t->max = max(t->max, t->right->max);
	}

	new_T->parent = t->parent;
	if(t->parent != NULL) {
		if(t->parent->left == t) t->parent->left = new_T;
		else t->parent->right = new_T;
	}
	new_T->left = t;
	t->parent = new_T;

	int left_h = 0, right_h = 0;
	if(t->left != NULL) left_h = t->left->height;
	if(t->right != NULL) right_h = t->right->height;
	t->height = max(left_h, right_h) + 1;

	int new_left_h = t->height, new_right_h = 0;
	if(new_T->right != NULL) new_right_h = new_T->right->height;
	new_T->height = max(new_left_h, new_right_h) + 1;

	return new_T;
}
Tree rightRotation(Tree t) {
	Tree new_T = t->left;

	t->left = new_T->right;
	if(t->left != NULL) t->left->parent = t;

	new_T->parent = t->parent;
	if(t->parent != NULL) {
		if(t->parent->left == t) t->parent->left = new_T;
		else t->parent->right = new_T;
	}
	new_T->right = t;
	t->parent = new_T;

	int left_h = 0, right_h = 0;
	if(t->left != NULL) left_h = t->left->height;
	if(t->right != NULL) left_h = t->right->height;
	t->height = max(left_h, right_h) + 1;

	int new_left_h = 0, new_right_h = t->height;
	if(new_T->left != NULL) new_left_h = new_T->left->height;
	new_T->height = max(new_left_h, new_right_h) + 1;

	new_T->max = max(new_T->max, t->max);

	return new_T;
}

Tree updateHeights(Tree t) {
	if(t == NULL) return t;
	else {
		int left_h = 0, right_h = 0;
		if(t->left != NULL) left_h = t->left->height;
		if(t->right != NULL) right_h = t->right->height;
		
		if(abs(left_h - right_h) > 1) {
			if(left_h > right_h) {
				if(t->left->left == NULL || (t->left->right != NULL && t->left->right->height > t->left->left->height)) leftRotation(t->left);
				//if left_h > right_h then t->left != NULL
				return rightRotation(t);
			} else {
				if(t->right->right == NULL || (t->right->left != NULL && t->right->left->height > t->right->right->height)) rightRotation(t->right);
				//if left_h < right_h then t->right != NULL
				return leftRotation(t);
			}
		}
		else {
			t->height = max(left_h, right_h) + 1;
			if(t->parent != NULL) return updateHeights(t->parent);
			else return t;
		}
	}
}

void updateMax(Tree t, int new_max, int p_max) {
	if(t == NULL) return;
	else {
		if(t->p <= p_max && t->max < new_max) {
			t->max = new_max;
			t->others = true;
		}
		updateMax(t->parent, new_max, p_max);
	}
}


Tree insert(Tree T, int p, int &inserted_mx)
{
	int accumulated_mx = 0;
	Tree tmp = T;
	Tree next_pos = T;

	while(next_pos != NULL && next_pos->p != p) {
		tmp = next_pos;
		if(p < next_pos->p) {
			accumulated_mx = max(accumulated_mx, next_pos->max);
			next_pos = next_pos->left;
		}
		else next_pos = next_pos->right;					//if p > next_pos->p
	}

	accumulated_mx++;
	if(next_pos == NULL) {
		Tree new_p = newNode(p, accumulated_mx, tmp);
		if(T == NULL) T = new_p;
		else {
			updateMax(new_p->parent, accumulated_mx, p);
			Tree rotated_T = updateHeights(new_p);
			if(rotated_T->parent == NULL) T = rotated_T;		//if root was rotated
		}
	}
	else {
		if(next_pos->others && next_pos->max + 1 > accumulated_mx) accumulated_mx = next_pos->max + 1;
		if(accumulated_mx >= next_pos->max) {
			next_pos->max = accumulated_mx;
			next_pos->others = false;
			updateMax(next_pos->parent, accumulated_mx, p);
		}
	}

	inserted_mx = accumulated_mx;
	return T;
}



int maxSandwiches() {
	int mx = 0;
	Tree cache = insert(NULL, P[0], mx);

	for(int i = 1; i < N; i++) {
		int inserted_mx;
		cache = insert(cache, P[i], inserted_mx);
		mx = max(mx, inserted_mx);
		//printTree(cache);
	}

	deleteTree(cache);

	return mx;
}





int main() {

	ios_base::sync_with_stdio(false);

	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	cin >> N;
	for(int i = 0; i < N; i++) cin >> P[i];
	
	cout << maxSandwiches();
}