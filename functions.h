#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<map>
#include<math.h>
#include<limits>
using namespace std;

void read_parameters(string &file_name, float &support, float &confidence);
void save_data(string file_name,int &attribute_count,vector< vector<string> > &info, int &line_count);
void print_info(int line_count, int attribute_count, vector< vector<string> >info);
void get_set_item(vector< vector<string> > info, int attribute_count, int line_count, map<string,int> &set_item, int &total);
void print_set_item(map<string, int> set_item);
void init_filter(map<string, int> set_item,int threshold,vector< vector<string> > &filtered);
void print_filtered(vector< vector<string> >filtered);
void join(vector< vector<string> > temp, vector< vector<string> > &temp1,vector< vector<string> > filtered);
void find_possible_join(vector< vector<string> >::iterator it, vector< vector<string> >::iterator jt, bool &flag, vector<string> &row);//flag:whether these two joinable
void get_all_subsets(vector<string> row,vector< vector<string> > &subsets);
bool check_subsets_frequent(vector< vector<string> > subsets, vector< vector<string> > filtered);
map< vector<string>, int> count(vector< vector<string> >temp1, vector< vector<string> > info);
vector< vector<string> > check_all_subsets(vector<string> transaction, vector< vector<string> > candidates);
bool check_in_transaction(vector<string> candidate_subset, vector<string> transaction);
int filter(map< vector<string>, int> count_subset_occureance, float thredshold, vector< vector<string> > &filtered);
vector< vector< vector<string> > > generate_rules(vector< vector<string> > filtered, vector< vector<string> > info,float threshold, int &rules_num,ofstream &output);
void check_possible_rules(vector<string> frequent_itemset, vector< vector< vector<string> > > &rules, vector< vector<string> > info,float threshold, int &rules_num,ofstream &output);


vector< vector< vector<string> > > get_all_possible_partition(vector<string> frequent_itemset);
void check_confidence(vector<string> from, vector<string> to, vector< vector< vector<string> > > &rule, vector< vector<string> > info, float threshold, int &rules_num,ofstream &output);
void print_rules(vector< vector< vector<string> > > rules);
void print_vector_string(vector<string> vector_string, int indictor,ofstream &output);
void writeToFile( char const* filename, unsigned lineNo, string toWrite);
#endif
