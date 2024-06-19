#include<bits/stdc++.h>
using namespace std;
int64_t H,W,h,w,d,t,tt,err,step,stop=1;
const int dw[4]={1,0,-1,0},dh[4]={0,1,0,-1};
int64_t*code;
string input;
stringstream output;
unordered_map<bitset<128>,int64_t>out_of_code;
vector<int64_t>st;
vector<array<int64_t,3>>reg;
random_device seed;
mt19937_64 engine(seed());
uniform_int_distribution<>dist(0,3);
void draw(void){
  cout<<"----code----    step : "<<step<<endl;
  for(int j=0;j<H;j++){
    for(int i=0;i<W;i++){
      if(j==h&&i==w){
        switch(d){
          case 0:cout<<"→ ";break;
          case 1:cout<<"↓ ";break;
          case 2:cout<<"← ";break;
          case 3:cout<<"↑ ";break;
        }
      }
      else {
        t=code[j*W+i];
        if(t&&(t<32||126<t))cout<<"??";
        else if(!t)cout<<"  ";
        else cout<<char(t)<<' ';
      }
    }
    cout<<endl;
  }
  cout<<"----stack----    stack size : "<<reg.size()<<endl;
  int64_t pos=0;
  for(auto& i:reg){
    printf_s("len:%6lld",i[0]);
    if(i[1])printf_s("&%6lld",i[2]);
    else printf_s("&      ");
    cout<<"|";
    if(i[0]>2*10){
      for (int64_t n=0;n<10;n++)
        cout<<st[pos+n]<<",";
      cout<<"...";
      for (int64_t n=i[0]-10;n<i[0];n++)
        cout<<","<<st[pos+n];
    }
    else{
      for (int64_t n=0;n<i[0];n++) {
        if(n)cout<<",";
        cout<<st[pos++];
      }
    }
    cout<<endl;
  }
  cout<<"----output----"<<endl;
  cout<<output.str()<<endl;
  cout<<"--------------"<<endl;
}
int main(int argc,char* argv[]){
  //open code
  {
    string file_name(argv[1]);
    ifstream file(file_name);
    if(!file)return 1;
    vector<string>scode;
    string line;
    while(getline(file,line)){
      scode.emplace_back(line);
      W=max(W,(int64_t)line.size());
    }
    H=scode.size();
    code=new int64_t[H*W];
    memset(code,0,H*W*sizeof(int64_t));
    for(int64_t j=0;j<H;++j){
    for(size_t i=0;i<scode[j].size();++i){
      int64_t c=int64_t(scode[j][i]);
      code[j*W+i]=c==' '?0:c;
    }
  }
    file.close();
  }
  //open input
  {
    string file_name(argv[2]);
    ifstream file(file_name);
    if(!file)return 1;
    getline(file,input,'\0');
  }
  out_of_code.reserve(1000000);
  reg.emplace_back();
  reverse(input.begin(),input.end());
  // start
  while(!err){
    //instruction
    switch(code[h*W+w]){
      case 0:
      case ' ':break;
      case '>':d=0;break;
      case 'v':d=1;break;
      case '<':d=2;break;
      case '^':d=3;break;
      case '/':
        switch(d){
          case 0:d=1;break;
          case 1:d=0;break;
          case 2:d=3;break;
          case 3:d=2;break;
        }
        break;
      case '\\':
        switch(d){
          case 0:d=1;break;
          case 1:d=0;break;
          case 2:d=3;break;
          case 3:d=2;break;
        }
        break;
      case '_':
        switch(d){
          case 1:d=3;break;
          case 3:d=1;break;
        }
        break;
      case '|':
        switch(d){
          case 0:d=2;break;
          case 2:d=0;break;
        }
        break;
      case '#':
        switch(d){
          case 0:d=2;break;
          case 1:d=3;break;
          case 2:d=0;break;
          case 3:d=1;break;
        }
        break;
      case 'x':d=dist(engine);break;
      case '!':h+=dh[d];w+=dw[d];break;
      case '?':
        if(reg.back()[0]>=1){
          if(!st.back()){
            h+=dh[d];
            w+=dw[d];
          }
          st.pop_back();
          reg.back()[0]--;
        }
        else{
          cout<<"stack is empty(?)"<<endl;
          err=-1;
        }
        break;
      case '.':
        if(reg.back()[0]>=2){
          h=st.back();st.pop_back();
          w=st.back();st.pop_back();
          reg.back()[0]-=2;
          if(h<0||H<=h||w<0||W<=w){
            cout<<"jump out of code("<<w<<","<<h<<")"<<endl;
            err=-1;
          }
        }
        else{
          cout<<"stack is empty(.)"<<endl;
          err=-1;
        }
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        st.emplace_back(code[h*W+w]-'0');
        reg.back()[0]++;
        break;
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
        st.emplace_back(code[h*W+w]-'a'+10);
        reg.back()[0]++;
        break;
      case '+':
      case '-':
      case '*':
      case ',':
      case '%':
      case '=':
      case '(':
      case ')':
        if(reg.back()[0]>=2){
          t=st.back();st.pop_back();
          switch(code[h*W+w]){
            case '+':st.back()+=t;break;
            case '-':st.back()-=t;break;
            case '*':st.back()*=t;break;
            case ',':
              if(st.back()%t){
                cout<<"result of division is not integer"<<endl;
                err=-1;
              }
              st.back()/=t;
              break;
            case '%':st.back()%=t;break;
            case '=':st.back()=st.back()==t;break;
            case '(':st.back()=st.back()<t;break;
            case ')':st.back()=st.back()>t;break;
          }
          reg.back()[0]--;
        }
        else{
          cout<<"stack is shortage("<<char(code[h*W+w])<<")"<<endl;
          err=-1;
        }
        break;
      case '\'':
      case '\"':
        t=code[h*W+w];
        while(true){
          h+=dh[d];
          w+=dw[d];
          if(h<0)h+=H;
          if(H<=h)h-=H;
          if(w<0)w+=W;
          if(W<=w)w-=W;
          if(code[h*W+w]==t)break;
          st.emplace_back(code[h*W+w]);
          reg.back()[0]++;
          if(!st.back())st.back()=' ';
        }
        break;
      case ':':
        if(reg.back()[0]>=1){
          st.emplace_back(st.back());
          reg.back()[0]++;
        }
        else{
          cout<<"stack is empty(:)"<<endl;
          err=-1;
        }
        break;
      case '~':
        if(reg.back()[0]>=1){
          st.pop_back();
          reg.back()[0]--;
        }
        else{
          cout<<"stack is empty(~)"<<endl;
          err=-1;
        }
        break;
      case '$':
        if(reg.back()[0]>=2){
          swap(*st.rbegin(),*(st.rbegin()+1));
        }
        else{
          cout<<"stack is shortage($)"<<endl;
          err=-1;
        }
        break;
      case '@':
        if(reg.back()[0]>=3){
          swap(*st.rbegin(),*(st.rbegin()+1));
          swap(*(st.rbegin()+1),*(st.rbegin()+2));
        }
        else{
          cout<<"stack is shirtage(@)"<<endl;
          err=-1;
        }
        break;
      case '}':
        if(reg.back()[0]){
          t=st.back();st.pop_back();
          st.emplace(st.end()+(1-reg.back()[0]),t);
        }
        break;
      case '{':
        if(reg.back()[0]){
          auto i=st.end()-reg.back()[0];
          t=*i;
          st.erase(i);
          st.emplace_back(t);
        }
        break;
      case 'r':
        reverse(st.rbegin(),st.rbegin()+reg.back()[0]);
        break;
      case 'l':
        st.emplace_back();
        reg.back()[0]++;
        break;
      case '[':
        if(reg.back()[0]>=1){
          t=st.back();st.pop_back();
          if(reg.back()[0]>t){
            reg.back()[0]-=t+1;
            reg.emplace_back();
            reg.back()[0]=t;
          }
          else{
            cout<<"stack is shortage"<<endl;
            err=-1;
          }
        }
        else{
          cout<<"can't read stack num"<<endl;
          err=-1;
        }
        break;
      case ']':
        if(reg.size()==1){
          st.clear();
          reg.clear();
          reg.emplace_back();
        }
        else{
          t=reg.back()[0];
          reg.pop_back();
          reg.back()[0]+=t;
        }
        break;
      case 'o':
        if(reg.back()[0]>=1){
          output<<char(st.back());
          st.pop_back();
          reg.back()[0]--;
        }
        else{
          cout<<"stack is empty(o)"<<endl;
          err=-1;
        }
        break;
      case 'n':
        if(reg.back()[0]>=1){
          output<<st.back();
          st.pop_back();
          reg.back()[0]--;
        }
        else{
          cout<<"stack is empty(n)"<<endl;
          err=-1;
        }
        break;
      case 'i':
        if (input.size()){
          st.emplace_back(input.back());
          input.pop_back();
        }
        else st.emplace_back(-1);
        reg.back()[0]++;
        break;
      case '&':
        if(reg.back()[1]){
          reg.back()[1]=0;
          st.emplace_back(reg.back()[2]);
          reg.back()[0]++;
        }
        else{
          if(reg.back()[0]>=1){
            reg.back()[1]=1;
            reg.back()[2]=st.back();
            st.pop_back();
            reg.back()[0]--;
          }
          else{
            cout<<"stack is empty(&)"<<endl;
            err=-1;
          }
        }
        break;
      case 'g':
        if(reg.back()[0]>=2){
          t=st.back();st.pop_back();
          tt=st.back();
          if(0<=t&&t<H&&0<=tt&&tt<W)
            st.back()=code[t*W+tt];
          else{
            bitset<128>key1=t,key2=tt;
            key1=key1<<64|key2;
            auto i=out_of_code.find(key1);
            if(i==out_of_code.end())
              st.back()=0;
            else
              st.back()=i->second;
          }
          reg.back()[0]--;
        }
        else{
          cout<<"stack is empty(g)"<<endl;
          err=-1;
        }
        break;
      case 'p':
        if(reg.back()[0]>=3){
          t=st.back();st.pop_back();
          tt=st.back();st.pop_back();
          if(0<=t&&t<H&&0<=tt&&tt<W){
            code[t*W+tt]=st.back();st.pop_back();
          }
          else{
            bitset<128>key1=t,key2=tt;
            key1=key1<<64|key2;
            out_of_code[key1]=st.back();st.pop_back();
          }
          reg.back()[0]-=3;
        }
        else{
          cout<<"stack is empty(p)"<<endl;
          err=-1;
        }
        break;
      case ';':
        err=1;
        break;
      default:
        cout<<"unknown instruction"<<endl;
        err=-1;
        break;
    }
    //error
    if(err){
      switch (err) {
        case -1:
          cout<<"error at("<<w<<","<<h<<")"<<endl;
        case 1:
          cout<<"~~end execution~~"<<endl;
      }
      break;
    }
    //display
    if(--stop==0){
      draw();
      cin>>stop;
    }
    //move
    h+=dh[d];
    w+=dw[d];
    if(h<0)h+=H;
    if(H<=h)h-=H;
    if(w<0)w+=W;
    if(W<=w)w-=W;
    step++;
  }
  //display
  cout << "----final result----" << endl;
  draw();
  delete[]code;
  return 0;
}
