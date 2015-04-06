基于C的PHP/Node.js快速IP解析扩展

性能:

Xeon E5620@2.4G 云主机下
```
SinaIP		100000/秒
QQWry PHP	3179/秒
GeoIP Record	40000/秒
```

Xeon E3-1230
```
SinaIP for PHP		780000/秒
SinaIP for node.js	2250000/秒
```

PHP安装:
```
phpize
./configure
make && make install
```

Node.js安装
```
node-waf configure && node-waf build && node-waf install
```

PHP例子:
```
$sinaip = sinaip_init("数据库文件"); /* 开启数据库文件 */
$count = sinaip_count($sinaip);  /* 查询数据库数据量 */
for ($i=0;$i<10000;$i++)
{
  sinaip_search($sinaip,mt_rand(100000000,3074228224));
}
$ip = sinaip_search($sinaip,ip2long("IP地址")); /* 查询IP 找到 返回数组  沒找到 返回false */
sinaip_close($sina_ip); /* 关闭数据库 释放资源 */
```

Node.js例子:
```
var hw = require("sinaip");
var s = new hw.SinaIP("SinaIP.dat");
s.preload();
var t1 = new Date();
var num = 1000000;
for (var i=0;i<num;i++) s.query(Math.floor(Math.random()*100000000));
console.log(Math.round(num/(((new Date().getTime())-t1.getTime())/1000))+" QPS");
```

QQ: 754774344
E-mail: magicbearmo@gmail.com