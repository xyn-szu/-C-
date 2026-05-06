#include <iostream>
#include <fstream>
#include <string>
#include <vector>  //这里根据ai推荐，使用vector实现包裹类的动态列表 ，比用new和dlete更加方便 
#include <iomanip>
#include <cstdlib>  // 用于清屏函数
#include <limits>   // 用于清除输入缓冲区
#include <sstream>  // 用于字符串流
using namespace std; 
void clearScreen() {  //清屏函数 ai给出 
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
const int minBattery=15;  //机器人电量低于15就要充电 
const int sdian=10;
const int bdian=15;
const double smaxweight=20.0;
const double bmaxweight=50.0;
const double smaxtiji=2000000;
const double bmaxtiji=4000000;
enum Package_State{  //枚举包裹状态 
	pending,  //待处理
	token,    //已装载 
	delivering,  //配送中 
	waiting_pickup,  //待取件 
	picked_up   // 已取件 
}; 
enum Robot_State{
	leisure,  //空闲 
	Rdelivering,  //派送中 
	charging,  //充电中 
	low_power   //电量过低 
}; 
class Package{
	string packagenumber ;   // 单号
	string name;  //收件人姓名
	string phone;  //收件人电话
	string address;   //收件人地址 
	double width;        //快递的宽 
	double length;      //快递的长 
	double height;      //快递的高 
	double weight;      //快递的重量 
	string ma;          //取件码 
	Package_State Pstate;    //快递状态 
public:
    Package(){   //包裹类的默认构造函数 
    	packagenumber="#";
    	width=0;
    	length=0;
    	height=0;
    	weight=0;
    	ma="#";
    	Pstate=pending;
		string address="#";
		string name="#";
		string phone="#";
	} 
	//带参数的构造函数 (单号，姓名，电话，地址，宽，长，高，重量，状态)
	Package(string pnum,string recname,string recphone,string recadress,double w,double l,double h,double m,string mm,Package_State ps); 
	//复制构造函数
	Package(const Package &p1);
	//==运算符重载函数，用取件码查找包裹
	bool operator==(string ma1)
	{
		return this->ma == ma1; 
	 } 
	 //包裹体积 
	double tiji()
	{
	   return width*length*height;	
    }
    //接口函数
	string getpackagenumber() {return packagenumber;} 
	string getma() {return ma;}
	string getname() {return name;}
	string getphone() {return phone;}
	string getaddress() {return address;}
	double getwidth() {return width;}
	double getlength() {return length;}
	double getheight() {return height;}
	double getweight() {return weight;}
	//设置包裹状态函数（询问ai后的结果） 
	void setPstate(Package_State newPstate) { Pstate=newPstate;}
	//状态与字符串转换 
	string getPstate()
	{
		switch (Pstate)
		{
			case pending:return "待处理";break;
			case delivering:return "派送中";break;
			case waiting_pickup:return "待取件"; break;
			case picked_up:return "已取件";break; 
			case token :return "已装载";break; 
		} 
	}
	//显示函数
	void Show();
	// 用于保存到文件 （询问ai后修改得到的代码） 
	void save(ofstream &ofs);
    //用于读取文件
	void read(ifstream &ifs); 
	 
}; 
Package::Package(string pnum,string recname,string recphone,string recadress,double w,double l,double h,double m,string mm,Package_State ps)
{
		packagenumber=pnum;
		name =recname;
		phone=recphone;
		address=recadress;
		width=w;
		length=l;
		height=h;
		weight=m;
		ma=mm; 
		Pstate=ps;
}
Package::Package(Package const&p1)
{
		packagenumber=p1.packagenumber;
		width=p1.width;
		length=p1.length;
		height=p1.height;
		weight=p1.weight;
		ma=p1.ma;
		name=p1.name;
		phone=p1.phone;
		address=p1.address; 
		Pstate=p1.Pstate;
} 
void Package::Show()
{
	    cout<<"包裹单号："<<packagenumber<<endl;
		cout<<"收件人姓名："<<name<<' '<<"收件人手机号码："<<phone<<endl;
		cout<<"收件地址："<<address<<endl;
		cout<<"包裹体积："<<length<<'*'<<width<<'*'<<height<<'='<<tiji()<<' '<<"包裹重量："<<weight<<endl;
		cout<<"取件码："<<ma<<endl;
		cout<<"包裹状态："<<getPstate()<<endl;
} 
//参考练习1 ：//格式：包裹单号 姓名 电话 地址  宽度 长度 高度 重量 取件码  状态 
void Package::save(ofstream &ofs)  
{
	    ofs << packagenumber <<"\t"  //保存包裹单号 
        << name <<"\t"     //保存姓名 
        << phone <<"\t"     //保存电话 
        << address <<"\t"   //保存地址 
        << width <<"\t"   // 保存宽度
        << length <<"\t"  // 保存长度
        << height <<"\t"   // 保存高度
        << weight <<"\t"    //保存重量 
        << ma <<"\t"        //保存取件码 
        << getPstate() << endl;  //保存状态 
}
//	//格式：包裹单号 姓名 电话 地址  宽度 长度 高度 重量 取件码  状态 （这里是ai给出建议后的代码） 
void Package::read(ifstream &ifs)
{
    string pnb, nam, pho, adr, code;
    double w, l, h, m;  
    // 使用更简单的读取方式，避免setw对齐问题
    ifs >> pnb >> nam >> pho >> adr >> w >> l >> h >> m >> code;   
    // 检查读取是否成功
    if (ifs.fail()) {
        // 重置对象状态
        *this = Package();
        return;
    }    
    packagenumber = pnb;
    name = nam;
    phone = pho;
    address = adr;
    ma = code;
    width = w;
    length = l;
    height = h;
    weight = m;
    
    string stateStr;
    ifs >> stateStr;
    
    // 状态转换
    if (stateStr == "待处理") {
        Pstate = pending;
    } else if (stateStr == "派送中") {
        Pstate = delivering;
    } else if (stateStr == "已取件") {
        Pstate = picked_up;
    } 
	else if(stateStr=="已装载"){
	Pstate = token;
	}else {
        Pstate = waiting_pickup; // 默认为待取件
    }
}
//机器人基类 
class Robot{
protected:
	string robotnumber;   //机器人编号 
	int battery;          //电量0-100
	string  position;   //当前位置；
	Robot_State Rstate;  //机器人状态
public:
	//默认构造函数 
     Robot() {
     	robotnumber="#";
		battery=100; 
		position="驿站";
		Rstate=leisure;
	 }
	 //带参数的构造函数
	 Robot(string robotnum,int bb,string pp,Robot_State rs) ;
	 //复制构造函数
	 Robot(const Robot &r0); 
	 //析构函数
	 virtual ~Robot() { }
	 //配送函数.询问ai后改用纯虚函数，这样子类中必须有这个函数的定义程序才能正确运行 
	virtual void deliver(vector <Package> &allbaoguo)=0;  
	virtual string getRstate(){
		switch (Rstate)
		{
			case leisure:return "空闲";break;
			case Rdelivering:return "派送中";break;
			case charging:return "充电中"; break;
			case low_power:return"电量过低";break; 
		} 
	}
	//检查电量
	virtual void checkbattery(); 
	//电量消耗
    virtual	void consume(int dian);
	//充电函数
	virtual void charge(); 
	//显示函数 
	virtual void Show(); 
	
	//保存文件
	virtual void save(ofstream &ofs);
	//读取文件
	virtual void read(ifstream &ifs, vector<Package> &packages) = 0;
	
	//接口函数
	string getrobotnumber() {return robotnumber;}
	int getbattery() {return battery;}
	string getposition() {return position;} 
	//修改机器人的当前位置 
	void setposition(string newp) {position=newp;}
	//设置机器人状态 
	void setRstate(Robot_State newrs) {Rstate=newrs;}

	
};

//robot带参数的构造函数 
Robot::Robot(string robotnum,int bb,string pp,Robot_State rs)
{
	robotnumber=robotnum;
	battery=bb;
	position=pp;
	Rstate=rs;
}
//robot复制构造函数 
Robot::Robot(const Robot &r0)
{
	robotnumber=r0.robotnumber;
	battery=r0.battery;
	position=r0.position;
	Rstate=r0.Rstate;
}
void Robot::Show()
{
     cout<<"机器人编号："<<robotnumber<<endl;
	 cout<<"机器人当前电量："<<battery<<endl;
	 cout<<"机器人当前位置："<<position<<endl;
	 cout<<"机器人当前状态："<<getRstate()<<endl;	
} 
//保存到文件 格式：机器人编号 电量 剩余载重  剩余体积  位置 状态  
void Robot::save(ofstream &ofs)
{
   	 ofs<<robotnumber<<"\t"<< battery<<"\t"<<position<<"\t"<<getRstate()<<endl;
}
void Robot::consume(int dian)
{
	battery=battery-dian;
	if(battery<0) battery=0;
	if(battery<minBattery){
		Rstate=low_power;

	} 
	cout<<"机器人"<<robotnumber<<" 剩余电量："<<battery<<endl ;
}

//充电 
void Robot::charge()
{
	if(getRstate()!="充电中")
	{
		Rstate=charging;
		position="驿站";	
	battery=100;
	}
	if(battery==100&&Rstate==charging)
    {
    	Rstate=leisure;
    	cout<<"机器人："<<robotnumber<<"充电完成！"<<endl;
	}
	
}
//检查电量
void Robot:: checkbattery()
{
	if(battery<=minBattery&&Rstate!=charging)
	{
		Rstate=low_power;
		position="驿站"; 
		cout<<"机器人："<<robotnumber<<"电量过低"<<endl; 
	}
	else 
	    return ;
}
//小型机器人
class SmallRobot:public Robot{
private:
    double maxweight;   //最大载重10kg
    double maxtiji;     //最大体积5000立方厘米 
    double nowweight;
	double nowtiji; 
	vector <Package> packageslist;  //
public:
//默认构造函数
SmallRobot() : Robot() {
	maxweight=smaxweight;maxtiji=smaxtiji;nowweight=smaxweight;nowtiji=smaxtiji;
}
//带参数的构造函数
SmallRobot(string Srobotnum,int Sba,string Sp,Robot_State Srs) :Robot(Srobotnum,Sba,Sp,Srs)
 {	maxweight=smaxweight;maxtiji=smaxtiji;nowweight=smaxweight;nowtiji=smaxtiji;}
//复制构造函数
SmallRobot(const SmallRobot &sr):Robot(sr){
	maxweight=sr.maxweight;
	maxtiji=sr.maxtiji;
	packageslist=sr.packageslist ;
	nowtiji=sr.nowtiji ;
	nowweight=sr.nowweight;
} 
//析构函数
~SmallRobot() {}
//装载函数
void put(Package &p1); 
//配送函数
void deliver(vector <Package> &allbaoguo); 
//显示函数
void Show();
//接口函数
double getmaxweight() {return maxweight;}
double getmaxtiji()  {return maxtiji;}
double getnowweight() {return nowweight;}
double getnowtiji() {return nowtiji;}
//写入文件
void  save(ofstream &ofs);
// 读取文件
void read(ifstream &ifs,vector<Package> &packages); 
}; 
void SmallRobot::put(Package &p1)
{
	Package_State t=token;
	if(p1.getPstate()=="待处理")
	{
	if(p1.getweight()>nowweight||p1.tiji() >nowtiji)
    {
        cout<<"超重！小型机器人"<<robotnumber<<"无法配送"<<endl; 
        return;
    } 
    else
    {
    packageslist.push_back(p1); 
	p1.setPstate(t); 
    nowweight=nowweight-p1.getweight();
    nowtiji=nowtiji-p1.tiji();
	}
    } 
} 
void SmallRobot::deliver(vector <Package> &allbaoguo)
{   
    Package_State d=delivering;
    Package_State p=pending;
    Package_State t=token;
    Package_State w=waiting_pickup;
    Robot_State le=leisure;
    if (packageslist.empty()) {
        cout << "小型机器人" << robotnumber << "没有需要配送的包裹" << endl;
        Rstate = leisure;
        position = "驿站";
        return;
    }
    if(Rstate==charging) 
    {
        cout<<"小型机器人"<<robotnumber<<"正在充电"<<endl; 
        return; 
    }
    if(Rstate==low_power)
	{
		cout<<"小型机器人"<<robotnumber<<"电量过低"<<endl;
		return ; 
	 } 
   while(!packageslist.empty() )
   {
   	Package nowpackage=packageslist.front() ;
   	    if (battery < minBattery) {
            cout << "电量不足，停止配送" << endl;
            Rstate = low_power;
            break;
        }
    Rstate=Rdelivering;
    nowpackage.setPstate(d);
    cout<<"小型机器人"<<robotnumber<<"正在配送包裹"<<nowpackage.getpackagenumber()<<endl;
    consume(sdian);
    setposition(nowpackage.getaddress());
    cout<<"小型机器人"<<robotnumber<<"配送完成包裹："<<nowpackage.getpackagenumber()<<endl;
    cout<<"包裹"<<nowpackage.getpackagenumber()<<"待取件"<<endl;
    nowpackage.setPstate(w);
    nowweight=nowweight+nowpackage.getweight();
    nowtiji=nowtiji+nowpackage.tiji();
    for(size_t i=0;i<allbaoguo.size() ;i++)
		{
		if(allbaoguo[i].getpackagenumber()==nowpackage.getpackagenumber() )
			    allbaoguo[i].setPstate(w);
		} 
    packageslist.erase(packageslist.begin()) ;  

    } 
// 所有包裹配送完成，返回驿站
    if (packageslist.empty()) {
        position = "驿站";
        Rstate = leisure;
        cout << "小型机器人" << robotnumber << "配送任务完成，返回驿站" << endl;
        nowweight=maxweight;
		nowtiji=maxtiji; 
    }
  
    checkbattery();   
} 
void SmallRobot::Show()
{
	 cout<<"小型机器人编号："<<robotnumber<<endl;
	 cout<<"小型机器人的最大载重："<<maxweight<<' '<<"最大体积"<<maxtiji<<endl;
	 cout<<"小型机器人的当前可载重："<<nowweight<<' '<<"当前可载体积"<<nowtiji<<endl; 
	 cout<<"小型机器人当前配送包裹数量:"<< packageslist.size()<<endl;
	 if(!packageslist.empty())
	 {
	 	cout<<"当前配送列表："<<endl;
		 for(size_t i=0;i<packageslist.size() ;i++)
		 {
		 	cout<<"包裹"<<packageslist[i].getpackagenumber()<<"的取件码为："<<packageslist[i].getma()<<endl; 
		  } 
	  } 
	 cout<<"小型机器人当前电量："<<battery<<endl;
	 cout<<"小型机器人当前位置："<<position<<endl;
	 cout<<"小型机器人当前状态："<<getRstate()<<endl;
	 cout<<"\n========================================"<<endl; 
}
//保存文件 
void SmallRobot::save(ofstream &ofs)
{
    	ofs<<robotnumber<<"\t"<< battery<<"\t"<<nowweight<<"\t"<<nowtiji<<"\t"<<position<<"\t"<<getRstate()<<"\t";
		ofs<<packageslist.size()<<"\t";
	 for(size_t i=0;i<packageslist.size() ;i++)
	 {
			ofs<<packageslist[i].getpackagenumber()<<"\t";
	}
		ofs<<endl;
}
//读取文件 
void SmallRobot::read(ifstream &ifs,vector<Package> &packages)
{
	string rnum,pos,rstateStr;
	int bat,pkgCount;
	double nw,nt;
	ifs >> rnum >> bat >>nw>>nt>> pos >> rstateStr >> pkgCount;
    robotnumber = rnum;
    battery = bat;
    position = pos;
    nowweight=nw;
    nowtiji=nt;
    // 转换状态字符串为枚举类型
    if (rstateStr == "空闲") {
        Rstate = leisure;
    } else if (rstateStr == "配送中") {
        Rstate = Rdelivering;
    } else if (rstateStr == "充电中") {
        Rstate = charging;
    }
    else 
       Rstate=low_power;
    // 读取包裹列表（根据编号从已有包裹中匹配）
        // 检查读取是否成功
    if (ifs.fail()) {
        // 重置对象状态
        *this = SmallRobot();
        return;
    }
    packageslist.clear();
    for (size_t i = 0; i < pkgCount; ++i) {
        string pnum;
        ifs >> pnum;
        // 查找对应编号的包裹
        for(size_t i=0;i<packages.size() ;i++)
      {
            if (packages[i].getpackagenumber() == pnum) {
                packageslist.push_back(packages[i]);
                break;
            }
        }
    }
}

//大型机器人
class BigRobot:public Robot{
private:
    double maxweight;   //最大载重20kg
    double maxtiji;   //最大体积10000立方厘米 
    double nowweight;
    double nowtiji;
	vector <Package> packageslist;  //
public:
//默认构造函数
BigRobot() : Robot(),maxweight(bmaxweight),maxtiji(bmaxtiji),nowweight(bmaxweight),nowtiji(bmaxtiji){}
//带参数的构造函数
BigRobot(string Brobotnum,int Bba,string Bp,Robot_State Brs) :Robot(Brobotnum,Bba,Bp,Brs),maxweight(bmaxweight),maxtiji(bmaxtiji),nowweight(bmaxweight),nowtiji(bmaxtiji) { }
//复制构造函数
BigRobot(const BigRobot &br):Robot(br){
	maxweight=br.maxweight;
	maxtiji=br.maxtiji;
	packageslist=br.packageslist ;
	nowtiji=br.nowtiji ;
	nowweight=br.nowweight ;
} 
//析构函数
~ BigRobot() {} 
//装载函数
void put(Package &p1); 
//配送函数
void deliver(vector <Package> &allbaoguo); 
//显示函数
void Show();
//接口函数
double getmaxweight() {return maxweight;}
double getmaxtiji()  {return maxtiji;}
double getnowweight() {return nowweight;}
double getnowtiji() {return nowtiji;}
//写入文件
void  save(ofstream &ofs);
void read(ifstream &ifs, vector<Package> &packages);
}; 
void BigRobot::put(Package &p1)
{
	Package_State t=token;
	if(p1.getweight()>nowweight||p1.tiji() >nowtiji)
    {
        cout<<"超重！大型机器人无法配送"<<endl; 
        return;
    } 
    else
    {
    packageslist.push_back(p1);
    p1.setPstate(t);
    nowweight=nowweight-p1.getweight();
    nowtiji=nowtiji-p1.tiji();
	}
} 
void BigRobot::deliver(vector<Package> &allbaoguo)
{   
    Package_State d=delivering;
    Package_State p=pending;
    Package_State w=waiting_pickup;
    Robot_State le=leisure;
    if (packageslist.empty()) {
        cout << "大型机器人" << robotnumber << "没有需要配送的包裹" << endl;
        Rstate = leisure;
        position = "驿站";
        return;
    }
    if(Rstate==charging) 
    {
        cout<<"大型机器人"<<robotnumber<<"正在充电"<<endl; 
        packageslist.back().setPstate(p);
        return; 
    }
    if(Rstate==low_power)
	{
		cout<<"大型机器人"<<robotnumber<<"电量过低"<<endl;
		packageslist.back().setPstate(p);
		return ; 
	 } 
   while(!packageslist.empty() )
   {
   	Package nowpackage=packageslist.front() ;
   	    if (battery < minBattery) {
            cout << "电量不足，停止配送" << endl;
            Rstate = low_power;
            break;
        }
    Rstate=Rdelivering;
   nowpackage.setPstate(d);
    cout<<"大型机器人"<<robotnumber<<"正在配送包裹"<<nowpackage.getpackagenumber()<<endl;
    consume(bdian);
    setposition(nowpackage.getaddress());
    cout<<"大型机器人"<<robotnumber<<"配送完成包裹："<<nowpackage.getpackagenumber()<<endl;
    cout<<"包裹"<<nowpackage.getpackagenumber()<<"待取件"<<endl;
    nowpackage.setPstate(w);
    nowweight=nowweight+nowpackage.getweight();
    nowtiji=nowtiji+nowpackage.tiji();
     for(size_t i=0;i<allbaoguo.size() ;i++)
		 {
		 	if(allbaoguo[i].getpackagenumber()==nowpackage.getpackagenumber() )
		 	    allbaoguo[i].setPstate(w);
		  } 
    packageslist.erase(packageslist.begin()) ;  
	
	}
    // 所有包裹配送完成，返回驿站
    if (packageslist.empty()) {
        position = "驿站";
        Rstate = leisure;
        cout << "大型机器人" << robotnumber << "配送任务完成，返回驿站" << endl;
        nowweight=maxweight;
		nowtiji=maxtiji; 
    }
  
    checkbattery();   
} 
void BigRobot::Show()
{
	 cout<<"大型机器人编号："<<robotnumber<<endl;
	 cout<<"大型机器人的最大载重："<<maxweight<<' '<<"最大体积"<<maxtiji<<endl;
	 cout<<"大型机器人的当前可载重："<<nowweight<<' '<<"当前可载体积"<<nowtiji<<endl; 
	 cout<<"大型机器人当前配送包裹数量:"<< packageslist.size()<<endl;
	 if(!packageslist.empty())
	 {
	 	cout<<"当前配送列表："<<endl;
		 for(size_t i=0;i<packageslist.size() ;i++)
		 {
		 	cout<<"包裹"<<packageslist[i].getpackagenumber()<<"的取件码为："<<packageslist[i].getma()<<endl; 
		  } 
	  } 
	 cout<<"大型机器人当前电量："<<battery<<endl;
	 cout<<"大型机器人当前位置："<<position<<endl;
	 cout<<"大型机器人当前状态："<<getRstate()<<endl;
	 cout<<"\n========================================"<<endl; 
}
//保存文件 
void BigRobot::save(ofstream &ofs)
{
    	ofs<<robotnumber<<"\t"<< battery<<"\t"<<nowweight<<"\t"<<nowtiji<<"\t"<<position<<"\t"<<getRstate()<<"\t";
		ofs<<packageslist.size()<<"\t";
		 for(size_t i=0;i<packageslist.size() ;i++)
		 {
			ofs<<packageslist[i].getpackagenumber()<<"\t";
		}
		ofs<<endl;
}
//读取文件 
void BigRobot::read(ifstream &ifs, vector<Package> &packages)
 {
	string rnum,pos,rstateStr;
	int bat,pkgCount;
	double nw,nt;
	ifs >> rnum >> bat >>nw>>nt>> pos >> rstateStr >> pkgCount;
    robotnumber = rnum;
    battery = bat;
    position = pos;
    nowweight=nw;
    nowtiji=nt;
    // 转换状态字符串为枚举类型
    if (rstateStr == "空闲") {
        Rstate = leisure;
    } else if (rstateStr == "配送中") {
        Rstate = Rdelivering;
    } else if (rstateStr == "充电中") {
        Rstate = charging;
    }
    else 
       Rstate=low_power;
        // 检查读取是否成功
    if (ifs.fail()) {
        // 重置对象状态
        *this =BigRobot();
        return;
    }
    packageslist.clear();
    for (size_t i = 0; i < pkgCount; ++i) {
        string pnum;
        ifs >> pnum;
        // 查找对应编号的包裹
        for(size_t i=0;i<packages.size() ;i++)
      {
            if (packages[i].getpackagenumber() == pnum) {
                packageslist.push_back(packages[i]);
                break;
            }
        }
    }
}
// 子菜单等待函数  ai给出 
void waitForReturn()
 {
    cout << "\n按 'q' 键返回主菜单: ";
    char ch;
    cin >> ch;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 清除输入缓冲区
    if (ch == 'q' || ch == 'Q') {
        return;
    }
}
//录入包裹信息函数 
void input(vector<Package> &packageslist)
{
	Package_State ps1=pending;  //默认录入的包裹都是待取件的 
	string danhao,name,phone,address,qujianma;
	double L,W,H,M;
	cout<<"包裹信息录入，在单号输入“quit”退出录入"<<endl;
	while(true)
	{
		cout<<"快递单号（两位英文+8位数字）：";
		cin>>danhao;
		if(danhao=="quit") //输入qiut后退出 
	    break ;
		cout<<"收件人姓名：";
		cin>>name;
		cout<<"收件人号码（11位）：";
		cin>>phone;
		cout<<"收件人地址：";
		cin>>address;
		cout<<"取件码(4位)：";
		cin>>qujianma;
		cout<<"包裹长(cm)：";
		cin>>L;
		cout<<"包裹宽（cm）";
		cin>>W;
		cout<<"包裹高（cm）";
		cin>>H;
		cout<<"包裹重量：";
		cin>>M; 

		//创建包裹类对象（带参数的构造函数） 
		//带参数的构造函数 (单号，姓名，电话，地址，宽，长，高，重量，状态)
		Package pack1(danhao,name,phone,address,W,L,H,M,qujianma,ps1);
		packageslist.push_back(pack1);  // 添加到列表
		cout<<"包裹"<<danhao<<"录入成功！"<<endl;
	 		 
	}
} 
// 包裹录入子函数
void inputPackagesMenu(vector<Package> &baoguolist) {
    clearScreen();
    cout << "========== 包裹信息录入 ==========" << endl;
    input(baoguolist);  // 调用原来的input函数
    waitForReturn();
}

//按取件码找包裹
Package *findPack(vector<Package> &pack2,string &qujianma1)
{
	for(size_t i=0;i<pack2.size() ;i++)
	 {
		if(pack2[i]==qujianma1)
		{
			return &pack2[i];
			
		}
	}
	cout<<"未找到！请从新输入。"<<endl;
	return NULL; 
 } 
 //取件码查找包裹显示
 void findpackages(vector<Package> &pack2) 
 {
 	cout<<"\n===============包裹查找==============="<<endl;
	 //输入取件码查找包裹
	 string code;
	 cout<<"输入取件码：";
	 cin>>code; 
	 Package *fp;
	 fp=findPack(pack2,code);
	 if(fp!=NULL)
	 {
	 	cout<<"找到包裹!包裹信息如下："<<endl;
	    (*fp).Show();
	}
	else 
     cout << "未找到取件码为 " << code << " 的包裹" << endl;
 }
 
 //添加机器人
 void addRobot( vector<SmallRobot> &smallrobot,vector<BigRobot> &bigrobot)
 {  
  	Robot_State rs=leisure;
    string type,hao;
 	cout<<"\n===============录入机器人信息==============="<<endl;
 	cout<<"输入要添加的机器人型号（S/B）" ;
 	cin>>type;
 	bool temp=true;
 	if(type=="S")
 	{
 		cout<<"输入小型机器人的编号(S+4位数字)：";
		 cin>>hao;
		 for(size_t i=0;i<smallrobot.size() ;i++)
		 {
		 	if(smallrobot[i].getrobotnumber()==hao)
		 	  temp=false;
		 }
		 if(temp){

		SmallRobot *sr1= new SmallRobot(hao,100,"驿站",rs);
		smallrobot.push_back(*sr1);
    	delete sr1;
    	sr1=NULL;
    	cout << "小型机器人 " << hao << " 添加成功！" << endl;	
			 }
		 else
			cout<<"小型机器人"<<hao<<"已存在！"<<endl; 
	 }
	else if(type=="B")
 	{
 		cout<<"输入大型机器人的编号(B+4位数字)：";
		 cin>>hao;
		for(size_t i=0;i<bigrobot.size() ;i++)
		 {
		 	if(bigrobot[i].getrobotnumber()==hao)
		 	  temp=false;
		 }
		 if(temp){
		BigRobot *br1= new BigRobot(hao,100,"驿站",rs);
		bigrobot.push_back(*br1);
    	delete br1;
    	br1=NULL;
    	cout << "大型机器人 " << hao << " 添加成功！" << endl;
		}
		else
		cout<<"大型机器人"<<hao<<"已存在！"<<endl; 
	 }
	 else
	 cout<<"输入错误!请重新输入."<<endl; 
	 waitForReturn();
  } 
  
//显示所有包裹信息 
void showpackages(vector <Package> &packages)
{
	cout<<"\n===============显示包裹信息==============="<<endl;
	 	for(size_t i=0;i<packages.size() ;i++)
	 {
		packages[i].Show();
	 cout<<"\n========================================="<<endl; 
	 }
	 cout << "总共 " << packages.size() << " 个包裹" << endl;
 } 
 
 //分配包裹列表给robot
 void fenpeiPL(vector <Package> &baoguolist,vector <SmallRobot> &srobot,vector <BigRobot> &brobot)
 {
   cout << "============ 自动分配机器人=============" << endl;

	 for(size_t i=0;i<srobot.size() ;i++)
	 { 	 	  
	  	   bool hasLoaded = false;  // 标记是否装载了包裹
	   for(size_t j=0;j<baoguolist.size() ;j++)
     	 {
		if(srobot[i].getRstate()=="空闲"&&baoguolist[j].getweight()<=srobot[i].getnowweight()&&baoguolist[j].tiji()<=srobot[i].getnowtiji()&&baoguolist[j].getPstate()=="待处理")
		{		
			srobot[i].put(baoguolist[j]);
 			cout<<"包裹"<<baoguolist[j].getpackagenumber()<<"分配给小型机器人："<<srobot[i].getrobotnumber()<<endl;
 			hasLoaded = true;
		}
	    }
	    if(hasLoaded){
			srobot[i].setRstate(Rdelivering);  // 将机器人状态设置为派送中
			cout<<"小型机器人："<<srobot[i].getrobotnumber()<< "装载完成"<<endl;
			}
     }
     	 for(size_t i=0;i<brobot.size() ;i++)
	 { 	 	
	 	   bool hasLoaded = false;
	   for(size_t j=0;j<baoguolist.size() ;j++)
     	 {
		if(brobot[i].getRstate()=="空闲"&&baoguolist[j].getweight()<=brobot[i].getnowweight()&&baoguolist[j].tiji()<=brobot[i].getnowtiji()&&baoguolist[j].getPstate()=="待处理")
		{		
			brobot[i].put(baoguolist[j]);
 			cout<<"包裹"<<baoguolist[j].getpackagenumber()<<"分配给大型机器人："<<brobot[i].getrobotnumber()<<endl;
 			hasLoaded = true;
		}
	    }
	    if(hasLoaded){
			brobot[i].setRstate(Rdelivering);  // 将机器人状态设置为派送中
			cout<<"大型机器人："<<brobot[i].getrobotnumber()<< "装载完成"<<endl;
			}
     }
	   for(size_t j=0;j<baoguolist.size() ;j++)
     	 {
     	 	if(baoguolist[j].getPstate()=="待处理")
			cout<<"没有可使用的机器人配送包裹："<<baoguolist[j].getpackagenumber()<<endl;
		 }
	 cout<<"\n========================================"<<endl; 
  } 
  
//开启机器人配送 
  void startDelivery(vector<SmallRobot> &smallrobot, vector<BigRobot> &bigrobot,vector<Package> &allbaoguo)
{
    cout << "\n===============开始配送===============" << endl;
    
    // 配送所有小型机器人
    for(size_t i = 0; i < smallrobot.size(); i++)
    {
        if (smallrobot[i].getRstate() == "派送中" && 
            smallrobot[i].getnowweight() < smallrobot[i].getmaxweight())
        {
            cout << "\n小型机器人 " << smallrobot[i].getrobotnumber() 
                 << " 开始配送：" << endl;
            smallrobot[i].deliver(allbaoguo);
            cout << "================================================" << endl;
        }
    }
    
    // 配送所有大型机器人
    for(size_t i = 0; i < bigrobot.size(); i++)
    {
        if (bigrobot[i].getRstate() == "派送中" && 
            bigrobot[i].getnowweight() < bigrobot[i].getmaxweight())
        {
            cout << "\n大型机器人 " << bigrobot[i].getrobotnumber() 
                 << " 开始配送：" << endl;
            bigrobot[i].deliver(allbaoguo);
            cout << "================================================" << endl;
        }
    }
    
    cout << "\n配送任务已完成！" << endl;
}
 //显示所有机器人信息
void showRobot( vector<SmallRobot> &smallrobot,vector<BigRobot> &bigrobot) 
{
	cout<<"\n===============显示机器人信息==============="<<endl;
	if (smallrobot.empty()) 
	{
         cout << "暂无小型机器人" << endl;
	}
    else 
    {
	for(size_t i=0;i<smallrobot.size() ;i++)
	 {
 		smallrobot[i].Show();
	 }
    } 
    if (bigrobot.empty()) 
	{
         cout << "暂无大型机器人" << endl;
	}
    else 
    {
	for(size_t i=0;i<bigrobot.size() ;i++)
	 {
 		bigrobot[i].Show();
	 } 
    }  
}
//输入机器人编号查找机器人
void findRobot(vector<SmallRobot> &srobot,vector<BigRobot> &brobot)
{
	    string hao;
		cout<<"\n===============查找机器人==============="<<endl;
		cout<<"请输入要查找的机器人编号：";
		cin>> hao;
	for(size_t i=0;i<srobot.size() ;i++)
	 {
	 	if(hao==srobot[i].getrobotnumber())
	 	{
	 		cout<<"查找到小型机器人："<<hao<<","<<"该小型机器人信息如下："<<endl;
		     srobot[i].Show();
		     return;
		 }
	 }
	for(size_t i=0;i<brobot.size() ;i++)
	 {
	 	if(hao==brobot[i].getrobotnumber())
	 	{
	 		cout<<"查找到大型机器人："<<hao<<","<<"该大型机器人信息如下："<<endl;
		     brobot[i].Show();
		     return;
		 }
	 }
	 cout<<"未找到编号为"<<hao<<"的机器人!"<<endl; 
	 
}
//机器人充电
void chargeRobot( vector<SmallRobot> &smallrobot,vector<BigRobot> &bigrobot) 
{
	cout<<"\n===============机器人充电==============="<<endl;
	for(size_t i=0;i<smallrobot.size() ;i++)
	 {
 		smallrobot[i].charge();
	 }
	 
	for(size_t i=0;i<bigrobot.size() ;i++)
	 {
 		bigrobot[i].charge();
	 } 
}
//模拟取件
void pickupPackage(vector<Package> &packagelist)
{
	cout<<"\n===============用户取件==============="<<endl;
	string code;
	cout<<"请输入取件码（4位）：";
	cin>>code ;
	
	Package *p0=findPack(packagelist,code);  //调用函数findPack根据取件码查找包裹 
	if(p0!=NULL) //
	{
		if((*p0).getPstate()=="待取件")
		{
			(*p0).setPstate(picked_up) ;
			cout<<"包裹"<<(*p0).getpackagenumber()<<"取件成功！"<<endl;
		 } 
		 else if((*p0).getPstate()=="已取件")
		 	cout<<"包裹"<<(*p0).getpackagenumber()<<"已被取走！"<<endl;
		else
		    cout<<"包裹"<<(*p0).getpackagenumber()<<"当前还不能取件！"<<endl; 
	}
	else
	     cout<<"取件码错误！"<<endl; 
}
//保存到文件
void savetofile(vector<SmallRobot> &smallrobot,vector<BigRobot> &bigrobot,vector <Package> &packages)
{
	cout<<"\n===============保存文件==============="<<endl;
	ofstream ofilep;
 	ofstream ofilerb;
 	ofstream ofilers;
 	ofilerb.open("bigrobot.txt");
  	ofilers.open("smallrobot.txt");	
 	ofilep.open("packages.txt");
 	//输入到robot文件 
	for(size_t i=0;i<smallrobot.size() ;i++)
	 {
 		smallrobot[i].save(ofilers) ;
	 }
	for(size_t i=0;i<bigrobot.size() ;i++)
	 {
 		bigrobot[i].save(ofilerb) ;
	 } 
	//输入到包裹文件 
	for(size_t i=0;i<packages.size() ;i++)
	 {
 		packages[i].save(ofilep) ;
	 }
	 ofilep.close();
	 ofilers.close();
	 ofilerb.close();
	cout<<"\n===============保存成功==============="<<endl;
}
//从文件加载包裹 
void readPackages(vector<Package> &packageslist, const string &filename) 
{
    ifstream ifile(filename.c_str());  
    if (!ifile.is_open()) {
        cout << "无法打开包裹文件，将创建新文件。" << endl;
        return;
    }   
    int count = 0;
    	Package p0;
    // 使用简单的while循环
    while (!ifile.eof() ) {       
        p0.read(ifile);
        // 检查是否读取成功
        if (p0.getpackagenumber() != "#" && !p0.getpackagenumber().empty()) {
            packageslist.push_back(p0);
            count++;
            cout << "读取包裹：" << p0.getpackagenumber() << endl;
        }
        else
		break; 

    }
    
    ifile.close();
    cout << "成功读取 " << count << " 个包裹" << endl;
}
//从文件加载小型机器人
void readSmallrobots(vector<SmallRobot> &smallrobotslist,vector<Package> &packagelist ,const string &filename) 
{
     ifstream ifile(filename.c_str()); 
    if (!ifile.is_open()) {
        cout << "无法打开小型机器人文件" << endl;
        return;
    }
   SmallRobot sr;
   int count=0;
    while (!ifile.eof()) {  // 只要不是文件结尾就继续读
       sr.read(ifile,packagelist) ;
    if (sr.getrobotnumber() != "#" && !sr.getrobotnumber().empty()) {
       smallrobotslist.push_back(sr);
       count++;
       cout << "小型机器人：" <<sr.getrobotnumber()<< endl;
   }
 
	}  
    ifile.close();
    cout << "成功读取 " << count << " 个小型机器人" << endl;
}
//从文件加载大型机器人
void readBigrobots(vector<BigRobot> &bigrobotslist,vector<Package> &packagelist, const string &filename) 
{
     ifstream ifile(filename.c_str()); 
    if (!ifile.is_open()) {
        cout << "无法打开小型机器人文件" << endl;
        return;
    }
   BigRobot br;
   int count=0;
    while (!ifile.eof()) {  // 只要不是文件结尾就继续读
       br.read(ifile,packagelist) ;
    if (br.getrobotnumber() != "#" && !br.getrobotnumber().empty()) {
       bigrobotslist.push_back(br);
       count++;
       cout << "大型机器人：" <<br.getrobotnumber()<< endl;
   }
 
	}  
    ifile.close();
    cout << "成功读取 " << count << " 个大型机器人" << endl;
}
 // 显示主菜单
void displayMenu() {
    cout << "\n===============智能快递配送系统===============" << endl;
    cout << "1.  添加包裹信息" << endl;
    cout << "2.  添加机器人" << endl;
    cout << "3.  显示所有包裹信息" << endl;
    cout << "4.  分配包裹给机器人" << endl;
    cout << "5.  开启机器人配送" <<endl; 
    cout << "6.  显示所有机器人信息" << endl;
    cout << "7.  输入机器人编号查找机器人"<<endl; 
    cout << "8.  按取件码查找包裹" << endl;
    cout << "9.  机器人充电" << endl;
    cout << "10.  取件（模拟用户取件）" << endl;
    cout << "11. 保存数据到文件" << endl;
    cout << "12. 从文件加载数据" << endl;
    cout << "0.  退出系统" << endl;
    cout << "================================================" << endl;
    cout << "请输入选项 (0-12): ";
}

 int main()
 {
 	vector<Package> baoguolist;
 	vector<SmallRobot> srobot;
 	vector<BigRobot> brobot;
 	string filenamep="packages.txt";
 	string filenameb="bigrobot.txt";
 	string filenames="smallrobot.txt";
    int choice;
 	while(choice!=0)
	{    
	system("cls");
	displayMenu();
	cin>>choice;
   	  switch(choice)
    	{
    		case 1:    system("cls");inputPackagesMenu (baoguolist);break;
    		case 2:    system("cls"); addRobot(srobot,brobot);break;
    		case 3:     system("cls");showpackages(baoguolist);waitForReturn();break;
    		case 4:   system("cls"); fenpeiPL(baoguolist,srobot,brobot);waitForReturn();break;
    	    case 5:   system("cls");startDelivery(srobot, brobot,baoguolist); waitForReturn(); break;
			case 6:    system("cls");showRobot(srobot,brobot) ;waitForReturn();break;	
			case 7:    system("cls");findRobot(srobot,brobot);  waitForReturn();break;
    		case 8:    system("cls");findpackages(baoguolist) ;waitForReturn();break;
    		case 9:   system("cls");chargeRobot( srobot,brobot) ;waitForReturn();break;
    		case 10:    system("cls");pickupPackage(baoguolist);waitForReturn();break;
    		case 11:   system("cls"); savetofile(srobot,brobot,baoguolist);waitForReturn();break;
    		case 12:   system("cls");readPackages(baoguolist, filenamep) ;readSmallrobots(srobot,baoguolist ,filenames) ;readBigrobots(brobot,baoguolist ,filenameb) ;waitForReturn();break;
    		case 0:    savetofile(srobot,brobot,baoguolist);    cout << "\n===============退出系统，信息已保存===============" << endl;break;
			default:   cout<<"未找到对应操作，请重新输入！"<<endl; 
		}
    }

    return 0; 
 }
 
