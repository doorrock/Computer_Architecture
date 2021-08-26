/* add, addu, and, div, divu, jalr, jr, mfhi, mflo, mthi, mtlo, mult, multu, nor,
or, sll, sllv, slt, sltu, sra, srav, srl, srlv, sub, subu, syscall, xor, addi, addiu,
andi, beq, bne, lb, lbu, lh, lhu, lui, lw, ori, sb, slti, sltiu, sh, sw, xori, j, jal
*/
#include<iostream>
#include<string>
#include<bitset>
#include<iomanip>
#include<fstream>
#include<vector>
using namespace std;
int res[32];
int res_HI;
int res_LO;
int pc;
char dataBox[65537];
bool error_flag = false;
bool read_flag = false;
bool unknown_flag = false;
void init_inst() {
	error_flag = false;
	unknown_flag = false;
	for (int i = 0;i < 32;i++) res[i] = 0;
	pc = 0;
	return;
}
void init_data() {
	for(int i=0; i<10001 ;i++){
		dataBox[i] = -1 ;
	}
}
union ubox_int
{
	int a;
	unsigned char arr[4];
}Ubox[65537];

union ubox_short
{
	short a;
	unsigned char arr[2];
};

bool dataBcheck(int i, int k){
	if (i < 0 || i > 0x10000){
		cout << "Memory address out of range: 0x";
		cout << setw(8) << setfill('0') << hex << i + 0x10000000 << " " << dec << "\n";
		return true;
	}
	else if (i + k > 0x10000){
		cout << "Memory address out of range: 0x10010000\n";
		return true;
	}
	else if (i % k != 0){
		cout << "Memory alignment error: 0x";
		cout << setw(8) << setfill('0') << hex << i + 0x10000000 << " " << dec << "\n";
		return true;
	}
	else return false;
}
int r_rs(string s) {
	string temp = s.substr(6, 5);
	bitset<5> pp(temp);
	return pp.to_ulong();
}
int r_rt(string s) {
	string temp = s.substr(11, 5);
	bitset<5> pp(temp);
	return pp.to_ulong();
}
int r_rd(string s) {
	string temp = s.substr(16, 5);
	bitset<5> pp(temp);
	return pp.to_ulong();
}
int imm(string s) {
	string temp = s.substr(16, 16);
	bitset<16> pp(temp);
	short num = (short)(pp.to_ulong() & 0xFFFF);
	return num;
}
int sa(string s) {
	string temp = s.substr(21, 5);
	bitset<5> pp(temp);
	return pp.to_ulong();
}
int offset(string s) {
	string temp = s.substr(16, 16);
	bitset<16> pp(temp);
	short num = (short)(pp.to_ulong() & 0xFFFF);
	return num;
}
int target(string s) {
	string temp = s.substr(6, 26);
	bitset<26> pp(temp);
	if (pp[25]) {
		pp.flip();
		return -1 * (pp.to_ulong() + 1);
	}
	else return pp.to_ulong();
}
void rFormat(string s) {
	string temp = s.substr(26, 6);
	if (temp == "100000") { 
		if(read_flag) cout << "add $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); 
		else{
			res[r_rd(s)] = res[r_rs(s)] + res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "100001") {
		if(read_flag) cout << "addu $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); 
		else{
			res[r_rd(s)] = res[r_rs(s)] + res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "100100") {
		if(read_flag) cout << "and $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); 
		else{
			res[r_rd(s)] = res[r_rs(s)] & res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "100111") {
		if(read_flag) cout << "nor $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); 
		else{
			res[r_rd(s)] = ~(res[r_rs(s)] | res[r_rt(s)]);
			pc += 4;
		}
	}
	else if (temp == "100101") {
		if(read_flag) cout << "or $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s);
		else{
			res[r_rd(s)] = res[r_rs(s)] | res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "101010") { 
		if(read_flag) cout << "slt $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); 
		else{
			res[r_rd(s)] = res[r_rs(s)] < res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "101011") { 
		if(read_flag) cout << "sltu $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); 
		else{
			res[r_rd(s)] = (unsigned)res[r_rs(s)] < (unsigned)res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "100010") {
		if(read_flag) cout << "sub $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s);
		else{
			res[r_rd(s)] = res[r_rs(s)] - res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "100011") {
		if(read_flag) cout << "subu $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s);
		else{
			res[r_rd(s)] = res[r_rs(s)] - res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "100110") { 
		if(read_flag) cout << "xor $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); 
		else{
			res[r_rd(s)] = res[r_rs(s)] ^ res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "000000") {
		if(read_flag) cout << "sll $" << r_rd(s) << ", $" << r_rt(s) << ", " << sa(s); 
		else{
			res[r_rd(s)] = res[r_rt(s)] << sa(s);
			pc += 4;
		}
	}
	else if (temp == "000100") {
		if(read_flag) cout << "sllv $" << r_rd(s) << ", $" << r_rt(s) << ", $" << r_rs(s);
		else{
			res[r_rd(s)] = res[r_rt(s)] << res[r_rs(s)];
			pc += 4;
		}
	}
	else if (temp == "000011") {//sign_extension automatically
		if(read_flag) cout << "sra $" << r_rd(s) << ", $" << r_rt(s) << ", " << sa(s);
		else{
			res[r_rd(s)] = res[r_rt(s)] >> sa(s);
			pc += 4;
		}
	}
	else if (temp == "000111") { 
		if(read_flag) cout << "srav $" << r_rd(s) << ", $" << r_rt(s) << ", $" << r_rs(s);
		else{
			res[r_rd(s)] = res[r_rt(s)] >> res[r_rs(s)];
			pc += 4;
		}
	}
	else if (temp == "000010") {//zero_extension
		if(read_flag) cout << "srl $" << r_rd(s) << ", $" << r_rt(s) << ", " << sa(s);
		else{
			res[r_rd(s)] = (unsigned)res[r_rt(s)] >> sa(s);
			pc += 4;
		}
	}
	else if (temp == "000110") {
		if(read_flag) cout << "srlv $" << r_rd(s) << ", $" << r_rt(s) << ", $" << r_rs(s); 
		else{
			res[r_rd(s)] = (unsigned)res[r_rt(s)] >> res[r_rs(s)];
			pc += 4;
		}
	}
	else if (temp == "011010") { 
		if(read_flag) cout << "div $" << r_rs(s) << ", $" << r_rt(s);
		else{
			res_LO = res[r_rs(s)] / res[r_rt(s)];
			res_HI = res[r_rs(s)] % res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "011011") { 
		if(read_flag) cout << "divu $" << r_rs(s) << ", $" << r_rt(s); 
		else{
			res_LO = (unsigned)res[r_rs(s)] / (unsigned)res[r_rt(s)];
			res_HI = (unsigned)res[r_rs(s)] % (unsigned)res[r_rt(s)];
			pc += 4;
		}
	}
	else if (temp == "010000") { 
		if(read_flag) cout << "mfhi $" << r_rd(s);
		else{
			res[r_rd(s)] = res_HI;
			pc += 4;
		}
	}
	else if (temp == "010010") { 
		if(read_flag) cout << "mflo $" << r_rd(s); 
		else{
			res[r_rd(s)] = res_LO;
			pc += 4;
		}
	}
	else if (temp == "010001") {
		if(read_flag) cout << "mthi $" << r_rs(s); 
		else{
			res_HI = res[r_rs(s)];
			pc += 4;
		}
	}
	else if (temp == "010011") { 
		if(read_flag) cout << "mtlo $" << r_rs(s); 
		else{
			res_LO = res[r_rs(s)];
			pc += 4;
		}
	}
	else if (temp == "011000") { 
		if(read_flag) cout << "mult $" << r_rs(s) << ", $" << r_rt(s); 
		else{
			long long A = res[r_rs(s)];
			long long B = res[r_rt(s)];
			long long C = 32;
			res_LO = ((A * B) << C) >> C;
			res_HI = (A * B) >> C;
			pc += 4;
		}
	}
	else if (temp == "011001") {
		if(read_flag) cout << "multu $" << r_rs(s) << ", $" << r_rt(s);
		else{
			long long A = res[r_rs(s)];
			long long B = res[r_rt(s)];
			long long C = 32;
			long long UA = (unsigned)A;
			long long UB = (unsigned)B;
			res_LO = (UA * UB << C) >> C;
			res_HI = (UA * UB) >> C;
			pc += 4;
		}
	}
	else if (temp == "001001") { 
		if (read_flag) {
			//if (r_rd(s) == 31) cout << "jalr $" << r_rs(s);
			//else cout << "jalr $" << r_rd(s) << ", $" << r_rs(s);
			cout << "jalr $" << r_rd(s) << ", $" << r_rs(s);
		}
		else{
			res[r_rd(s)] = pc ;
			pc = res[r_rs(s)] ;
		}
	}
	else if (temp == "001000") { 
		if(read_flag) cout << "jr $" << r_rs(s); 
		else{
			pc = res[r_rs(s)];
		}
	}
	else if (temp == "001100") { 
		if(read_flag) cout << "syscall "; 
		else{
			if(res[2]==1) cout << res[4];
			
			else if(res[2]==4){
				int p = res[4] - 0x10000000;
				int i = 0;
				for(; p + i < 0x00010000 && dataBox[p + i] != '\n'; i++);
				
				if(p + i == 0x00010001){
					cout << "Memory address out of range: 0x10010000\n";
					
					error_flag = true;
				}
				else{
					string str = dataBox + p;
					cout << str;
				}
				
			}
			else if(res[2]==10){
				cout << "EXIT syscall" << '\n';
				error_flag  = true;
			}
			else{
				cout << "Invalid syscall" << '\n';
				error_flag  = true;
				//exit(0);
			}
			pc += 4;
		}
	}
	else { if(read_flag) cout << "unknown instruction ";
			else{unknown_flag = true; pc += 4; }}
}

void change(string s) {
	string temp = s.substr(0, 6);
	if (temp == "000000") rFormat(s);
	else if (temp == "001000") {
		if(read_flag) cout << "addi $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s);
		else{
			res[r_rt(s)] = res[r_rs(s)] + imm(s);
			pc += 4;
		}
	}
	else if (temp == "001001") { 
		if(read_flag) cout << "addiu $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); 
		else{
			res[r_rt(s)] = (unsigned)res[r_rs(s)] + (unsigned)imm(s);
			pc += 4;
		}
	}
	else if (temp == "001100") { //andi is not sign_extension
		if(read_flag) cout << "andi $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); 
		else{
			res[r_rt(s)] = res[r_rs(s)] & ((unsigned)(imm(s) << 16) >> 16);
			//res[r_rt(s)] = res[r_rs(s)] & imm(s);
			pc += 4;
		}
	}
	else if (temp == "001111") { //imm shift left by 4 bit
		if(read_flag) cout << "lui $" << r_rt(s) << ", " << imm(s); 
		else{
			res[r_rt(s)] = res[r_rs(s)] + (imm(s)<<16);
			pc += 4;
		}
	}
	else if (temp == "001101") {//ori is not sign_extension
		if(read_flag) cout << "ori $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); 
		else{
			res[r_rt(s)] = res[r_rs(s)] | ((unsigned)(imm(s) << 16) >> 16);
			pc += 4;
		}
	}
	else if (temp == "001010") { //imm is sign_extension automatically
		if(read_flag) cout << "slti $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s);
		else{
			res[r_rt(s)] = res[r_rs(s)] < imm(s);
			pc += 4;
		}
	}
	else if (temp == "001011") { //compare with unsigned
		if(read_flag) cout << "sltiu $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); 
		else{
			res[r_rt(s)] = (unsigned)res[r_rs(s)] < (unsigned)imm(s);
			pc += 4;
		}
	}
	else if (temp == "001110") { //xori is not sign_extension
		if(read_flag) cout << "xori $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); 
		else{
			res[r_rt(s)] = res[r_rs(s)] ^ ((unsigned)(imm(s)<<16)>>16);
			pc += 4;
		}
	}
	else if (temp == "000100") { 
		if(read_flag) cout << "beq $" << r_rs(s) << ", $" << r_rt(s) << ", " << offset(s); 
		else{
			if(res[r_rs(s)] == res[r_rt(s)]) pc = pc + 4 + offset(s)*4;
			else pc += 4;
		}
	}
	/*else if (temp == "000001") {
		string t = s.substr(11, 5);
		if (t == "00001") { if(read_flag) cout << "bgez $" << r_rs(s) << ", " << offset(s); }
		else if (t == "10001") { if(read_flag) cout << "bgezal $" << r_rs(s) << ", " << offset(s); }
		else if (t == "00000") { if(read_flag) cout << "bltz $" << r_rs(s) << ", " << offset(s); }
		else if (t == "10000") { if(read_flag) cout << "bltzal $" << r_rs(s) << ", " << offset(s); }
	}*//*
	else if (temp == "000111") { if(read_flag) cout << "bgtz $" << r_rs(s) << ", " << offset(s); }
	else if (temp == "000110") { if(read_flag) cout << "blez $" << r_rs(s) << ", " << offset(s); }*/
	else if (temp == "000101") { 
		if(read_flag) cout << "bne $" << r_rs(s) << ", $" << r_rt(s) << ", " << offset(s); 
		else{
			if(res[r_rs(s)] != res[r_rt(s)]) pc = pc + 4 + offset(s)*4;
			else pc += 4;
		}
	}
	else if (temp == "000010") { 
		if(read_flag) cout << "j " << target(s);
		else{
			pc = target(s) << 2;
		}
	}
	else if (temp == "000011") { 
		if(read_flag) cout << "jal " << target(s); 
		else{
			res[31] = pc + 4;
			pc = target(s) << 2;
		}
	}
	/*else if (temp == "010000") {
		string t = s.substr(6, 5);
		if (t == "00000") { if(read_flag) cout << "mfc0 $" << r_rt(s) << ", $" << r_rd(s); }
		else if (t == "00100") { if(read_flag) cout << "mtc0 $" << r_rt(s) << ", $" << r_rd(s); }
	}*/
	else if (temp == "100000") { 
		if(read_flag) cout << "lb $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; 
		else{
			int i = offset(s) + res[r_rs(s)] - 0x10000000;
			error_flag = dataBcheck(i, 1);
			if(!error_flag){
				res[r_rt(s)] = *((char*)&dataBox[i]);
			}
			
			pc += 4;
			
		}
	}
	else if (temp == "100100") { 
		if(read_flag) cout << "lbu $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")";
		else{
			int i = offset(s) + res[r_rs(s)] - 0x10000000;
			error_flag = dataBcheck(i, 1);
			if(!error_flag) {
				int p = *((char*)&dataBox[i]);
				res[r_rt(s)] = (unsigned)(p << 24) >> 24 ;
			}
			
			pc += 4;
		}
	}
	else if (temp == "100001") { 
		if(read_flag) cout << "lh $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; 
		else{
			int i = offset(s) + res[r_rs(s)] - 0x10000000;
			error_flag = dataBcheck(i, 2);
			if(!error_flag) {
				ubox_short p;
				p.a = *((short*)&dataBox[i]);
				swap(p.arr[0], p.arr[1]);
				res[r_rt(s)] = p.a;
			}
			
			pc += 4;
		}
	}
	else if (temp == "100101") { 
		if(read_flag) cout << "lhu $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; 
		else{
			int i = offset(s) + res[r_rs(s)] - 0x10000000;
			error_flag = dataBcheck(i, 2);
			if(!error_flag) {
				ubox_short p;
				p.a = *((short*)&dataBox[i]);
				swap(p.arr[0], p.arr[1]);
				res[r_rt(s)] = (unsigned)(p.a << 16) >> 16;
			}
			
			pc += 4;
		}
	}
	else if (temp == "100011") { 
		if(read_flag) cout << "lw $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; 
		else{
			int i = offset(s) + res[r_rs(s)] - 0x10000000;
			error_flag = dataBcheck(i, 4);
			if(!error_flag) {
				ubox_int p;
				p.a = *((int*)&dataBox[i]);
				swap(p.arr[1], p.arr[2]);
				swap(p.arr[0], p.arr[3]);
				res[r_rt(s)] = p.a;
			}
			
			pc += 4;
		}
	}
	else if (temp == "101000") { 
		if(read_flag) cout << "sb $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; 
		else{
			int i = offset(s) + res[r_rs(s)] - 0x10000000;
			error_flag = dataBcheck(i, 1);
			if(!error_flag){
				*((char*)&dataBox[i]) = res[r_rt(s)];
			}
			
			pc += 4;
		}
	}
	else if (temp == "101001") { 
		if(read_flag) cout << "sh $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; 
		else{
			int i = offset(s) + res[r_rs(s)] - 0x10000000;
			error_flag = dataBcheck(i, 2);
			if(!error_flag) {
				ubox_short p;
				p.a = res[r_rt(s)];
				swap(p.arr[0], p.arr[1]);
				*((short*)&dataBox[i]) = p.a;
			}
			
			pc += 4;
		}
	}
	else if (temp == "101011") {
		if(read_flag) cout << "sw $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")";
		else{
			int i = offset(s) + res[r_rs(s)]- 0x10000000;
			error_flag = dataBcheck(i, 4);
			if(!error_flag) {
				ubox_int p;
				p.a = res[r_rt(s)];
				swap(p.arr[1], p.arr[2]);
				swap(p.arr[0], p.arr[3]);
				*((int*)&dataBox[i]) = p.a;
			}	
			
			pc += 4;
		}
	}
	else { if(read_flag) cout << "unknown instruction";
			else{unknown_flag = true; pc += 4; }}
}

int main(void)
{
	int n=-1;
	while (1) {
		cout << "mips-sim> ";
		char* fileName = new char[100];
		string command;
		cin >> command;
		if (command == "read") {
			cin >> fileName;
			ifstream fs;
			fs.open(fileName, ios::binary);
			fs.seekg(0, ios::end);
			int sz = fs.tellg();
			n = sz / 4;
			fs.seekg(0, ios::beg);

			string tlr;
			read_flag = true;
			for (int i = 0;i < n;i++) {
				fs.read((char*)&Ubox[i], sizeof(int));
				swap(Ubox[i].arr[1], Ubox[i].arr[2]);
				swap(Ubox[i].arr[0], Ubox[i].arr[3]);
				cout << "inst " << i << ": ";
				cout << setw(8) << setfill('0') << hex << Ubox[i].a << " " << dec;

				tlr = (bitset<32>(Ubox[i].a)).to_string();
				change(tlr);
				cout << '\n';
			}
			Ubox[n].a = -1;
			read_flag = false;
			fs.close();
			delete[]fileName;
		}
		else if (command == "loadinst") {
			cin >> fileName;
			ifstream fs;
			fs.open(fileName, ios::binary);
			fs.seekg(0, ios::end);
			int sz = fs.tellg();
			n = sz / 4;
			fs.seekg(0, ios::beg);

			for (int i = 0;i < n;i++) {
				fs.read((char*)&Ubox[i], sizeof(int));
				swap(Ubox[i].arr[1], Ubox[i].arr[2]);
				swap(Ubox[i].arr[0], Ubox[i].arr[3]);
			}
			Ubox[n].a = -1;

			fs.close();
			delete[]fileName;
		}
		else if (command =="loaddata") {
			cin >> fileName;
			ifstream fs;
			fs.open(fileName, ios::binary);
			fs.seekg(0, ios::end);
			int sz = fs.tellg();
			n = sz;
			fs.seekg(0, ios::beg);
			
			init_data();
			for (int i = 0;i < n;i++) {
				fs.read((char*)&dataBox[i], sizeof(char));
			}
		}
		else if (command == "run") {
			init_inst(); // set all registers 0
			int hm,cnt=0;
			cin >> hm;
			string tlr;
			pc = 0;
			for (int i = 0;i < hm;i++) {
				if (pc < 0 || pc > 0x00010000){
					cout << "Memory address out of range: 0x";
					cout << setw(8) << setfill('0') << hex << pc << " " << dec << "\n";
					break;
				}
				//No case of !(pc/4)
				tlr = (bitset<32>(Ubox[pc/4].a)).to_string();
				change(tlr);
				cnt++;
				
				if (error_flag == true) {
					break;
				}
				if(unknown_flag == true) {
					cout << "unknown instruction" << '\n';
					break;
				}
			
			}
			cout << "Executed " << cnt << " instructions\n";
		}
		else if (command == "registers") {
			for (int i = 0;i < 32;i++) {
				cout << "$" << i << ": 0x";
				cout << setw(8) << setfill('0') << hex << res[i] << " " << dec <<"\n";
			}
			cout << "HI: 0x";
			cout << setw(8) << setfill('0') << hex << res_HI << " " << dec << "\n";
			cout << "LO: 0x";
			cout << setw(8) << setfill('0') << hex << res_LO << " " << dec << "\n";
			cout << "PC: 0x";
			cout << setw(8) << setfill('0') << hex << pc << " " << dec << "\n";
		}
		else if (command == "exit") break;
		/*else if (command == "seedata") {
			for(int i = 0 ; i < 20 ; i++){
				int m = dataBox[i];
				cout << setw(8) << setfill('0') << hex << m << " " << dec << "\n";
			}
		}*/
		else {
			cout << "Wrong command" << endl; continue;
		}
	}

	return 0;
}
