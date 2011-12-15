#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

size_t nCount = 0;
unsigned int *index_start = NULL;
unsigned int *index_end = NULL;
unsigned int *index_ptr = NULL;

#define BASE_PTR 5


struct php_sinaip
{
	FILE *fp;
	int nCount;
};

int query(FILE *fp, unsigned int ip)
{
	int i_s = 1;
	int i_e = nCount;
	int run = 0;
	while (i_s <= i_e)
	{
		int i_mid = (i_s+i_e)/2;
		
		if (index_start[i_mid-1] == 0)
		{
			fseek(fp,BASE_PTR+(i_mid-2)*sizeof(int),SEEK_SET);
			fread(&index_start[i_mid-1],sizeof(int),1,fp);
			index_start[i_mid-1] = htonl(index_start[i_mid-1]);
		}
		
		if (ip == index_start[i_mid-1]){
			return i_mid;
		}else if (index_start[i_mid-1] > ip)
		{
			i_e = i_mid-1;
		}else
		{
			i_s = i_mid + 1;
		}
		run++;
	}
	if (index_start[i_e-1] == 0)
	{
		fseek(fp,BASE_PTR+(i_e-2)*sizeof(int),SEEK_SET);
		fread(&index_start[i_e-1],sizeof(int),1,fp);
		index_start[i_e-1] = htonl(index_start[i_e-1]);
	}
	if (index_end[i_e-1] == 0)
	{
		fseek(fp,BASE_PTR+(nCount+i_e-2)*sizeof(int),SEEK_SET);
		fread(&index_end[i_e-1],sizeof(int),1,fp);
		index_end[i_e-1] = htonl(index_end[i_e-1]);
	}
	if (index_start[i_e] && ip>index_start[i_e-1] && ip<=index_end[i_e-1])
	{
		return i_e-1;
	}else
	{
		return 0;
	}
}

char *getstr(struct php_sinaip *ps, unsigned char *len)
{
	if (fread(len,sizeof(char),1,ps->fp)!=1) return NULL;
	if (!*len) return NULL;
	char *data = (char *)malloc(*len+1);
	if (fread(data, sizeof(char), *len, ps->fp)!=*len) return NULL;
	data[*len] = 0;
	return data;
}

void main(){
	struct timeval time;
	float microtime = 0.0f;

	FILE *fp = fopen("data/SinaIP.dat","rb");
	if (!fp) return;
	
	fseek(fp,1,SEEK_SET);
	fread(&nCount,sizeof(int),1,fp);
	nCount = htonl(nCount);
	
	printf("count: %zd\n",nCount);
	
	index_start = (unsigned int *)calloc(nCount+1, sizeof(int));
	index_end = (unsigned int *)calloc(nCount+1, sizeof(int));
	index_ptr = (unsigned int *)calloc(nCount+1, sizeof(int));
	
	fread((index_start+1), sizeof(int), nCount, fp);
	fread((index_end+1), sizeof(int), nCount, fp);
	fread((index_ptr+1), sizeof(int), nCount, fp);
	
	gettimeofday(&time, NULL);

	microtime = time.tv_sec;
	microtime += time.tv_usec/1000.0;

	unsigned int j;
	unsigned int key;
	struct php_sinaip ps;
	int i;
	int found = 0;
	ps.fp = fp;
	ps.nCount = nCount;
	for (i=0;i<1000000;i++)
	{
		j=100000000+(int)(3074228224.0*rand()/RAND_MAX);
		key = query(fp,j);
		if (key)
		{
			if (index_ptr[key] == 0)
			{
				fseek(fp,BASE_PTR+(nCount*2+key-1)*sizeof(int),SEEK_SET);
				if (1 != fread(&index_ptr[key],sizeof(int),1,fp)) continue;
				index_ptr[key] = htonl(index_ptr[key]);
			}
			fseek(fp,index_ptr[key],SEEK_SET);
			int nlen = 0;
			if (1 != fread(&nlen,sizeof(int),1,fp)) continue;
			nlen = htonl(nlen);
			
			unsigned char ilen;
			char *data;
			data=getstr(&ps,&ilen);
			data=getstr(&ps,&ilen);
			data=getstr(&ps,&ilen);
			data=getstr(&ps,&ilen);
			data=getstr(&ps,&ilen);
			data=getstr(&ps,&ilen);
			data=getstr(&ps,&ilen);
			data=getstr(&ps,&ilen);
			data=getstr(&ps,&ilen);
			
			found++;
		}
	}
	
	gettimeofday(&time, NULL);

	printf("run at: %0.6f   found: %d\n", time.tv_sec+time.tv_usec/1000.0 - microtime,found);

	fclose(fp);
}