//Developing a banking system

#include<iostream>
#include<map>
#include<fstream>
using namespace std;
#define min_balance 500;
class insuffiecientfunds{};                                          //for throwing exception

class account{
    private:
    string fname;
    string lname;
    float balance;
    int account_no;

    static int next_accno;

    public:
    account();
    account(string fname,string lname,float balance);
    string getfname();
    string getlname();
    float getbalance();
    int getaccountno();

    void deposit(float amount);
    void withdraw(float amount);

    static void setlastaccountno(int account_no);
    int getlastaccountno();
    
    friend ofstream & operator<<(ofstream &out,account &ac);
    friend ifstream & operator>>(ifstream &in, account &ac);
    friend ostream & operator<<(ostream &output, account &ac);

};
int account::next_accno=0;

account::account(){
    account_no=0;
    fname="default";
    lname="default";
    balance=1;
    }
account::account(string fname,string lname,float balance){
    ++next_accno;
    this->fname=fname;
    this->lname=lname;
    this->balance=balance;
    this->account_no=next_accno;     
}
string account::getfname(){
    return fname;
}
string account::getlname(){
    return lname;
}
float account::getbalance(){
    return balance;
}
int account::getaccountno(){
    return account_no;
}
void account::deposit(float amount){
    balance+=amount;
}
void account::withdraw(float amount){
    try{
        if(balance-amount<=500){
            throw insuffiecientfunds();
        }
        else{
            cout<<"\nAmount withdrawed successfully"<<endl;
            balance-=amount;
        }
    }
    catch(insuffiecientfunds e){
        cout<<"\nMoney cannot be withdrawn as minimum balance should be Rs500"<<endl;
    }
    
}

void account::setlastaccountno(int account_no){
    next_accno=account_no;
}
int account::getlastaccountno(){
    return next_accno;
}
ofstream & operator<<(ofstream &out, account &ac){
    out<<ac.account_no<<endl;
    out<<ac.fname<<endl;
    out<<ac.lname<<endl;
    out<<ac.balance<<endl;
    return out;
}
ifstream & operator>>(ifstream &in, account &ac){
    in>>ac.account_no;
    in>>ac.fname;
    in>>ac.lname;
    in>>ac.balance;
    return in;
}
ostream & operator<<(ostream &output, account &ac){
    output<<ac.account_no<<endl;
    output<<ac.fname<<endl;
    output<<ac.lname<<endl;
    output<<ac.balance<<endl;
    return output;
}


class bank{
    private:
    map<int,account> accounts;                     //<int,account> int is for account no. and account is for complete account

    public:
    bank();
    account openaccount(string fname,string lname,float balance);
    account balanceenquiry(int account_no);
    account deposit(float amount, int account_no);
    account withdraw(float amount, int account_no);
    void closeaccount(int account_no);
    void showallaccount();
    ~bank();
};

bank::bank(){
    account aco;
    ifstream infile;
    infile.open("239projectbank.txt");
    if(!infile){
        cout<<"File does not exists"<<endl;
    }

    while(!infile.eof()){
        infile>>aco;
        accounts.insert(pair<int,account>(aco.getaccountno(),aco));
    }
    //scout<<aco.getaccountno()<<endl;
    aco.setlastaccountno(aco.getaccountno());
    infile.close();

}
account bank::openaccount(string fname,string lname,float balance){
    account acc(fname,lname,balance);
    int n=acc.getaccountno();
    if(n==1){                                    //when we created object of class bank (line202), default 
                                                 //constructor of bank will be called, as bank is having account 
                                                 //(<int,account>) default constructor of class account is also 
                                                 //called, and 0th index of map will contain values of default 
                                                 //constructor of account class so we need to erase it
        accounts.erase(0);
    }
    accounts.insert(pair<int,account>(acc.getaccountno(),acc));
    ofstream outfile;
    outfile.open("239projectbank.txt",ios::trunc);
    map<int,account>::iterator itr;
    //itr=accounts.begin();                                  
    //itr++;                                                   //we done this because (see line145 comment)
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        //outfile<<itr->first;                                 //no need to print account no. separately
        outfile<<itr->second;                                  //as account will be having account no.
    }
    outfile.close();
    return acc;
}
account bank::balanceenquiry(int account_no){
    map<int,account>::iterator itr;
    itr=accounts.find(account_no);
    return itr->second;
}
account bank::deposit(float amount,int account_no){
    map<int,account>::iterator itr;
    itr=accounts.find(account_no);
    itr->second.deposit(amount);
    return itr->second;
}
account bank::withdraw(float amount,int account_no){
    map<int,account>::iterator itr;
    itr=accounts.find(account_no);
    itr->second.withdraw(amount);
    return itr->second;
}
void bank::closeaccount(int account_no){
    map<int,account>::iterator itr;
    itr=accounts.find(account_no);
    cout<<"\nAccount closed: "<<itr->second<<endl;
    accounts.erase(account_no);
}
void bank::showallaccount(){
    map<int,account>::iterator itr;
    cout<<"\nAllAccounts:"<<endl;
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        cout<<itr->second<<endl;                                 
    }
}
bank::~bank(){
    ofstream outfile;
    outfile.open("239projectbank.txt");
    map<int,account>::iterator itr;
    // itr=accounts.begin();
    // itr++;
    for(itr=accounts.begin();itr!=accounts.end();itr++){
        outfile<<itr->second;                                
    }
    outfile.close();
}

int main(){
    bank b;
    account acc;

    string fname;
    string lname;
    float balance;
    int account_no;

    int num;

do{
    cout<<"***Banking System***"<<endl;
    cout<<"1.Open an Account"<<endl;
    cout<<"2.Balance Enquiry"<<endl;
    cout<<"3.Deposit"<<endl;
    cout<<"4.Withdraw"<<endl;
    cout<<"5.close an Account"<<endl;
    cout<<"6.Show AllAccount"<<endl;
    cout<<"7.Quit"<<endl;

    cout<<"\nEnter your choice"<<endl;
    cin>>num;

    
    switch(num){
        case 1:
            
            cout<<"Enter your first name: "<<endl;
            cin>>fname;
            cout<<"Enter your last name: "<<endl;
            cin>>lname;
            cout<<"Enter initial amount to deposit: "<<endl;
            cin>>balance;
            acc=b.openaccount(fname,lname,balance);
            cout<<endl;
            cout<<acc<<endl;
            break;
        
        case 2:
            cout<<"Enter your account number"<<endl;
            cin>>account_no;
            acc=b.balanceenquiry(account_no);
            cout<<"\nYour Account balance details"<<endl;
            cout<<acc<<endl;
            break;
        
        case 3:
            cout<<"Enter your account number"<<endl;
            cin>>account_no;
            int deposit;
            cout<<"Enter amount to deposit: "<<endl;
            cin>>deposit;
            acc=b.deposit(deposit,account_no);
            cout<<"\nAmount deposited successfully"<<endl;
            cout<<acc<<endl;
            break;

        case 4:
            cout<<"Enter your account number"<<endl;
            cin>>account_no;
            int withdraw;
            cout<<"Enter amount to withdraw: "<<endl;
            cin>>withdraw;
            acc=b.withdraw(withdraw,account_no);
            cout<<acc<<endl;
            break;
        
        case 5:
            cout<<"Enter your account number"<<endl;
            cin>>account_no;
            b.closeaccount(account_no);
            break;

        case 6:
            b.showallaccount();
            break;

        case 7:
            break;

        default:
            cout<<"Enter correct choice"<<endl;
            exit(0);
    }
}while(num!=7);

    return 0;
}