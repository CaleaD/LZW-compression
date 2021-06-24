#include <iostream>
#include <fstream>
#include <cstring>
#include <string.h>
#include <vector>
#include <map>
#include <cmath>

#define MAX_COMMAND_LENGTH 1000

using namespace std;

/* --AUXILIARY FUNCTIONS -- */

void display(vector<int> &v){
    /* Displays the elements of a vector.
     * Used for testing the parser mainly. */
    for(int i; i<v.size(); i++){
        cout << v[i] << " ";
    }
    cout << "\n" << endl;
}

void parse_args(char* cmd_line, vector<string> &cmds){
    /* Given a command line and an empty vector, fills the vector with parsed argument tokens.
     * Returns the number of arguments. */
    char str[MAX_COMMAND_LENGTH];
    strcpy(str, cmd_line);
    char *p = strtok(str, " \0\t\n");
    int count = 0;
    while(p != NULL){
        cmds.push_back(p);
        p = strtok(NULL, " ");
        count++;
    }
    //display(cmds);
}

int getFileSize(string filename){
    /* Used to get the size of a file in bits. */
    FILE *file = fopen(filename.c_str(),"r");
    fseek(file,0,SEEK_END);
    int size = ftell(file);
    printf("Bytes: %d (%d bits)\n",size,size*8);
    fclose(file);
    return 8*size;
}

int getPercent(float in, float out){
    /* Used to calculate the percentage decrease after the change. */
    double change = (in-out)/in;
    return change*100;
}


/* --COMMANDS-- */

vector<int> compress(string filename, string out){
    /* Compression algorithm function */
	FILE *infile = fopen(filename.c_str(),"r");
	if (infile == NULL) {
        if (errno == EACCES){
            cout << "Error: Permission denied.\n" << endl;
            return {};
        }
        else {
            cout << "Error: File is missing.\n"<< endl;
            return {};
        }
    }

    string ch = "";
    string p = "";
    vector<int> encoding;
    map<string, int> dict;
    string str = "";
    int i = 0;

    //Fill the dictionary with all characters and their encodings.
    for (;i<=255;i++) {
        string ch;
        ch += char(i);
        dict[ch] = i;
    }

    printf("Compressing...\n");
    p += getc(infile);
    char c;
    while((c = getc(infile))!=(unsigned)EOF){
        ch = "";
        ch += c;
        //If dictionary doesn't contain p+c add it
        if(dict.find(p+ch) == dict.end()){
            encoding.push_back(dict[p]);
            //Encode p+ch, add it to dictionary and increment i
            dict[p+ch] = i++;
            //Prefix becomes ch
            p = ch;
        }
        else {
            //Otherwise, prefix becomes p+ch
            p = p+ch;
        }
    }
    fclose(infile);
    encoding.push_back(dict[p]);

    //Write output to new file.
    FILE *outfile = fopen(out.c_str(),"w");
    if(outfile == NULL){
        printf("Error: Unable to create file.\n");
        return {};
    }

    for(int  j= 0; j<encoding.size(); j++){
        fprintf(outfile,"%s",&encoding[j]);
    }
    fclose(outfile);

    int size_in = getFileSize(filename);
    int size_out = getFileSize(out);

    int percent = getPercent(size_in,size_out);

    printf("The file was reduced by %d percent.\n",percent);

    printf("Do you wish to see the dictionary? [y/n]\n");
    while(true){
        char choice;
        scanf("%s",&choice);
        if(choice == 'y'){
            for (const auto& x : dict) {
                cout << x.first << ": " << x.second << "\n";
            }
            return encoding;
        }
        else if(choice == 'n'){
            return encoding;
        }
        else {
            printf("Error: Please answer with y(yes) or n(no).\n");
        }
    }

}

void decompress(vector<int> encoding,string out){
    /* Decompression algorithm function */
    map<int, string> dict;
    //Write output to new file.
    /*FILE *outfile = fopen(out.c_str(),"w");
    if(outfile == NULL){
        printf("Error: Unable to create file.\n");
        return;
    }*/

    int i = 0;
    int prev = encoding[0];
    string ch = "";
    //Fill the dictionary with all characters and their encodings.
    for (;i<=255;i++) {
        ch = char(i);
        dict[i] = ch;
    }

    string p = dict[prev];
    string c = "";
    c += p[0];
    cout<<p;
    //fprintf(outfile,"%s",&p);
    for(int  j= 1; j<encoding.size(); j++){
        if (dict.find(encoding[j]) != dict.end()) {
            p = dict[encoding[j]];
        }
        else {
            p = dict[prev]+c;
        }
        c = "";
        c += p[0];
        cout<<p;
        //fprintf(outfile,"%s",&p);
        dict[i++] = dict[prev]+c;
        prev = encoding[j];
    }

    //fclose(outfile);
}

void help(){
    printf("\t\t\t USER COMMANDS \n");
    printf("-c [FILE_NAME] - compress file\n");
	printf("-d             - decompress file (encoding was stored previously)\n");
	printf("-e             - check encoding\n");
	printf("-h             - user commands \n");
	printf("-q             - exit application\n\n");
}

int execute(vector<string> args, vector<int> &encoding){
    if(args.size() > 2){
        printf("Error: Arguments size must not excel length 2.\n");
        return 1;
    }
    else if(args.size() == 0){
        printf("Error: Arguments size cannot be 0.\n");
        return 1;
    }
    else if(args[0][0]!= '-') {
        printf("Error: Incorrect format. Command must start with -.\n");
        return 1;
    }

    switch(args[0][1]) {
        case 'c': {
            if(args.size()==1)
                printf("Error: Incorrect format. Command -c must given a file name.\n");
            else{
                encoding = compress(args[1],"compressed.txt");
            }
            break;}
        case 'd': {
            if(encoding.empty()){
                printf("Error: No encoding stored. First, compress a file.\n");
            }
            else{
                decompress(encoding,"decompressed.txt");
            }
            break;}
        case 'e': {
            if(encoding.empty()){
                printf("There is no current encoding.\n");
            }
            else {
                for(int  j= 0; j<encoding.size(); j++){
                    printf("%s",&encoding[j]);
                }
            }
            break;
        }
        case 'h': {help(); break;}
        case 'q': {exit(0);}
        default : printf("Error: No such command.");
    }

    return 0;
}

/* --MENU AND INIT-- */

void menu(){
    printf("\t*** Lempel Ziv Welch Compression Algorithm ***\n\n");
    help();
    vector<int> en;
    while (true) {
        vector<string> args;
        char *buf;
        string line;
        cout << "\n> ";
        getline(cin, line);
        strcpy(buf, line.c_str());
        parse_args(buf,args);
        int status = execute(args,en);
    }
}

int main() {

    menu();
    return 0;
}
