/*
Alice's mooncake shop
Time Limit: 1000MS 		Memory Limit: 32768KB

Description
The Mid-Autumn Festival, also known as the Moon Festival or Zhongqiu Festival is a popular harvest festival celebrated by Chinese people, dating back over 3,000 years to moon worship in China's Shang Dynasty. The Zhongqiu Festival is held on the 15th day of the eighth month in the Chinese calendar, which is in September or early October in the Gregorian calendar. It is a date that parallels the autumnal equinox of the solar calendar, when the moon is at its fullest and roundest.

The traditional food of this festival is the mooncake. Chinese family members and friends will gather to admire the bright mid-autumn harvest moon, and eat mooncakes under the moon together. In Chinese, “round”(圆) also means something like “faultless” or “reuion”, so the roundest moon, and the round mooncakes make the Zhongqiu Festival a day of family reunion.

Alice has opened up a 24-hour mooncake shop. She always gets a lot of orders. Only when the time is K o’clock sharp( K = 0,1,2 …. 23) she can make mooncakes, and We assume that making cakes takes no time. Due to the fluctuation of the price of the ingredients, the cost of a mooncake varies from hour to hour. She can make mooncakes when the order comes,or she can make mooncakes earlier than needed and store them in a fridge. The cost to store a mooncake for an hour is S and the storage life of a mooncake is T hours. She now asks you for help to work out a plan to minimize the cost to fulfill the orders.

Input
The input contains no more than 10 test cases.
For each test case:
The first line includes two integers N and M. N is the total number of orders. M is the number of hours the shop opens.
The next N lines describe all the orders. Each line is in the following format:

month date year H R

It means that on a certain date, a customer orders R mooncakes at H o’clock. “month” is in the format of abbreviation, so it could be "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov" or "Dec". H and R are all integers.
All the orders are sorted by the time in increasing order.
The next line contains T and S meaning that the storage life of a mooncake is T hours and the cost to store a mooncake for an hour is S.
Finally, M lines follow. Among those M lines, the ith line( i starts from 1) contains a integer indicating the cost to make a mooncake during the ith hour . The cost is no more than 10000. Jan 1st 2000 0 o'clock belongs to the 1st hour, Jan 1st 2000 1 o'clock belongs to the 2nd hour, …… and so on.

(0<N <= 2500; 0 < M,T <=100000; 0<=S <= 200; R<=10000 ; 0<=H<24)

The input ends with N = 0 and M = 0.
 

Output
You should output one line for each test case: the minimum cost.

Sample Input
1 10 
Jan 1 2000 9 10 
5 2 
20 
20 
20 
10 
10 
8 
7
9 
5 
10 
0 0
 

Sample Output
70

Hint

“Jan 1 2000 9 10” means in Jan 1st 2000 at 9 o'clock , there's a consumer ordering 10 mooncakes. 
Maybe you should use 64-bit signed integers. The answer will fit into a 64-bit signed integer.
 
*/
 


//Result:2012-10-04 19:56:05	Accepted	4122	218MS	8248K	2717 B	G++	Wizmann
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <map>

using namespace std;

#define print(x) cout<<x<<endl
#define input(x) cin>>x
#define ORDER 2560
#define SIZE 100010

typedef long long llint;

int query,n;

namespace DATE
{
	int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	struct date
	{
		int year,month,day,hour;
		date(){}
		date(int iyear,int imonth,int iday,int ihour)
		{
			year=iyear;month=imonth,day=iday;hour=ihour;
		}
	};
	
	const date START_TIME=date(2000,1,1,0);
	
	inline bool leap(int year)
	{
		return (year%4==0 && year%100!=0) || year%400==0;
	}


	int date2int(const date &a)
	{
		int ret=a.year*365+(a.year-1)/4-(a.year-1)/100+(a.year-1)/400;
		days[1]+=leap(a.year);
		for(int i=0;i<a.month-1;i++) ret+=days[i];
		days[1]=28;
		return ret+a.day;
	}

	int date2hour(const date& a)
	{
		int days=date2int(a)-date2int(START_TIME);
		return days*24+a.hour+1;
	}
}

namespace RMQ
{
	enum{RMQSIZE=20};
	int mini[SIZE][RMQSIZE];
	int pow2[RMQSIZE];
	void init()
	{
		memset(mini,0,sizeof(mini));
		for(int i=0;i<RMQSIZE;i++) pow2[i]=1<<i;
	}
	
	int log2(int x)
	{
		return upper_bound(pow2,pow2+RMQSIZE,x)-pow2-1;
	}
	void initRMQ()
	{
		int m=log2(n);
		for(int i=1;i<=m;i++)
		{
			for(int j=n;j>=1;j--)
			{
				mini[j][i]=mini[j][i-1];
				if(j+(1<<(i-1))<=n) mini[j][i]=min(mini[j][i],mini[j+(1<<(i-1))][i-1]);
			}
		}
	}

	int query_min(int l,int r)
	{
		int m=log2(r-l+1);
		return min(mini[l][m],mini[r-(1<<m)+1][m]);
	}
}

int order_num[ORDER];
int order_time[ORDER];
int store,cost;
map<string,int> str2month;

void init()
{
	str2month["Jan"]=1;
	str2month["Feb"]=2;
	str2month["Mar"]=3;
	str2month["Apr"]=4;
	str2month["May"]=5;
	str2month["Jun"]=6;
	str2month["Jul"]=7;
	str2month["Aug"]=8;
	str2month["Sep"]=9;
	str2month["Oct"]=10;
	str2month["Nov"]=11;
	str2month["Dec"]=12;
}
	

int main()
{
	freopen("input.txt","r",stdin);
	init();
	char month[10];
	int day,year,hour,ask;
	while(input(query>>n) && query+n)
	{
		for(int i=0;i<query;i++)
		{
			scanf("%s%d%d%d%d",month,&day,&year,&hour,&ask);
			int mm=str2month[month];
			order_time[i]=DATE::date2hour(DATE::date(year,mm,day,hour));
			order_num[i]=ask;
			//print(order_time[i]);
		}
		input(store>>cost);
		RMQ::init();
		for(int i=1;i<=n;i++)
		{
			scanf("%d",&ask);
			RMQ::mini[i][0]=(n-i)*cost+ask;
		}

		RMQ::initRMQ();
		llint ans=0;
		for(int i=0;i<query;i++)
		{
			int ll=max(1,order_time[i]-store+1);
			int rr=order_time[i];
			llint mincost=RMQ::query_min(ll,rr)-(n-order_time[i])*cost;
			ans+=mincost*order_num[i];
		}
		print(ans);
	}
	return 0;
}

