#include <iostream>
#include <stdlib.h>

using namespace std;

int main()
{
	int stage = 2;
	cout<<"main"<<endl;
	cin>>stage;
	
	if ( stage == 1 )
	{
		system("g++ -o graphics graphics.cpp imageloader.cpp vec3f.cpp -lglut -lGL -lGLU");
		system("./graphics");
	}
	else if ( stage == 2 )
	{
		system("make -s");
		system("./a");	
	}		
	//cin>>stage;	
}

