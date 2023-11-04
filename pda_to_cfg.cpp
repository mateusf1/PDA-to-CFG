// File: pda_to_cfg.cpp
// Purpose: converts PDA to CFG.
//
// Created by Mateus A. Fernandes A.
// Date: 04/19/2020.
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void pda_to_cfg_function(vector<string> &CFG, vector<string> &PDA, int states);
void combine_productions(vector<string> &CFG);
void remove_useless_productions(vector<string> &CFG);
void remove_nonreachable_variables(vector<string> &CFG);


// main function: gets input, PDA-to-CFG convertion and output
int main(){
    
    int states;
    string input = "";
    string convert = "";
    vector<string> PDA;
    vector<string> CFG;
    
    cout << "PDA TO CFG CONVERTER\n Conditions:" << endl << endl;
    
    // Output: Condition List
    cout << "   1) PDA transition format should be follows:" << endl << endl;
    cout << "       $(qi,_a,_A) = {(qj,_X)}" << endl << endl;
    cout << "       Where: " << endl;
    cout << "           qi = current state (where i={0,1,2,3,4,5,6,7,8,9}) " << endl;
    cout << "           a = input symbol to read (including #)" << endl;
    cout << "           A = symbol to pop from the top of stack" << endl;
    cout << "          qj = next state to transition to (where j={0,1,2,3,4,5,6,7,8,9})" << endl;
    cout << "          X = string of symbols to push on top of stack (max 2)" << endl;
    cout << "          _ = space" << endl << endl;
    cout << "   2) Xi can only be up-to 2 symbols" << endl;
    cout << "   3) Use # for empty string and $ for the transition symbol" << endl;
    cout << "   4) PDA acceptance only by empty stack" << endl;
    cout << "   5) Only one transition per line; not on the same braces" << endl;
    cout << "   6) Start state should be q0" << endl;
    cout << "   7) Start stack symbol must be Z" << endl;
    cout << "   8) Brackets [], greater than > and vertical bar | cannot be in input nor stack alphabet" << endl << endl;
    
    
    // Input: PDA
    cout << "Please enter your PDA (enter DONE at the end of your PDA): " << endl;
    
    while (input != "DONE"){
        
        getline(cin, input);
        
        if(input[0] != '$')
            continue; //NOT break;
        
        PDA.push_back(input);
    }
    
    cout << endl << "Enter the total amount of states: ";
    cin >> states;
    
    //This loop creates initial CFG productions
    int count = 0;
    convert = "S --> ";
    
    while(count < states){
        convert += "[q0Zq";
        convert += to_string(count);
        convert += "]";
        count ++;
        if (count != states)
            convert += " | ";
    }
    
    CFG.push_back(convert);
    
    //function called to convert PDA to CFG
    pda_to_cfg_function(CFG, PDA, states);
    
    // Simplification
    
    combine_productions(CFG);
    remove_useless_productions(CFG);
    remove_nonreachable_variables(CFG);
    
    
    // Output: simplified CFG
    
    cout << endl << "CFG: " << endl;
    
    
    if(CFG.empty()){
        cout << "The language is empty!" << endl;
        cout << "There is no input string that will make the PDA stack go empty." << endl;
        cout << "Therefore, no useful CFG productions." << endl << endl;
        
    }else{
        
        for(string &s : CFG)
            cout << s << endl;
        cout << endl;
        
    }
    
    // End execution
    return 0;
}



// FUNCTIONS //

//This function converts PDA to CFG
void pda_to_cfg_function(vector<string> &CFG, vector<string> &PDA, int states){
    string convert = "";
    
    
    for(const string &s : PDA){
        
        //if empty string is being push to top of stack
        if(s[20] == '#'){
            convert = "[q";
            convert += s[3];
            convert += s[9];
            convert += s[16];
            convert += s[17];
            convert += "] --> ";
            convert += s[6];
            CFG.push_back(convert); //store the productions to CFG vector
            
        }//Otherwise
        else{
            int count = 0;
            int inner_count = 0;
            
            //This while loop gets all possible CFG productions for each PDA transition rule.
            while(count < states){
                if(s[21] != ')')
                    inner_count = 0;
                
                convert = "[q";
                convert += s[3];
                convert += s[9];
                convert += s[16];
                convert += to_string(count);
                convert += "] --> ";
                
                while(inner_count < states){
                    convert += s[6];
                    convert += "[q";
                    convert += s[17];
                    convert += s[20];
                    convert += "q";
                    convert += to_string(inner_count);
                    convert += "]";
                    
                    //Checks if there are 2 symbols getting push to top of stack
                    //If so, it creates 2nd variables and other possible productions
                    if(s[21] != ')'){
                        convert += "[q";
                        convert += to_string(inner_count);
                        convert += s[21];
                        convert += "q";
                        convert += to_string(count);
                        convert += "]";
                        inner_count++;
                        if(inner_count < states)
                            convert += " | ";
                        
                    }else{
                        
                        inner_count++;
                        break;
                    }
                }
                count++;
                CFG.push_back(convert); //store the productions to CFG vector
            }
        }
        
    }
    
}



// This functions combines productions in CFG with the same left-hand side (LHS) variable
void combine_productions(vector<string> &CFG){
    int i,j;
    for(i=1; i<CFG.size(); i++){
        for(j=i+1; j<CFG.size(); j++){
            if(CFG[i].substr(0,7) == CFG[j].substr(0,7)){
                CFG[i] += " | " + CFG[j].substr(12);
                CFG.erase(CFG.begin()+j); // removes LHS variable copies
            }
        }
    }
}




//This function removes all useless productions in CFG
//Excepts the non-reachables (yet)
void remove_useless_productions(vector<string> &CFG){
    int i, j;
    size_t count, pos;
    
    bool repeat; // = if some LHS variable got removed entirely
    bool found; // = iff variable in RHS matches a LHS variable
    bool term; // = if LHS variable reaches OR could reach terminal or empty string
    
    do{
        repeat = false;
        
        for(i=0; i<CFG.size(); ++i){
            term = false;
            
            count = CFG[i].find('[', 6);
            
            // Skips if just terminal/empty string productions = useful
            if(count == string::npos)
                continue;
            
            while(count != string::npos){
                found = false;
                
                // tries to find RHS production variables on some LHS variable
                // also checks for external variables that could prevent infinite loop
                for(j=0; j<CFG.size(); ++j){
                    if(CFG[i].substr(count,7) == CFG[j].substr(0,7)){
                        found = true;
                        if(i != j)
                            term = true; // = maybe
                    }else
                        if(i == j)
                            term = true; // = maybe
                }
                
                
                
                if(!found){ //Otherwise, it removes production
                    pos = CFG[i].find('|', count);
                    
                    for(; CFG[i][count] != '|' && CFG[i][count] != '>'; --count);
                    if(CFG[i][count] == '>'){
                        ++count;
                        ++pos;
                    }
                    
                    CFG[i].erase(count, pos-count);
                }
                count = CFG[i].find('[', count+1);
            }
            
            
            // Check if contains terminal/empty string prdoction = not cycle
            for(pos = CFG[i].find('>'); !term && pos != string::npos; pos = CFG[i].find('|', pos))
                if(CFG[i][pos+3] != '[')
                    term = true;
            
            
            // Removes any variable without production OR doesnt reaches terminal
            if(CFG[i].size() <= 11 || !term){
                CFG.erase(CFG.begin()+i);
                repeat = true;
            }
        }
    }while(repeat); // Stops repeating until all usless productions are deleted
}






// This function removes non-reachable left-hand side (LHS) variables from S in CFG
void remove_nonreachable_variables(vector<string> &CFG){
    
    //Clears all the CFG is NO starting production found
    if(CFG.empty() || CFG[0][0] != 'S'){
        CFG.clear();
        return;
    }
    
    int i, j;
    size_t count;
    bool repeat;
    vector<bool> reachable(CFG.size(), false);
    reachable[0] = true;
    
    do{
        repeat = false;
        
        for(i=0; i<CFG.size(); ++i){
            
            // Skip test on variables that are already known variable
            if (reachable[i] == true)
                continue;
            
            for(j=0; j<CFG.size(); ++j){
                
                // Skip testing it with a not known reachable variable
                if(reachable[j] == false)
                    continue;
                
                // Tries to find it in a production from a known reachable variable
                count = CFG[j].find('[', 6);
                while(count < CFG[j].size()){
                    if(CFG[i].substr(0, 7) == CFG[j].substr(count, 7)){
                        reachable[i] = true;
                        repeat = true;
                        break;
                    }
                    count = CFG[j].find('[', count+1);
                }
            }
        }
        
    }while(repeat); // Stop repeating until all reachables were identified
    
    // removes LHS variable that are non-reachable (and its productions)
    for(i=0; i<CFG.size(); ++i)
    {
        if(reachable[i] == false)
        {
            CFG.erase(CFG.begin()+i);
            reachable.erase(reachable.begin()+i);
        }
    }
}
