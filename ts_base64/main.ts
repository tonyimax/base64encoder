console.log("BASE64算法基于TS实现");

let alph = [ 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
    '0','1','2','3','4','5','6','7','8','9','+','/'];

console.log(`BASE64字符表:${alph}`);

function  Base64Encode(toEncodeString){
    let te = new TextEncoder();//实例化字符编码对象
    let byteArray = te.encode(toEncodeString);//字符串转编码集合
    let len = toEncodeString.length;//取字符串数量

    let s = "";//临时使用的二进制字符串拼接
    let t = "";//输出base64字符串
    let base64Str = "";//临时转换的4位base64字符

    //取满足3字节，先编码
    for (let i = 0; i < Math.floor(len/3); i++)
    {
        var b1 = byteArray[i * 3];    //第1个字节
        var b2 = byteArray[i * 3+1];  //第2个字节
        var b3 = byteArray[i * 3+2];  //第3个字节

        //拼接3个字符8位数据为24位数据 x.toString(2) 为将x转换二进制 .padStart为向高位补0
        s = b1.toString(2).padStart(8,'0') + b2.toString(2).padStart(8,'0') + b3.toString(2).padStart(8,'0');

        let data = s;//保存24位数据
        s=null;
        //分割24位数据,转换成4个6位，然后转换成整数
        let p1 = data.substr(0, 6);
        let p2 = data.substr(6, 6);
        let p3 = data.substr(12, 6);
        let p4 = data.substr(18, 6);

        //编码一组3个字节为4字节, parseInt(x,2) 为将二进制x转换为整数
       base64Str = alph[parseInt(p1,2)] + alph[parseInt(p2,2)] + alph[parseInt(p3,2)] + alph[parseInt(p4,2)];

       t+=(base64Str);
    }

    //处理不足3字节数据
    if (len % 3 > 0)
    {
        let lastGroupLen = len % 3;
        let tmpBytes = [];
        if (lastGroupLen == 1)
        {
            tmpBytes[0] = byteArray[len-1];
            tmpBytes[1] = 0;
            tmpBytes[2] = 0;
        }
        if (lastGroupLen == 2)
        {
            tmpBytes[0] = byteArray[len - 2];
            tmpBytes[1] = byteArray[len - 1];
            tmpBytes[2] = 0;
        }

        s = tmpBytes[0].toString(2).padStart(8,'0') + tmpBytes[1].toString(2).padStart(8,'0') + tmpBytes[2].toString(2).padStart(8,'0');
        let data = s;
        s=null;

        //转换成4个6位，然后转换成整数  //011001 000000 000000 000000
        let p1 = data.substr(0, 6);
        let p2 = data.substr(6, 6);
        let p3 = data.substr(12, 6);
        let p4 = data.substr(18, 6);

        if (lastGroupLen==1)
        {
            //编码一组3个字节为4字节
            base64Str = alph[parseInt(p1,2)] + alph[parseInt(p2,2)] + "==";  //最后一组只有1位,补两个0
        }
        if (lastGroupLen == 2)
        {
            //编码一组3个字节为4字节
            base64Str = alph[parseInt(p1,2)] + alph[parseInt(p2,2)] + alph[parseInt(p3,2)] + "="  //最后一组只有2位,补1个0
        }
        t+=(base64Str);
    }
    console.log(`源码字符串: ${toEncodeString} -> BASE64字符串: ${t}`);
    return t;

}

Base64Encode("123");
