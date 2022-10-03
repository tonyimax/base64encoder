//
//  main.cpp
//  base64
//
//  Created by Hacker X on 2022/8/27.
//

//
// Created by devel on 2022/8/27.
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { cout<<"删除数组指针失败"<<endl; } pData=NULL; }

int GetCodeLength(int nDataLength){
    // len = 13      +  4   +   1 = 18
    int len = nDataLength + nDataLength /3 + (int)(nDataLength%3!=0);
    if(len%4){
        len += 4 - len%4;//len = 18 + (4-2)  = 20
    }
    return len;
}

union unBuffer{
    unsigned char bytes[4];//4字节  刚好32位
    unsigned int block=0;//int 也是32位
};


//下面是过滤字符变量声明+/=
const wchar_t CHAR_63 = '+';
const wchar_t CHAR_64 = '/';
const wchar_t CHAR_PAD = '=';

//参与生成BASE64字符串的字符A-za-z+/
//也就是说生成的BASE64字符串只包含下面表中的所有字符
const wchar_t alph[] =
        {
                'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',//26
                'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',//26
                '0','1','2','3','4','5','6','7','8','9',CHAR_63,CHAR_64 //12
        };//26+26+12=64

//生成BASE64字符串
/*
*   pData ： 要生成BASE64字符串的明文
*   nDataLen  ： pData指向的字符串长度
*    szOutBuffer ：保存转换后的BASE64字符串容器
*   nBufferLen ： 容器大小
*/
int Encode(const char * pData, int nDataLen, wchar_t szOutBuffer[], int nBufferLen)
{
    int len = GetCodeLength(nDataLen);
    wchar_t* out = new wchar_t[len];

    unBuffer buffer;
    const int mask = 0x3F;
    for (int i = 0, j = 0, left = nDataLen; i < nDataLen; i += 3, j += 4, left -= 3)
    {
        buffer.bytes[2]=pData[i];
        if (left > 1)
        {
            buffer.bytes[ 1 ] = pData[ i + 1 ];
            if( left > 2 ) //3>2
                buffer.bytes[ 0 ] = pData[ i + 2 ];
            else
                buffer.bytes[ 0 ] = 0;
        }
        else
        {
            buffer.bytes[ 1 ] = 0;
            buffer.bytes[ 0 ] = 0;
        }

        out[ j ] = alph[ ( buffer.block >> 18 ) & mask ];//字符第一位
        out[ j + 1 ] = alph[ ( buffer.block >> 12 ) & mask ];//字符第二位

        if( left > 1 )
        {
            out[ j + 2 ] = alph[ ( buffer.block >> 6 ) & mask ];//字符第三位
            if ( left > 2 )
                out[ j + 3 ] = alph[ buffer.block & mask ];//字符第四位
            else
                out[ j + 3 ] = CHAR_PAD;//不足四位，=号填充
        }
        else
        {
            //不足四位，=号填充
            out[ j + 2 ] = CHAR_PAD;
            out[ j + 3 ] = CHAR_PAD;
        }
    }

    memcpy(szOutBuffer, out, len*sizeof(wchar_t));

    //释放资源
    SafeDeleteArray(out);

    return len;
}

int main(){
    wchar_t buf[32]={0};//2 * 32
    Encode("123", strlen("123") ,buf,sizeof(buf));
    wprintf(buf);//宽字符直接输出
    
    size_t bufLen= wcslen(buf)+1;
    char* cbuf = (char*) malloc(bufLen*sizeof (char));
    memset(cbuf,0,bufLen);
    wcstombs(cbuf,buf,bufLen);

    printf("\nchar* : %s\n",cbuf);

    if (0 == strcmp("MTIz",cbuf)){
        printf("base64 encode ok\n");
    }
    delete[] cbuf;

    return 0;
}

