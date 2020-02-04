#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>
#include<string.h>

#define SIZE 78 	//78 array elements needed to store 308 digits and sign

typedef enum status_code {FAILURE,SUCCESS} SC;
typedef enum {NEGATIVE=-1,POSITIVE=1} sign;

typedef struct num_tag
{
	int number[SIZE];
	sign s;
}num;

int size()
{
	int i=INT_MAX;
	i=log(i)/(log(10)*2);     //i=4
	return i;
}

void initialise(num* n)
{
	int i;
	for(i=0;i<SIZE;i++)
	{
		n->number[i]=-1;
	}
}

SC intscan(num* n,int d)
{
	SC status=SUCCESS;
	int max_digits,len,i,count,a,b,j,factor;
	max_digits=309; 
	char* c=(char*)malloc(sizeof(char)*max_digits);
	scanf("%s",c);
	len=strlen(c);
	if(c[0]=='+')
	{
		n->s=POSITIVE;
	}
	else
	{
		n->s=NEGATIVE;
	}
	initialise(n);   
	j=0;
	i=len-1;
	while((i>0)&&(j<SIZE))
	{
		a=0;
		count=d;
		factor=1;
		while((count>0)&&(i>0))
		{
			b=((int)c[i])-48;
			a=a+b*factor;       
			factor=factor*10;
			count--;
			i--;
		}
		n->number[j]=a;
		j++;
	}
	if((j==SIZE)&&(i!=0))
	{
		status=FAILURE;
	}
	return status;
}
void print(num n,int d)
{
	int i,flag,p,temp;
	p=pow(10,d);
	if(n.s==NEGATIVE)
	{
		printf("-");
	}
	else
	{
		printf("+");
	}
	flag=0;
	i=0;
	while((i<SIZE)&&(flag==0))
	{
		if(n.number[i]==-1)
		{
			flag=1;
		}
		else
		{
			i++;
		}
	}
	i=i-1;
	while(i>=0)
	{
		if(n.number[i]==0)
		{
			printf("0000");
		}
		else
		{
			temp=p/10;
			while(n.number[i]/temp==0)
			{
				printf("0");
				temp=temp/10;
			}
			printf("%d",n.number[i]);
		}
		i--;
	}
	printf("\n");
}
SC add(num n1,num n2,num* result,int d)
{
	SC status=SUCCESS;
	int carry,sum,p,flag,i,borrow,diff,len1,len2;
	num temp;
	initialise(result);
	p=pow(10,d);
	if(n1.s*n2.s==1)    //checking both nos are of same sign
	{
		carry=0;
		result->s=n1.s;
		flag=0;
		i=0;
		while((i<SIZE)&&(flag==0))
		{
			if((n1.number[i]==-1)||(n2.number[i]==-1))
			{
				flag=1;
			}
			else
			{
				sum=carry+n1.number[i]+n2.number[i];
				carry=sum/p;
				sum=sum%p;
				result->number[i]=sum;
				i++;
			}
		}
		if(n1.number[i]==-1) 	//n2>n1
		{
			while((i<SIZE)&&(n2.number[i]!=-1))
			{
				sum=carry+n2.number[i];
				carry=sum/p;
				sum=sum%p;
				result->number[i]=sum;
				i++;
			}
		}
		else if(n2.number[i]==-1)  		//n2<n1
		{
			while((i<SIZE)&&(n1.number[i]!=-1))
			{
				sum=carry+n1.number[i];
				carry=sum/p;
				sum=sum%p;
				result->number[i]=sum;
				i++;
			}
		}
		if(carry!=0)
		{
			if(i==SIZE)				//value exceeded
			{
				status=FAILURE;
			}
			else
			{
				result->number[i]=carry;
			}
		}
	}
	else 			//both are diiff sign
	{
		len1=0;
		len2=0;
		flag=0;
		i=0;
		while((i<SIZE)&&(flag==0))
		{
			if(n1.number[i]==-1)
			{
				flag=1;
			}
			else
			{
				len1++;
				i++;
			}
		}
		i=0;
		flag=0;
		while((i<SIZE)&&(flag==0))
		{
			if(n2.number[i]==-1)
			{
				flag=1;
			}
			else
			{
				len2++;
				i++;
			}
		}
		flag=1;
		if(len1>len2)
		{
			result->s=n1.s;
		}
		else if(len1<len2)
		{
			result->s=n2.s;
			temp=n1;
			n1=n2;
			n2=temp;
		}
		else		//both lenth same but sign diff
		{
			i=len1-1;
			flag=0;
			while((i>=0)&&(flag==0))
			{
				if(n1.number[i]!=n2.number[i])
				{
					flag=1;
				}
				else
				{
					i--;
				}
			}
			if(flag==1)
			{
				if(n1.number[i]>n2.number[i])
				{
					result->s=n1.s;
				}
				else
				{
					result->s=n2.s;
					temp=n1;
					n1=n2;
					n2=temp;
				}
			}
		}
		if(flag==0)
		{
			result->s=POSITIVE;
			result->number[0]=0;		//ans is 0
		}
		else						//sign diff lenth diff
		{
			borrow=0;
			i=0;
			flag=0;
			while((i<SIZE)&&(flag==0))
			{
				if(n2.number[i]==-1)
				{
					flag=1;
				}
				else
				{
					diff=n1.number[i]-borrow-n2.number[i];
					borrow=0;
					if(diff<0)
					{
						diff=diff+p;
						borrow=borrow+1;
					}
					result->number[i]=diff;
					i++;
				}
			}
			if(flag==1)
			{
				while((i<SIZE)&&(n1.number[i]!=-1))
				{
					diff=n1.number[i]-borrow;
					borrow=0;
					if(diff<0)
					{
						diff=diff+p;
						borrow=borrow+1;
					}
					result->number[i]=diff;
					i++;
				}
			}
		}
	}
	return status;
}
SC sub(num n1,num n2,num* result,int d)
{
	SC status;
	n2.s=-1*n2.s;
	status=add(n1,n2,result,d);
	return status;
}

SC mul(num n1,num n2,num* result,int d)
{
	int i,carry,temp,flag,j,p;
	SC status=SUCCESS;
	p=pow(10,d);
	initialise(result);
	result->s=n1.s*n2.s;
	i=0;
	flag=0;
	while((i<SIZE)&&(n2.number[i]!=-1)&&(flag==0)) 	//while n2 not vanishes
	{
		carry=0;
		j=0;
		while((j<SIZE)&&(n1.number[j]!=-1)&&(flag==0))		//while n1 not vanishes		
		{
			if((i+j)>=SIZE)
			{
				flag=1;
				status=FAILURE;
			}
			else
			{
				if(result->number[i+j]==-1)
				{
					temp=n1.number[j]*n2.number[i]+carry;
					carry=temp/p;
					temp=temp%p;
					result->number[i+j]=temp;
				}
				else
				{
					temp=n1.number[j]*n2.number[j]+carry+result->number[i+j];
					carry=temp/p;
					temp=temp%p;
					result->number[i+j]=temp;
				}
			}
			j++;
		}
		i++;
	}
	return status;
}

int main()
{
	int d,flag,k;
	num n1,n2,result;
	SC status;
	d=size();
	flag=0;
    printf("enter number1:");
	status=intscan(&n1,d);
	printf("enter number2:");
	status=intscan(&n2,d);
	status=add(n1,n2,&result,d);
	if(status==SUCCESS)
	{
	  	printf("\n1:Addition ===>");
		print(result,d);
	}
	else
	{
		printf("Error\n");
	}
   	status=sub(n1,n2,&result,d);
	if(status==SUCCESS)
	{
		printf("\n2:Subraction ===>");
		print(result,d);
	}
	else
	{
		printf("Error\n");
	}
	status=mul(n1,n2,&result,d);
	if(status==SUCCESS)
	{
		printf("\n3:Multiplication ===>");
		print(result,d);
	}
	else
	{
		printf("Error\n");
	}
}














































