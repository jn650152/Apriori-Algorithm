#include "functions.h"
void read_parameters(string &file_name, float &support, float &confidence){//read file name, support rate and confidence rate
	cout << "what the name of the dataset?"<< endl;
        cin >> file_name;
        cout << "what is the minimum support rate?"<< endl;
        cin >> support;
        cout << "what is the minimum confidence rate?"<< endl;
        cin >> confidence;
}
void save_data(string file_name,int &attribute_count,vector< vector<string> > &info, int &line_count){//save file's data into a vector info.
        ifstream file;
        string line;
        file.open(file_name);
	  
	//save titles and dataset
       	if(getline(file, line)){//get titles
              	stringstream ss;
               	ss  << line;
              	vector<string> attributes;//save attrubutes
    	        string column;
    	        while(ss >> column){
               	        attributes.push_back(column);
                       	attribute_count++;
               	}

             	while(getline(file, line)){//save dataset
                        stringstream ss;
              	        ss << line;
                      	string cell;
            	    	vector<string> row;
			int column_num = 0;
                	while(ss >> cell){//process and save each row
                       		row.push_back(attributes[column_num]+"= "+cell);
				column_num++;
                        }
                       	line_count ++;
                       	info.push_back(row);
               	}
	
       	}else{
               	cout << "failed to read the file"<< endl;
       	}
}
//check dataset vector(for testing)
void print_info(int line_count, int attribute_count, vector< vector<string> >info){
	cout << "print database: "<<endl;
        for(int j = 0; j < line_count; j++){
                for(int i = 0; i < attribute_count; i++){
                        cout << info[j][i]<< " ";
                }
                cout << endl;
        }
}
//get the set of items and their correponding numebr of occurence.
void get_set_item(vector< vector<string> > info, int attribute_count, int line_count, map<string,int> &set_item,int &total){
	for(int i =0; i < line_count; i ++){
		for(int j = 0; j < attribute_count; j++){
			if(set_item.find(info[i][j])==set_item.end()){//if the item is not in the dictionary
				set_item[info[i][j]]= 1;//record it in the dictionary with 1 occurence		
				total ++;//record the number of items in the set of items
			}else{
				set_item[info[i][j]]++;//if the item already existed in the dictionary, add 1 to the occurence for that item
			}					
		}
	}
}
//check the set of items for testing
void print_set_item(map<string, int> set_item){
	for (const auto &p : set_item) {
 	   cout << "set_item[" << p.first << "] = " << p.second << '\n';
	}
}
//get and save the frequent one-item itemset
void init_filter(map<string, int> set_item,int threshold,vector< vector<string> > &filtered){
	for(const auto &p : set_item){
		if(p.second > threshold){//if it is frequent itemset, save it in vector filered as a frequent itemset
			vector<string> row;
			row.push_back(p.first);
			filtered.push_back(row);
		}
	}
}
//check the initial filered for testing
void print_filtered(vector< vector<string> >filtered){
	for(auto it = filtered.begin(); it != filtered.end(); ++it)
	{
		for(auto jt = it->begin(); jt != it->end(); ++jt)
		{
			cout << *jt << " ";
		}
		cout << endl;
	}
}
//join(find joined itemset and check whether all of their subsets are frequent and save possible combination after joining in temp1 )
void join(vector< vector<string> > temp, vector< vector<string> > &temp1,vector< vector<string> > filtered){//temp: the items combination of the current level; temp1: possible items combination after being joined, filtered: the frequent itemsets we already got.
	map< vector<string>, bool > joined;//store possible joining
	//joining 
	for(auto it = temp.begin(); it != (temp.end()-1); ++it){
		for(auto jt = it + 1; jt != temp.end(); ++jt){
			bool flag = false;
			vector<string> row;
			find_possible_join(it, jt, flag, row);//flag:whether these two joinable
			if(flag == true){//only one different
				if(joined.find(row)== joined.end()){//if the joined result is not formed before
					vector< vector<string> > subsets;
					get_all_subsets(row,subsets);//get its all subsets
					bool frequent = check_subsets_frequent(subsets, filtered);//check whether all the subsets of the joined itemset is frequent
					if(frequent){//no infrequent subsets
						joined[row] = true;
						temp1.push_back(row);//save it in temp1
					}
				}
			}
		}
	}

}
//find joined itemset(only one diff)
void find_possible_join(vector< vector<string> >::iterator it, vector< vector<string> >::iterator jt, bool &flag, vector<string> &row){//it, jt represent two itemsets to be joined, flag stands for whether the jt and it are joinable, row stores the joined result

	//a few parameters to record items and difference within two itemsets
	map<string, bool> map;//record items within two itemsets it,jt
	string new_item;//new item to be added after being joined
	int diff = 0;//the number of different items between two item combinations


	//scan two itemsets to check whether these two itemsets are joinable(joinable means two itemset only contain one different item)
	//scan the first itemset
	for(vector<string>::iterator i = it->begin(); i != it->end(); ++i){//scan items in the first itemset and record items
		map[*i] = true;
		row.push_back(*i);//push all the items within the first itemset to vector row 
	}
	//scan the second itemset
	for(vector<string>::iterator j = jt -> begin(); j != jt->end(); ++j){//scan item in the second itemset and record new items
		if(map.find(*j) == map.end()){//if new item(not in first itemset)
			diff ++;//record number of difference
			map[*j] = true;
			new_item = *j;//save the different item
		}
	}
	//check whether joinable and save the joined in row
	if(diff == 1){//if joinable
		row.push_back(new_item);//form the new joined itemset by pushing the only different item to row
		flag = true;//set the flag
	}

}
//get all subsets of the joined itemset
void get_all_subsets(vector<string> row,vector< vector<string> > &subsets){//parameter subset stores all the subset of parameter row
	int len = row.size();

	//use binary representation represents subsets. 1: the item exists in the subset, 0: the item doesn't exist in the subset. there are pow(2, len)-2 subsets
	for(int i = 1; i < pow(2, len)-1; i++){
	//	int current = i;
		vector<string> result;//represents one subset
		int temp = i;
		for(int j=0; j < len; j++){
			if(temp%2){
				result.push_back(row[j]);
			}
			temp = temp/2;
		}
		subsets.push_back(result);
	}
	
}
//check whether all the subsets are frequent
bool check_subsets_frequent(vector< vector<string> > subsets, vector< vector<string> > filtered){//filtered represented frequent itemsets we already got

	for(auto i = subsets.begin(); i != subsets.end();i++){//scan all subsets
		for(auto m = filtered.begin(); m != filtered.end();m++){//scan all frequent itemsets we already got

			int match = 0;//a flag indicates whether the subset is found in known frequent itemset by now. 1: found, 0: not found yet
			if(*m == *i){//if the subset is frequent itemset
				match = 1;
				break;//jump out the current loop to check next subset
			}
			if(m == (filtered.end()-1)){//if finish scaning all the known frequent itemsets
				if(match == 0){//and not yet find a match for the current subset
					return false;//then the subset is infrequent
				}
			}
	        }	
	}
	return true;//if finish scaning all the subsets and find a match for each subset, then all the subsets are frequent.
}

//count the frequency of the joined items combinations
map< vector<string>, int> count(vector< vector<string> >temp1, vector< vector<string> > info){
	map< vector<string>, int> count; 
	for(auto it = info.begin(); it != info.end(); it++){//scan the dataset
		//check all subset of newly generated candidates in each transaction
		vector< vector<string> > subsets = check_all_subsets(*it, temp1);
	
		//count the subsets
		for(auto i = subsets.begin(); i != subsets.end(); i++){
			if(count.find(*i) == count.end()){
				count[*i] = 1;
			}else{
				count[*i]++;
			}
			
		}	
	}
	return count;
}
//find the subset of candidates that occur in the transaction
vector< vector<string> > check_all_subsets(vector<string> transaction, vector< vector<string> > candidates){
		vector< vector<string> > subsets;//store the subset of candidates appearing in the transaction

		for(auto i = candidates.begin(); i != candidates.end(); i++){//scan all candidates
			if(check_in_transaction(*i,transaction)){//if the candidate occurs in the current transaction, save it
				subsets.push_back(*i);
			}
		}
		return subsets;
	
}

//check whether the candidate occurs in the transaction
bool check_in_transaction(vector<string> candidate_subset, vector<string> transaction){
	bool all_find = false;
	for(auto i = candidate_subset.begin(); i != candidate_subset.end(); i++){//scan items in the candidate
		bool find = false;
		for(auto j = transaction.begin(); j != transaction.end(); j++){//scan items in the transaction
			if(*i== *j){//if find match, check next item in the candidate
				find = true;
				break;
			}
		}
		if(find == false){//if not find match, no need to continue to check becuase the candidate doesn't occur in the transaction
			break;
		}
		if(i == candidate_subset.end()-1){//if all the items in the candidate are checked and found matches, the candidate occurs in the transaction
			all_find = true;
		}
	}
	return all_find;

}

//filter out the infrequent and push those frequent itemsets into filtered.
int filter(map< vector<string>, int> count_subset_occureance, float thredshold, vector< vector<string> > &filtered){
	int num_added = 0;//count the number of newly added frequent itemsets 
        for (const auto &p :count_subset_occureance) {//scan the subset of candidates occuring in the current transaction
                if(p.second >= thredshold){//check frequency
			num_added++;
			filtered.push_back(p.first);
		}
        }
	return num_added;
} 
//generate rules
vector< vector< vector<string> > > generate_rules(vector< vector<string> > filtered, vector< vector<string> > info, float threshold, int &rules_num,ofstream &output){
	vector< vector< vector<string> > > rules;
	for(auto i = filtered.begin(); i != filtered.end(); i ++){//for each frequent itemset
		check_possible_rules(*i, rules,info, threshold, rules_num, output);//get rules for each frequent itemset
	}	
	return rules;
}
//get rules for each frequent itemset
void check_possible_rules(vector<string> frequent_itemset, vector< vector< vector<string> > > &rules,vector< vector<string> > info, float threshold,int &rules_num, ofstream &output){
	vector< vector<string> > subsets;
	vector< vector< vector<string> > > partitions = get_all_possible_partition(frequent_itemset);
	for(auto j = partitions.begin(); j != partitions.end(); j++){//for each partition
		 vector<string> from = *(j->begin());
		 vector<string> to = *(j->begin()+1);
		 check_confidence(from, to, rules,info, threshold, rules_num,output);//get rules with confidence rate higher than threshold
		
	}
}
//get all possible partitions of the frequent itemset(eg: one frequent itemset: {A,B,C}. possible partitions: {A}->{B,C},{B}->{A,C},etc.)
vector< vector< vector<string> > > get_all_possible_partition(vector<string> frequent_itemset){
	vector< vector< vector<string> > > partitions;
	int item_num = frequent_itemset.size();
	int size = pow(2,item_num)-1;//using binary presentation to describle possible partitions. All 0 digits belong to one side while all 1 digits belong to the other side.
	for(int i = 1; i < size; i++){//for each possible partition
		vector< vector<string> > possible_partition;
		int temp =i;
		vector<string> from;
		vector<string> to;
		for(int j =0; j < item_num; j++){
			if(temp%2){
				from.push_back(frequent_itemset[j]);
			}else{
				to.push_back(frequent_itemset[j]);
			}
			temp /=2;
		}
		possible_partition.push_back(from);
		possible_partition.push_back(to);
		partitions.push_back(possible_partition);
	}
	return partitions;
}

//compute confidence rate and compare it with threshold to get rules.store rules in a file.
void check_confidence(vector<string> from, vector<string> to, vector< vector< vector<string> > > &rules, vector< vector<string> > info, float threshold, int &rules_num,ofstream &output){
	int x = 0;//confidence rate= y/x
	int y = 0;
	for(auto i = info.begin(); i != info.end(); i++){//scan db
		if(check_in_transaction(from, *i)){
			x++;
			if(check_in_transaction(to, *i)){
				y++;
			}
		}
	}
	int size = info.size();
	//get support rate for both and compute confidence rate 
	//compare and add rules
	float confidence_rate = float(y)/x;
	if(confidence_rate >= threshold){
		string confidence_string = to_string(confidence_rate);
		string support_string = to_string(float(y)/size);
		vector<string> rate;
		rate.push_back(support_string);
		rate.push_back(confidence_string);
		vector< vector<string> > from_to;
		from_to.push_back(from);
		from_to.push_back(to);
		from_to.push_back(rate);
		rules_num++;
		rules.push_back(from_to);
		//write the rule to a file
		output <<  "#Rule"<< rules_num <<": (";
		print_vector_string(*(from_to.begin()+2),2,output);
		output<<")"<<endl;
		output<<"{";
		print_vector_string(*(from_to.begin()),1,output);
		output<< "}";
		output << " => "; 
		output<< "{";
		print_vector_string(*(from_to.begin()+1),1,output);
		output<< "}";
		output << endl;
	
	}
	
}
/*
void print_rules(vector< vector< vector<string> > > rules){
	int num = 1;
	for(auto i = rules.begin(); i != rules.end(); i++){
		cout << "#Rule"<< num <<": ";
		cout << "(";
		print_vector_string(*(i-> begin()+2), 2);
		cout<<")"<<endl;
		print_vector_string(*(i-> begin()),1);
		cout << " => "; 
		print_vector_string(*(i-> begin()+1),1);
		cout << endl;
		num++;
	}
}
*/

//write standanrize the format of rules
void print_vector_string(vector<string> vector_string, int indicator,ofstream &output){
	if(indicator ==1){
		for(auto i = vector_string.begin(); i != vector_string.end(); i ++){
			if(i != vector_string.end()-1){
				output << *i << ", ";
			}else{
				output << *i <<" ";
			}
		}
	}else if(indicator == 2){
		auto i = vector_string.begin();
		output << "support rate: "<<*i << ", ";
		output << "confidence rate: "<<*(i+1);
		
	}
}
//write the number of discovery rules to a file
void writeToFile( char const* filename, unsigned lineNo, string toWrite)
{
     fstream file( filename ) ;
     unsigned currentLine = 0 ;
     while ( currentLine < lineNo )
     {
          file.ignore( std::numeric_limits<std::streamsize>::max(), '\n') ;
          ++currentLine ;
     }

     file.seekp(file.tellg()) ;
     file << toWrite ;  

}
