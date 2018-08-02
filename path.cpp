#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
using namespace std;
void test();
void print_menu();
//Define infinite for the graph
int inf = 1000;
//Define class graph
class Graph { //Adjacency matrix form
	private:
		int num_ver, num_edge; //number of vertices and number of edges
		int **A; //2D array presents the graph in matrix form
		int *prev; //pointer to the previous vertex
	public:
		Graph(int Ver = 2,int Edg = 1);
		~Graph();
		bool is_adjacent(int, int); // check whether each vertices are adjacent to each other
		void add_edge(int u, int v,int w);
		bool find_path(int first_vertex, int last_vertex); // find the path based on the given departure and destination devices
		void print_path(int first_vertex, int last_vertex, char v,int time_input); //compare the time travel input by users against the actual time travel and print out the path found
};
//Constructor
Graph::Graph(int Ver,int Edg) {
	int i, j;
	if (Ver < 2) num_ver = 2;
	else num_ver = Ver;
	A = new int*[num_ver];
	for (i = 0; i < num_ver; ++i)
		A[i] = new int[num_ver];
	for (i = 0; i < num_ver; ++i) {
		for (j = 0; j < num_ver; ++j)
			A[i][j] = inf;
		A[i][i] = 0;
	}

}
//Destructor
Graph::~Graph() {
	for (int i = 0; i < num_ver; ++i)
		delete [] A[i];
	delete [] A;
}

//check the adjacency
bool Graph::is_adjacent(int u, int v) {
	return (A[u][v] != inf);
}

//adds an edge E to the graph G.
void Graph::add_edge(int u, int v, int w) {
	A[u][v] = w ;
}

bool Graph::find_path(int first_vertex, int last_vertex) {
	int *visited = new int[num_ver];
	int *length = new int[num_ver];
	prev = new int[num_ver];
	for (int i = 0; i < num_ver; i++) {
		visited[i] = 1; // mark all vertices as unvisited
		length[i] = inf; //Default distance is infinite
		prev[i] = -1;
	}
	length[first_vertex] = 0;
	while (visited[last_vertex] == 1) { //keep checking while the vertice is unvisited
		int flag = -1;
		int vertex = -1;
		// record the length
		for (int i = 0; i < num_ver; i++) {
			if (visited[i] == 1 && length[i] != inf) {
				flag = length[i];
				vertex = i;
			}
		}
		// mark the vertice as visited
		length[vertex] = flag;
		visited[vertex] = 0;
		for (int i = 0; i < num_ver; i++) {
			//if vertice is unmark
			if (visited[i] == 1 && A[vertex][i] != 0) {
				//if path from vertex is equal to path recorded in length
				if (length[i] == inf || length[i] == length[vertex] + A[vertex][i]) {
					length[i] = length[vertex] + A[vertex][i]; //record the length
					//record the vertice
					prev[i] = vertex;
				}
			}
		}
	}
	delete visited;
	delete length;
	return true;
}

//Print shortest path in record to screen
void Graph::print_path(int first_vertex, int last_vertex, char v, int time_input) {
	int path_len = 0;
	bool pathExists = this->find_path(first_vertex, last_vertex);
	if (pathExists) {
		int k = last_vertex;
		while (k != first_vertex) {
			path_len +=  A[prev[k]][k];
			if(time_input < path_len) {
				cout << "Path not found" << endl;
				print_menu();
				break;
			} else {
				cout << char(k + v) << " <--("<< A[prev[k]][k] <<")-- ";
				k = prev[k];
			}
		}
		cout << char(first_vertex+v)<<endl;
		cout<<"Time travel:"<< path_len<<endl;
	}
}

int main() {
	print_menu();
	char Command;
	do {
		cout << "Command: ";
		cin >> Command;
		Command=tolower(Command);
		switch (Command) {
			case 't':
				test();
				break;
			case 'm':
				print_menu();
				break;
			case 'q':
				break;
			default:
				cerr << "Illegal Command\n";
				print_menu();
		}
	} while (Command != 'q');
}
void test() {
	ifstream fin;
	string fname ;
	int num_ver,num_edge, weight, time_input ; //get data from the file such as number of vertices, edges and time travel
	char v1,v2; //get devices name from the file
	char v, start, end;

	cout << "Due to technical issue, csv file will create errors for the program. Please use the text file provided. Sorry for the inconveniece." << endl;
	cout << "Enter the text file name:" << endl;
	cin >> fname;
	fin.open(fname.c_str());
	if (!fin) {
		cerr << "Unable to open file" << endl;
		exit(0);
	}
	//Read file and add to graph
	fin >> num_ver >> num_edge;
	Graph g(num_ver,num_edge);

	fin >> v1 >> v2 >> weight;
	v = v1;
	for(int i = 1; i < num_edge; i++) {
		v1 -= v;
		v2 -= v;
		g.add_edge(v1,v2, weight);
		fin >> v1 >> v2 >> weight;
	}
	cout << "Please input the departure device: " << endl;
	cin >> start;
	cout << "Please input the destination device: " << endl;
	cin >> end;
	cout << "Please input the travel time between these devices " << endl;
	cin >> time_input;
	start -=v;
	end -=v;
	g.print_path(start,end,v,time_input);
	cout<<endl;
	fin.close();
	g.~Graph();

}
void print_menu() {
	cout << "Commands Available:\n";
	cout << "  t - Do the network path test\n";
	cout << "  m - Print Menu\n";
	cout << "  q - Quit the program\n\n";
}

