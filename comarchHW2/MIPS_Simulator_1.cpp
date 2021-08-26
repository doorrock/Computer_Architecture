/* add, addu, and, div, divu, jalr, jr, mfhi, mflo, mthi, mtlo, mult, multu, nor,
or, sll, sllv, slt, sltu, sra, srav, srl, srlv, sub, subu, syscall, xor, addi, addiu, 
andi, beq, bne, lb, lbu, lh, lhu, lui, lw, ori, sb, slti, sltiu, sh, sw, xori, j, jal
*/
#include<iostream>
#include<string>
#include<bitset>
#include<iomanip>
#include<fstream>
using namespace std;
union ubox
{
	int a;
	unsigned char arr[4];
}Ubox[100];

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
	if (temp == "100000") { cout << "add $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "100001") { cout << "addu $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "100100") { cout << "and $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "100111") { cout << "nor $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "100101") { cout << "or $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "101010") { cout << "slt $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "101011") { cout << "sltu $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "100010") { cout << "sub $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "100011") { cout << "subu $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "100110") { cout << "xor $" << r_rd(s) << ", $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "000000") { cout << "sll $" << r_rd(s) << ", $" << r_rt(s) << ", " << sa(s); }
	else if (temp == "000100") { cout << "sllv $" << r_rd(s) << ", $" << r_rt(s) << ", $" << r_rs(s); }
	else if (temp == "000011") { cout << "sra $" << r_rd(s) << ", $" << r_rt(s) << ", " << sa(s); }
	else if (temp == "000111") { cout << "srav $" << r_rd(s) << ", $" << r_rt(s) << ", $" << r_rs(s); }
	else if (temp == "000010") { cout << "srl $" << r_rd(s) << ", $" << r_rt(s) << ", " << sa(s); }
	else if (temp == "000110") { cout << "srlv $" << r_rd(s) << ", $" << r_rt(s) << ", $" << r_rs(s); }
	else if (temp == "011010") { cout << "div $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "011011") { cout << "divu $" << r_rs(s) << ", $" << r_rt(s);	}
	else if (temp == "010000") { cout << "mfhi $" << r_rd(s); }
	else if (temp == "010010") { cout << "mflo $" << r_rd(s); }
	else if (temp == "010001") { cout << "mthi $" << r_rs(s); }
	else if (temp == "010011") { cout << "mtlo $" << r_rs(s); }
	else if (temp == "011000") { cout << "mult $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "011001") { cout << "multu $" << r_rs(s) << ", $" << r_rt(s); }
	else if (temp == "001001") { cout << "jalr $" << r_rs(s); }
	else if (temp == "001000") { cout << "jr $" << r_rs(s); }
	else if (temp == "001100") { cout << "syscall "; }
	//else if (temp == "001101") { cout << "unknown instruction "; }
	else { cout << "unknown instruction "; }
}

void change(string s) {
	string temp = s.substr(0, 6);
	if (temp == "000000") rFormat(s);
	else if (temp == "001000") { cout << "addi $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); }
	else if (temp == "001001") { cout << "addiu $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); }
	else if (temp == "001100") { cout << "andi $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); }
	else if (temp == "001111") { cout << "lui $" << r_rt(s) << ", " << imm(s); }
	else if (temp == "001101") { cout << "ori $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); }
	else if (temp == "001010") { cout << "slti $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); }
	else if (temp == "001011") { cout << "sltiu $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); }
	else if (temp == "001110") { cout << "xori $" << r_rt(s) << ", $" << r_rs(s) << ", " << imm(s); }
	else if (temp == "000100") { cout << "beq $" << r_rs(s) << ", $" << r_rt(s) << ", " << offset(s); }
	/*else if (temp == "000001") {
		string t = s.substr(11, 5);
		if (t == "00001") { cout << "bgez $" << r_rs(s) << ", " << offset(s); }
		else if (t == "10001") { cout << "bgezal $" << r_rs(s) << ", " << offset(s); }
		else if (t == "00000") { cout << "bltz $" << r_rs(s) << ", " << offset(s); }
		else if (t == "10000") { cout << "bltzal $" << r_rs(s) << ", " << offset(s); }
	}*//*
	else if (temp == "000111") { cout << "bgtz $" << r_rs(s) << ", " << offset(s); }
	else if (temp == "000110") { cout << "blez $" << r_rs(s) << ", " << offset(s); }*/
	else if (temp == "000101") { cout << "bne $" << r_rs(s) << ", $" << r_rt(s) << ", " << offset(s); }
	else if (temp == "000010") { cout << "j " << target(s); }
	else if (temp == "000011") { cout << "jal " << target(s); }
	/*else if (temp == "010000") {
		string t = s.substr(6, 5);
		if (t == "00000") { cout << "mfc0 $" << r_rt(s) << ", $" << r_rd(s); }
		else if (t == "00100") { cout << "mtc0 $" << r_rt(s) << ", $" << r_rd(s); }
	}*/
	else if (temp == "100000") { cout << "lb $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; }
	else if (temp == "100100") { cout << "lbu $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; }
	else if (temp == "100001") { cout << "lh $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; }
	else if (temp == "100101") { cout << "lhu $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; }
	else if (temp == "100011") { cout << "lw $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; }
	else if (temp == "101000") { cout << "sb $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; }
	else if (temp == "101001") { cout << "sh $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; }
	else if (temp == "101011") { cout << "sw $" << r_rt(s) << ", " << offset(s) << "($" << r_rs(s) << ")"; }
	else { cout << "unknown instruction"; }
}

int main(void)
{
	while (1) {
		cout << "mips-sim> ";
		char* fileName = new char[100];
		string command;
		cin >> command;
		if (command == "read") cin >> fileName;
		else if (command == "exit") break;
		else {
			cout << "Wrong command" << endl; continue;
		}

		ifstream fs;
		fs.open(fileName, ios::binary);
		fs.seekg(0, ios::end);
		int sz = fs.tellg();
		int n = sz / 4;
		fs.seekg(0, ios::beg);

		string tlr;
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
		fs.close();
		delete []fileName;
	}
	
	return 0;
}