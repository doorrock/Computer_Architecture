#include<iostream>
#include<bitset>
#include<fstream>
#include<string>
using namespace std;

int dataBox_0[1000]; //cache 0
int dataBox_1[500][2];
bool valid_0[1000];
bool valid_1[500][2];
bool write[500][2];
int used[500];
/*
void init(){
	for(int i=0;i<1000;i++){
		dataBox_0[i]=-1;
	}
	for(int i=0;i<500;i++){
		dataBox_1[i][0]=-1;
		dataBox_1[i][1]=-1;
	}
	return;
}*/

int getIndex(int type, string str){
	string s;
	if(type == 0){
		s = str.substr(18, 10);
		bitset<10> pp(s);
		return pp.to_ulong();
	}
	else if(type == 1){
		s = str.substr(17, 9);
		bitset<9> pp(s);
		return pp.to_ulong();
	}
}

int getOffset(int type, string str){
	string s;
	if(type == 0){
		s = str.substr(28, 4);
		bitset<4> pp(s);
		return pp.to_ulong();
	}
	else if(type == 1){
		s = str.substr(26, 6);
		bitset<6> pp(s);
		return pp.to_ulong();
	}
}

int getTag(int type, string str){
	string s;
	if(type == 0){
		s = str.substr(0, 18);
		bitset<18> pp(s);
		return pp.to_ulong();
	}
	else if(type == 1){
		s = str.substr(0, 17);
		bitset<17> pp(s);
		return pp.to_ulong();
	}
}

int main(int argc, char **argv){
	ifstream file_input;
	char LoadStore;
	string str;
	
 	string num = argv[2];
  	string s = "trace.txt";
 	s.insert(5,num);
	file_input.open(s);
	//file_input.open(argv[1]);
	int type = atoi(argv[1]);
	//cin >> type;
	

	int access = 0;
	int miss = 0;
	int hit = 0;
	int writeCnt = 0;
	
	string temp;
	while(!file_input.eof()){
	//while(!getline(file_input,temp)){
		file_input >> LoadStore >> str;
		//cout << LoadStore << ' '<< str << '\n';
		if(str.length() > 8) continue;
		bitset<32> bit(stoul(str,0,16));
		str = bit.to_string();

		if(LoadStore == 'L'){
			int index = getIndex(type, str);
			int offset = getOffset(type, str);
			int tag = getTag(type, str);
			
			if(type == 0){
				if(valid_0[index]==true){
					if(dataBox_0[index] == tag){
						hit++;
					}
					else{
						miss++;
						dataBox_0[index] = tag;
					}
				}
				else{
					valid_0[index]=true;
					miss++;
					dataBox_0[index] = tag;
				}
			}
			
			if(type == 1){		
				if(valid_1[index][0]==false){
					miss++;
					dataBox_1[index][0] = tag;
					valid_1[index][0]=true;
					used[index] = 1;
				}
				else{
					if(dataBox_1[index][0] == tag){
						used[index] = 1;
					}
					else{
						if(valid_1[index][1]==false){
							miss++;
							dataBox_1[index][1] = tag;
							valid_1[index][1]=true;
							used[index] = 0;
						}
						
						else{
							if(dataBox_1[index][1] == tag){
								used[index] = 0;
							}
							else{
								miss++;
								dataBox_1[index][used[index]] = tag;
								if(write[index][used[index]]==true){
									writeCnt++;
									write[index][used[index]]=false;
								}
								used[index] = 1 - used[index];
							}
						}
					}
				}
			}
			
		}
		
		if(LoadStore == 'S'){
			
			int index = getIndex(type, str);
			int offset = getOffset(type, str);
			int tag = getTag(type, str);
			
			if(type == 0){
				if(valid_0[index]==true){
					if(dataBox_0[index] != tag){
						miss++;
					}
				}
				else miss++;
				
				writeCnt++;
			}
			
			else if(type == 1){
				if(valid_1[index][0] == false){
					miss++;
					dataBox_1[index][0] = tag;
					valid_1[index][0]=true;
					used[index] = 1;
					
					write[index][0] = true;
				}
				else{
					if(dataBox_1[index][0] == tag){
						used[index] = 1;
						write[index][0] = true;
					}
					else{
						if(valid_1[index][1]==false){
							miss++;
							dataBox_1[index][1] = tag;
							valid_1[index][1] = true;
							used[index] = 0;
							
							write[index][1] = true;
						}
						
						else{
							if(dataBox_1[index][1] == tag){
								used[index] = 0;
								write[index][1] = true;
							}
							else{
								miss++;
								dataBox_1[index][used[index]] = tag;
								if(write[index][used[index]] == true){
									writeCnt++;
								}
								write[index][used[index]] = true;
								used[index] = 1 - used[index];
							}
						}
					}
				}
			}
		}
	}
	cout<<miss<<" "<<writeCnt<<'\n';
	file_input.close();
}
