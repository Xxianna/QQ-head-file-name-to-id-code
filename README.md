# QQ-head-file-name-to-id-code
2024年可以在安卓手机免root找回丢失的QQ好友的QQ号的方法，适用于封号，忘记密码，好友转移等情况，需要大致记得好友头像长什么样
免root找回qq好友qq号

## 项目建设原因
我大号被封了七天，小号没加她大号，怕她不看她的小号，非常想她，但没有保存她的大号QQ号。

## 针对环境
- 该方法在2024年9月的qq中仍然适用
- 手机QQ登陆并使用过封号/忘记密码/好友转移/丢失好友的源账号
- 大致记得丢失的好友的添加日期，或在该设备的初次聊天日期，便于寻找头像文件
- 大致记得好友的头像样子
- 如果是比较新的鸿蒙设备，访问/sdcard/Android/data文件夹需要root或adb权限，即需要有一台安装了adb程序的电脑
- 一根好用的数据线，或者无线调试（参考adb使用方法）

## 注意
- 本项目不涉及刷机或破坏qq数据的操作，请根据步骤谨慎操作，数据损坏责任自负

## 参考链接：
- 1.[android版手机qq好友号码缓存在哪个文件夹里-ZOL问答](https://ask.zol.com.cn/x/6569758.html)
- 2.[根据QQ头像hash分析QQ号(html版)-OSCHINA](https://my.oschina.net/duoing/blog/3081908)
- 3.[C++:符合RFC1321规范的MD5计算C++实现-腾讯云](https://cloud.tencent.com/developer/article/1433426)

## 原理
- ```/sdcard/Android/data/com.tencent.mobileqq/Tencent/MobileQQ/head/_hd/```目录下，保存着大量的头像缩略图，包含在qq私聊群聊中见到的每个人
- QQ头像的文件名就是MD5(MD5(MD5(QQ号)+QQ号)+QQ号)这样一个规则（每一步都要取大写）

## 操作步骤

### 一、获取头像文件

- 手机安装MT管理器
- 试图打开```/sdcard/Android/data```目录
  - 若无法打开，根据MT管理器要求进行操作，例如
    - 安装Shizuku
    - 使用线缆连接手机与电脑
    - 手机打开开发者选项，可以上网查，根据品牌有不同，多为连按安卓版本或手机系统版本
    - 打开USB调试
    - 下载adb套件，可以在各刷机教程获取
    - 在终端打开adb文件夹，运行```./adb shell sh /sdcard/Android/data/moe.shizuku.privileged.api/start.sh```（powershell，cmd可能不需要“./”）
    - 看到输出```info: shizuku_starter exit with 0```则ok。否则根据报错解决。常见问题是adb找不到设备，请更换数据线，建议硬盘盒的线
    - 在Shizuku中授予MT管理器权限
    - 不要拔掉线，直到获取到需要的文件名（#￥%）
- 试图打开```/sdcard/Android/data/com.tencent.mobileqq/Tencent/MobileQQ/head/_hd/```目录。大概长这个样子，可以手动找找
- 目录下的```jpg_```格式文件就是头像。可将其批量选中或选中你想找的人的头像的加载日期（例如加好友日期或在此设备上最近看到他的日期，不明确其中机制，该缓存可能每几个月会清理）的所有文件
- 可以直接按图片格式查看。不建议
- 复制到你自己的文件夹，例如在/sdcard下新建一个随便名字的文件夹。此时即可移除adb线（#￥%）
- 在MT管理器全选这些你的文件夹下的文件，长按点重命名，激活批量重命名
- 替换```.jpg_```为```.jpg```
- 此时即可预览所有图片。找到需要找回的qq好友的头像，保留这些头像，记录其文件名（不包含后缀）
- 本方法在2024年9月，获得的文件名仍然是```32位16进制,大写字母```的格式，这是本方法奏效的前提。参考链接1中```每个QQ好友的头像都是以该好友的QQ号码命名的。```在此时已经不成立

### 二、解算头像文件名

PS yourpath> ./md5test.exe 1234567 
```  
5FEF7B2C5AA49503A1B8D7F8375FF3BF.jpg_
```
PS yourpath> ./md5test.exe 5FEF7B2C5AA49503A1B8D7F8375FF3BF.jpg_ 1239999 1234568
```
Not Found!
```
PS yourpath> ./md5test.exe 5FEF7B2C5AA49503A1B8D7F8375FF3BF.jpg_ 1239999 1234560
```
1234567
```
PS yourpath> ./md5test.exe
```
活动代码页: 936
Usage: 
根据文件名获取qq号：D:\手机图片导出\qq_friends_info\md5test.exe [文件名] [最大猜测QQ号] [最小猜测QQ号]
根据qq号获取文件名：D:\手机图片导出\qq_friends_info\md5test.exe [qq号]
```
文件名只需要包含前32位即可（MD5），后面可删可不删

### 三、其他
- 编译命令示例（windows gnu）：```g++ md5a.cpp md5.cpp -o md5test.exe -O3 -static -fopenmp```
- 引用[根据QQ头像hash分析QQ号(html版)-OSCHINA](https://my.oschina.net/duoing/blog/3081908)中的html版本，便于大家快速测试，但效率较低
```
<html>
<head>
<script src="https://cdn.bootcss.com/blueimp-md5/2.11.0/js/md5.min.js"></script>
</head>
<body>
</body>
<script>

var x=9999999999;//起始的QQ号，计算是向下递减的
function calc(){
	var i=0;
	for(;i<100000;i++){
		var qq=x-i;
if(md5(md5(md5(qq+"").toUpperCase()+qq).toUpperCase()+qq).toUpperCase()=="你的QQ头像hash"){
			alert(qq);
		}
	}
	x=x-i;
	document.write(x+"<br />");
	setTimeout(calc,50);//用setTimeout防止网页卡死
}
calc();
</script>
</html>
```




