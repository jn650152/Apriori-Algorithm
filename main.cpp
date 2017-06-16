#include "functions.h"
#include<sstream>
#include<ctime>
using namespace std;
int main(){
	//get parameters
	string file_name;
	float support;
	float confidence;
	read_parameters(file_name, support, confidence);

	//read file and store the file's data into a vector info
	int rules_num = 0;
	int attribute_count = 0;
	vector< vector<string> > info;
	int line_count = 0;
	save_data(file_name,attribute_count,info, line_count);
	const long double time_start = time(0);
	//the file used to store rules
	ofstream output;
	output.open("Rules");
	output << "Summary:"<<endl;
	output << "Total rows in the original set: "<< line_count<<endl;
	output << "Total rules discovered: 12"<<endl;
	output << "The selected measures: Support = "<< support<<", Confidence= "<< confidence<<endl;
	output << "--------------------------------"<<endl;
	output <<"Discovered Rules:"<<endl;
	 
	//find the set of items
	map<string,int> set_item;//record the occurence of each item in a dictionary
	int total = 0;//the number of items in the set of items
	//cout << "before get_set_item"<<endl;
	get_set_item(info,attribute_count,line_count, set_item,total);

	//get frequent one-item itemsets and save it in vector filtered
	float support_threshold = line_count * support;
	vector< vector<string> > filtered;
	init_filter(set_item,support_threshold,filtered);
	vector< vector<string> > temp(filtered);//temp represents the frequent i-item itemset for current level. i here is 1. 
	//the following are getting frequent n-item itemset by joining, count, filter
	while(total!=1){
		total--;
		vector< vector<string> > temp1;
		join(temp, temp1,filtered);//temp: the items combination of the current level; temp1: possible items combination after being joined
		if(temp1.size() == 0){//if no possible items combination after being joined, no more need for the following joining 
			break;
		}
		map< vector<string>, int> count_subset_occureance= count(temp1,info);//count the frequency of the joined items combinations)
		int num_added = filter(count_subset_occureance, support_threshold,filtered);//filter out the infrequent and push those frequent item set into filtered. store rules
		if(num_added == 0){//if no new frequent itemset, no need for further joining
			break;
		}
		temp.clear();
		temp = temp1;//update temp to the next level frequent itemset
	}

//	cout << "generate rules"<<endl;		
	//generate association rules from frequent item sets (no prune)
	vector< vector< vector<string> > > rules = generate_rules(filtered, info, confidence,rules_num,output);
	//write the number of discovery rule to a file
	stringstream ss;
        ss << rules_num;
	writeToFile("Rules",2,"Total rules discovered: "+ss.str());
	output.close();
	const long double time_end = time(0);
    cout << "time taken: "<<1000*(time_end - time_start)<<"milliseconds"<<endl;
	return 0;
}
