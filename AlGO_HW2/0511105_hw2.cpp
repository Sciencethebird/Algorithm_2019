#include <iostream>
#include <fstream>
#include <string>
//#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;


int max_value_recursive(int area, int** a, int num_of_crop) {
	//cout << area << endl;
	if (num_of_crop < 0||area<=0) return 0;
	int subarea = area - a[num_of_crop][0];
	if (subarea>=0){
		  return max(max_value_recursive(subarea, a, num_of_crop) + a[num_of_crop][1],
		         max_value_recursive(area, a, num_of_crop - 1));
	}else return max_value_recursive(area, a, num_of_crop - 1);
}


int max_value_recursive_mem(int area, int** a, int num_of_crop, int **calculated) {
	int temp1, temp2;
	if (num_of_crop < 0 || area <= 0) return 0;
	if (calculated[area][num_of_crop]) return calculated[area][num_of_crop];
	int subarea = area - a[num_of_crop][0];
	if (subarea >= 0) {
	
		temp1 = max_value_recursive_mem(subarea, a, num_of_crop, calculated) + a[num_of_crop][1];
		temp2 = max_value_recursive_mem(area, a, num_of_crop - 1, calculated);
		
		return calculated[area][num_of_crop] = max(temp1, temp2);
	}
	else{
		return calculated[area][num_of_crop] = max_value_recursive_mem(area, a, num_of_crop - 1, calculated);
	}
}

int mode_2(int area, int** a, int num_of_crop) {
	int **cal = new int*[area + 10];
	for (int i = 0; i < area + 10; i++)
		cal[i] = new int[num_of_crop + 1];
	for (int i = 0; i < area + 10; i++)
		for (int j = 0; j < num_of_crop + 1; j++)
			cal[i][j] = 0;
	return max_value_recursive_mem(area, a, num_of_crop - 1, cal);
}

int max_value_dp(int area, int** a, int num_of_crop) {
	//unordered_map<int, int > calculated;
	int * calculated = new int[area+10];
	for (int i = 0; i < area+10; i++) calculated[i] = 0;
	for (int i = 0; i <= area; i++) {
		for (int j = 0; j < num_of_crop; j++) {
			if (i - a[j][0] >= 0) {
				calculated[i] = max(calculated[i], calculated[i - a[j][0]] + a[j][1]);
			}		
		}
	}
	return calculated[area];
}

int max_value_dp_bonus(int area, int** a, int num_of_crop) {  
	//unordered_map<int, int > calculated;
	int * calculated = new int[100000];
	for (int i = 0; i < 100000; i++) calculated[i] = 0;
	for (int i = 0; i < num_of_crop; i++) {
		calculated[a[i][0]] = a[i][1];
	}
    vector<int> acomb;
	for (int i = 0; i < num_of_crop; i++) {
		acomb.push_back(a[i][0]);
	}

	int new_area, new_value;
	int run_num = area / 3 + 1;
	for (int k = 0; k < 5; k++) { // combination
		//cout << k << endl;
        vector<int> temp;
		for (int i = 0; i < acomb.size(); i++) {
			for (int j = 0; j < num_of_crop; j++) {
				new_area = a[j][0] + acomb[i];
				new_value = a[j][1] + calculated[acomb[i]];
				if (new_area <= area) {
					temp.push_back(a[j][0] + acomb[i]);
					if (new_value > calculated[new_area]) {
						calculated[new_area] = new_value;
					}
				}
			}
		}
		acomb.clear();
		for (int i = 0; i < temp.size();i++) {
			acomb.push_back(temp[i]);
		}
	}
	return calculated[area];
}


int main(int argc, const char * argv[])
{
	fstream file;
	//string file_name = "case11.txt"; // dp still faster
	//file.open(file_name, ios::in);

	string input_file = argv[1];
	string output_file = argv[2];
	file.open(input_file.c_str(), ios::in);

	int mode;
	file >> mode;

	int Area, num_of_crop, crop_area_temp;
	file >> Area >> num_of_crop;

	
	int **crop;
	crop = new int*[num_of_crop];
	for (int i = 0; i < num_of_crop; i++) 
		crop[i] = new int[3];


	for (int i = 0; i < num_of_crop; ++i) {
		file >> crop[i][0] >> crop[i][1];
	}
	int ans;
	if(mode == 0)
		ans = max_value_recursive(Area, crop, num_of_crop-1);
	else if(mode == 1)
		ans = max_value_dp(Area, crop, num_of_crop);
    else if(mode == 2){
		ans = mode_2(Area, crop, num_of_crop);
        //cout << "mem" << endl;
    }
	//int ans = mode_2(Area, crop, num_of_crop);
	//
	//int ans = max_value_dp_bonus(Area, crop, num_of_crop);
    file.close();
	file.open(output_file.c_str(), fstream::out);
	file << ans << endl;
	file.close();
    //cout << output_file << endl;
 	//knapsack(crop, num_of_crop, Area);
	return 0;
}

