#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <limits>
using namespace std;

string asciiToBitString(const string& asciiString) {
    string bitString;
    for (char c : asciiString) {
        // Convert each character to its binary representation using bitset
        bitset<8> bits(c);
        // Append the binary representation to the bit string
        bitString += bits.to_string();
    }
    return bitString;
}

void makemultipleof4(string &str){
	int size = str.size();
	int r = size % 4;
	if(r != 0)
		for(int i = 0; i < 4-r; i++)
			str += "0";
}

void makemultipleof16(string &str){
	int size = str.size();
	int r = size % 16;
	if(r != 0)
		for(int i = 0; i < 16-r; i++)
			str += "0";
}

void crccode(string data, string &ans){
	int size = data.size();
	string gen = "100000111";

	cout << "dataword  ---->   crc  ---->  codeword" << endl;

	for(int m = 0; m < size/16; m++){
		string ans1 = "", data1 = data.substr(m*16, 16);

		for (int i=0;i<8;i++) data1+="0";

		for(int i=0;i<16;i++)
		{
			int j=0;
			int k=i;
			if (data1[k]>=gen[j])
			{
				for(j=0,k=i;j<9;j++,k++)
				{
					data1[k] = ((data1[k] - '0')^(gen[j] - '0') + '0');
				}
			}
		}
		string crc = data1.substr(16,8);
		ans1 += data.substr(m*16, 16);
		ans1 += crc;

		cout << data.substr(m*16, 16) <<"  --->  " << crc << "  --->  " << ans1 << endl;
		ans+=ans1;
	}

}

void crc_detection(string code){
	int size = code.size();
	string gen = "100000111";
	int uflag = 0;
	string dataword = "";

	cout << "codeword  ---->   reminder  ---->  result" << endl;

	for(int m = 0; m < size/24; m++){
		string code1 = code.substr(m*24, 24);
		string result;

		for(int i=0;i<16;i++)
		{
			int j=0;
			int k=i;
			if (code1[k]>=gen[j])
			{
				for(j=0,k=i;j<9;j++,k++)
				{
					code1[k] = ((code1[k] - '0')^(gen[j] - '0') + '0');
				}
			}
		}

		string rem = code1.substr(16,8);

		int flag=0;
		for(int i=0;i<8;i++)
		{
			if(rem[i]!='0')
			{
				flag=1;
				uflag = 1;
				break;
			}
		}

		if(flag==0) result = "error is not present";
		else result = "error is present";

		cout << code.substr(m*24, 24) << "  --->  " << rem << "  --->  " << result << endl;
		dataword += code.substr(m*24, 16);
	}

	cout << "\n------------------------- result -----------------------\n";
	if(uflag == 0){
		cout << "extracted datastream is ---  " << dataword << endl;
	}
	else{
		cout << "Discard\n";
	}
}

string binary_symmetric_channel(string data, int hop, float prob){
	string ans = data;
	prob *= 1000;
	int size = ans.size();

	while(hop--){
		for(int i = 0; i < size; i++){
			int k = rand()%1000;

			if(k < prob){
				if(ans[i] == '0') ans[i] = '1';
				else ans[i] = '0';
			}
		}
	}

	return ans;
}

void crcfunction(){
	string data, data1, data_ascii, encodedata = "";
	int hop;
	float prob;

	cout << "enter data of 32 bit or more -->  ";
	getline(cin, data_ascii);

	data = asciiToBitString(data_ascii);
	cout << "The bit string is: " << data << "\n";

	makemultipleof16(data);

	cout << "enter hop for binary symmetric channel 1 or 2 --> ";
	cin >> hop;

	cout << "enter probability between 0 and 1 --> ";
	cin >> prob;

	cout << "\n\n\n";
	cout << "##################################################################"<<endl;
	cout << "#                                                                #"<<endl;
	cout << "#                           at sender side                       #"<<endl;
	cout << "#                                                                #"<<endl;
	cout << "##################################################################\n"<<endl;

	cout << "data after appending zero's in the end to make it multiple of 16 -- " << data << endl;

	cout << "\n-------------------process-------------------\n";

	crccode(data, encodedata);

	cout << "\n------------------data to send---------------\n";
	cout << encodedata << endl;

	cout << "\n\n\n";
	cout << "##################################################################"<<endl;
	cout << "#                                                                #"<<endl;
	cout << "#                          at reciever side                      #"<<endl;
	cout << "#                                                                #"<<endl;
	cout << "##################################################################\n"<<endl;

	data1 = binary_symmetric_channel(encodedata, hop, prob);
	cout << "received data --  " << data1 << endl;
	cout << "\n-------------------process-------------------\n";

	crc_detection(data1);
}

void hammingcode(string data, string &ans){
	int size = data.size();
	int dataint[7];

	cout << "dataword  ---->   codeword" << endl;

	for(int i = 0; i < size/4; i++){
		dataint[0] = data[i*4] - '0';//  0101
		dataint[1] = data[i*4 + 1] - '0';
		dataint[2] = data[i*4 + 2] - '0';
		dataint[4] = data[i*4 + 3] - '0';

		dataint[6]=dataint[0]^dataint[2]^dataint[4];
		dataint[5]=dataint[0]^dataint[1]^dataint[4];
		dataint[3]=dataint[0]^dataint[1]^dataint[2];

		string codeword = "";

		for(int i = 0; i < 7; i++){
			codeword += (dataint[i] + '0');
		}

		cout << dataint[0] << dataint[1] << dataint[2] << dataint[4] << "   ---->    " << codeword << endl;
		ans += codeword;
	}
}

int hammingcorrection(string data, string &ans){
	int size = data.size();
	int dataint[7];
	int c, c1, c2, c3;
	int terror = 0;

	cout << "codeword  ---->   result" << endl;

	for(int i = 0; i < size/7; i++){
		dataint[0] = data[i*7] - '0';
		dataint[1] = data[i*7 + 1] - '0';
		dataint[2] = data[i*7 + 2] - '0';
		dataint[3] = data[i*7 + 3] - '0';
		dataint[4] = data[i*7 + 4] - '0';
		dataint[5] = data[i*7 + 5] - '0';
		dataint[6] = data[i*7 + 6] - '0';

		c1=dataint[6]^dataint[4]^dataint[2]^dataint[0];
		c2=dataint[5]^dataint[4]^dataint[1]^dataint[0];
		c3=dataint[3]^dataint[2]^dataint[1]^dataint[0];
		c=c3*4+c2*2+c1 ;

		string dataword = "";

		if(c==0) {
			dataword += (dataint[0] + '0');
			dataword += (dataint[1] + '0');
			dataword += (dataint[2] + '0');
			dataword += (dataint[4] + '0');
			cout<<dataint[0]<<dataint[1]<<dataint[2]<<dataint[3]<<dataint[4]<<dataint[5]<<dataint[6]<<"   ---->    No error in transmission and dataword is "<<dataword<<endl;
		}

		else{
			terror++;
			cout<<dataint[0]<<dataint[1]<<dataint[2]<<dataint[3]<<dataint[4]<<dataint[5]<<dataint[6]<<"   ---->    error at "<<c<<" bit and correct codeword is ";
			if(dataint[7-c]==0)
				dataint[7-c]=1;
			else
				dataint[7-c]=0;

			dataword += (dataint[0] + '0');
			dataword += (dataint[1] + '0');
			dataword += (dataint[2] + '0');
			dataword += (dataint[4] + '0');

			cout<<dataint[0]<<dataint[1]<<dataint[2]<<dataint[3]<<dataint[4]<<dataint[5]<<dataint[6]<<" and dataword -- "<<dataword<<endl;
		}
		ans += dataword;
	}
	return terror;
}

string randomerrorgenerator(string data){
	int size = data.size();
	string ans = data;

	for(int i = 0; i < size/7; i++){
		int k = rand() % 2;
		if(k == 1){
			int kk = rand() % 7;
			if(ans[i*7 + kk] == '0') ans[i*7 + kk] = '1';
			else ans[i*7 + kk] = '0';
		}
	}

	return ans;
}

void hammingcodefunction(){
	string data, data1, data_ascii, encodedata = "", decodeddata = "";

	cout << "enter data of 16 bit or more -->  ";
	getline(cin, data_ascii);

	data = asciiToBitString(data_ascii);
	cout << "The bit string is: " << data << "\n";

	cout << "\n\n\n";
	cout << "##################################################################"<<endl;
	cout << "#                                                                #"<<endl;
	cout << "#                           at sender side                       #"<<endl;
	cout << "#                                                                #"<<endl;
	cout << "##################################################################\n"<<endl;

	makemultipleof4(data);
	cout << "data after appending zero's in the end to make it multiple of 4 -- " << data << endl;

	cout << "\n-------------------process-------------------\n";
	hammingcode(data, encodedata);

	cout << "\n------------------data to send---------------\n";
	cout << encodedata << endl;

	cout << "\n\n\n";
	cout << "##################################################################"<<endl;
	cout << "#                                                                #"<<endl;
	cout << "#                          at reciever side                      #"<<endl;
	cout << "#                                                                #"<<endl;
	cout << "##################################################################\n"<<endl;

	data1 = randomerrorgenerator(encodedata);
	cout << "received data --  " << data1 << endl;
	cout << "\n-------------------process-------------------\n";

	int terror;
	terror = hammingcorrection(data1, decodeddata);

	cout << "\n------------------------- result -----------------------\n";
	cout << "there were error in " << terror <<" bit and extracted data stream is " << decodeddata;
}

void print_usage() {
	cout << "Usage: program_name -c/-h\n";
}

int main(int argc, char *argv[]) {
	if (argc < 1) {
		print_usage();
		return 1;
	}

	int opt;
	bool is_crc = false;
	bool is_hamming = false;
	while ((opt = getopt(argc, argv, "ch")) != -1) {
		switch (opt) {
			case 'c':
				is_crc = true;
				break;
			case 'h':
				is_hamming = true;
				break;
			case '?':
				print_usage();
				return 1;
			default:
				print_usage();
				return 1;
		}
	}

	if (is_crc)
		crcfunction();
	else if (is_hamming)
		hammingcodefunction();
	else {
		print_usage();
		return 1;
	}

	return 0;
}

