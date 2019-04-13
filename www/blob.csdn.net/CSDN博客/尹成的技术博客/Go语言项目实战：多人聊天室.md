
# Go语言项目实战：多人聊天室 - 尹成的技术博客 - CSDN博客

2018年11月16日 09:32:24[尹成](https://me.csdn.net/yincheng01)阅读数：377


**功能需求**
实现单撩
实现群撩
实现用户上线的全网通知
实现用户昵称
实现聊天日志的存储和查看
**服务端实现**
`type Client struct {
	conn net.Conn
	name string
	addr string
}
var (
	//客户端信息,用昵称为键
	//clientsMap = make(map[string]net.Conn)
	clientsMap = make(map[string]Client)
)
func SHandleError(err error, why string) {
	if err != nil {
		fmt.Println(why, err)
		os.Exit(1)
	}
}
func main() {
	//建立服务端监听
	listener, e := net.Listen("tcp", "127.0.0.1:8888")
	SHandleError(e, "net.Listen")
	defer func() {
		for _, client := range clientsMap {
			client.conn.Write([]byte("all:服务器进入维护状态，大家都洗洗睡吧！"))
		}
		listener.Close()
	}()
	for {
		//循环接入所有女朋友
		conn, e := listener.Accept()
		SHandleError(e, "listener.Accept")
		clientAddr := conn.RemoteAddr()
		//TODO:接收并保存昵称
		buffer := make([]byte, 1024)
		var clientName string
		for {
			n, err := conn.Read(buffer)
			SHandleError(err, "conn.Read(buffer)")
			if n > 0 {
				clientName = string(buffer[:n])
				break
			}
		}
		fmt.Println(clientName + "上线了")
		//TODO:将每一个女朋友丢入map
		client := Client{conn, clientName, clientAddr.String()}
		clientsMap[clientName] = client
		//TODO:给已经在线的用户发送上线通知——使用昵称
		for _, client := range clientsMap {
			client.conn.Write([]byte(clientName + "上线了"))
		}
		//在单独的协程中与每一个具体的女朋友聊天
		go ioWithClient(client)
	}
	//设置优雅退出逻辑
}
//与一个Client做IO
func ioWithClient(client Client) {
	//clientAddr := conn.RemoteAddr().String()
	buffer := make([]byte, 1024)
	for {
		n, err := client.conn.Read(buffer)
		if err != io.EOF {
			SHandleError(err, "conn.Read")
		}
		if n > 0 {
			msg := string(buffer[:n])
			fmt.Printf("%s:%s\n", client.name, msg)
			//将客户端说的每一句话记录在【以他的名字命名的文件里】
			writeMsgToLog(msg, client)
			strs := strings.Split(msg, "#")
			if len(strs) > 1 {
				//all#hello
				//zqd#hello
				//要发送的目标昵称
				targetName := strs[0]
				targetMsg := strs[1]
				//TODO:使用昵称定位目标客户端的Conn
				if targetName == "all" {
					//群发消息
					for _, c := range clientsMap {
						c.conn.Write([]byte(client.name + ":" + targetMsg))
					}
				} else {
					//点对点消息
					for key, c := range clientsMap {
						if key == targetName {
							c.conn.Write([]byte(client.name + ":" + targetMsg))
							//在点对点消息的目标端也记录日志
							go writeMsgToLog(client.name + ":" + targetMsg,c)
							break
						}
					}
				}
			} else {
				//客户端主动下线
				if msg == "exit" {
					//将当前客户端从在线用户中除名
					//向其他用户发送下线通知
					for name, c := range clientsMap {
						if c == client {
							delete(clientsMap, name)
						} else {
							c.conn.Write([]byte(name + "下线了"))
						}
					}
				}else if strings.Index(msg,"log@")==0 {
					//log@all
					//log@张全蛋
					filterName := strings.Split(msg, "@")[1]
					//向客户端发送它的聊天日志
					go sendLog2Client(client,filterName)
				} else {
					client.conn.Write([]byte("已阅：" + msg))
				}
			}
		}
	}
}
//向客户端发送它的聊天日志
func sendLog2Client(client Client,filterName string) {
	//读取聊天日志
	logBytes, e := ioutil.ReadFile("D:/BJBlockChain1801/demos/W4/day1/01ChatRoomII/logs/" + client.name + ".log")
	SHandleError(e,"ioutil.ReadFile")
	if filterName != "all"{
		//查找与某个人的聊天记录
		//从内容中筛选出带有【filterName#或filterName:】的行，拼接起来
		logStr := string(logBytes)
		targetStr := ""
		lineSlice := strings.Split(logStr, "\n")
		for _,lineStr := range lineSlice{
			if len(lineStr)>20{
				contentStr := lineStr[20:]
				if strings.Index(contentStr,filterName+"#")==0 || strings.Index(contentStr,filterName+":")==0{
					targetStr += lineStr+"\n"
				}
			}
		}
		client.conn.Write([]byte(targetStr))
	}else{
		//查询所有的聊天记录
		//向客户端发送
		client.conn.Write(logBytes)
	}
}
//将客户端说的一句话记录在【以他的名字命名的文件里】
func writeMsgToLog(msg string, client Client) {
	//打开文件
	file, e := os.OpenFile(
		"D:/BJBlockChain1801/demos/W4/day1/01ChatRoomII/logs/"+client.name+".log",
		os.O_CREATE|os.O_WRONLY|os.O_APPEND,
		0644)
	SHandleError(e, "os.OpenFile")
	defer file.Close()
	//追加这句话
	logMsg := fmt.Sprintln(time.Now().Format("2006-01-02 15:04:05"), msg)
	file.Write([]byte(logMsg))
}`**客户端实现**
`import (
	"net"
	"fmt"
	"os"
	"bufio"
	"io"
	"flag"
)
var (
	chanQuit = make(chan bool, 0)
	conn     net.Conn
)
func CHandleError(err error, why string) {
	if err != nil {
		fmt.Println(why, err)
		os.Exit(1)
	}
}
func main() {
	//TODO:在命令行参数中携带昵称
	nameInfo := [3]interface{}{"name", "无名氏", "昵称"}
	retValuesMap := GetCmdlineArgs(nameInfo)
	name := retValuesMap["name"].(string)
	//拨号连接，获得connection
	var e error
	conn, e = net.Dial("tcp", "127.0.0.1:8888")
	CHandleError(e, "net.Dial")
	defer func() {
		conn.Close()
	}()
	//在一条独立的协程中输入，并发送消息
	go handleSend(conn,name)
	//在一条独立的协程中接收服务端消息
	go handleReceive(conn)
	//设置优雅退出逻辑
	<-chanQuit
}
func handleReceive(conn net.Conn) {
	buffer := make([]byte, 1024)
	for {
		n, err := conn.Read(buffer)
		if err != io.EOF {
			CHandleError(err, "conn.Read")
		}
		if n > 0 {
			msg := string(buffer[:n])
			fmt.Println(msg)
		}
	}
}
func handleSend(conn net.Conn,name string) {
	//TODO:发送昵称到服务端
	_, err := conn.Write([]byte(name))
	CHandleError(err,"conn.Write([]byte(name))")
	reader := bufio.NewReader(os.Stdin)
	for {
		//读取标准输入
		lineBytes, _, _ := reader.ReadLine()
		//发送到服务端
		_, err := conn.Write(lineBytes)
		CHandleError(err, "conn.Write")
		//正常退出
		if string(lineBytes) == "exit" {
			os.Exit(0)
		}
	}
}
func GetCmdlineArgs(argInfos ...[3]interface{}) (retValuesMap map[string]interface{}) {
	fmt.Printf("type=%T,value=%v\n", argInfos, argInfos)
	//初始化返回结果
	retValuesMap = map[string]interface{}{}
	//预定义【用户可能输入的各种类型的指针】
	var strValuePtr *string
	var intValuePtr *int
	//预定义【用户可能输入的各种类型的指针】的容器
	//用户可能输入好几个string型的参数值，存放在好几个string型的指针中，将这些同种类型的指针放在同种类型的map中
	//例如：flag.Parse()了以后，可以根据【strValuePtrsMap["cmd"]】拿到【存放"cmd"值的指针】
	var strValuePtrsMap = map[string]*string{}
	var intValuePtrsMap = map[string]*int{}
	/*	var floatValuePtr *float32
		var floatValuePtrsMap []*float32
		var boolValuePtr *bool
		var boolValuePtrsMap []*bool*/
	//遍历用户需要接受的所有命令定义
	for _, argArray := range argInfos {
		/*
		先把每个命令的名称和用法拿出来,
		这俩货都是string类型的，所有都可以通过argArray[i].(string)轻松愉快地获得其字符串
		一个叫“cmd”，一个叫“你想干嘛”
		"cmd"一会会用作map的key
		*/
		//[3]interface{}
		//["cmd" "未知类型" "你想干嘛"]
		//["gid"     0     "要查询的商品ID"]
		//上面的破玩意类型[string 可能是任意类型 string]
		nameValue := argArray[0].(string)  //拿到第一个元素的string值,是命令的name
		usageValue := argArray[2].(string) //拿到最后一个元素的string值，是命令的usage
		//判断argArray[1]的具体类型
		switch argArray[1].(type) {
		case string:
			//得到【存放cmd的指针】，cmd的值将在flag.Parse()以后才会有
			//cmdValuePtr = flag.String("cmd", argArray[1].(string), "你想干嘛")
			strValuePtr = flag.String(nameValue, argArray[1].(string), usageValue)
			//将这个破指针以"cmd"为键，存在【专门放置string型指针的map，即strValuePtrsMap】中
			strValuePtrsMap[nameValue] = strValuePtr
		case int:
			//得到【存放gid的指针】，gid的值将在flag.Parse()以后才会有
			//gidValuePtr = flag.String("gid", argArray[1].(int), "商品ID")
			intValuePtr = flag.Int(nameValue, argArray[1].(int), usageValue)
			//将这个破指针以"gid"为键，存在【专门放置int型指针的map，即intValuePtrsMap】中
			intValuePtrsMap[nameValue] = intValuePtr
		}
	}
	/*
	程序运行到这里，所有不同类型的【存值指针】都放在对相应类型的map中了
	flag.Parse()了以后，可以从map中以参数名字获取出【存值指针】，进而获得【用户输入的值】
	*/
	//用户输入完了，解析，【用户输入的值】全都放在对应的【存值指针】中
	flag.Parse()
	/*
	遍历各种可能类型的【存值指针的map】
	*/
	if len(strValuePtrsMap) > 0 {
		//从【cmd存值指针的map】中拿取cmd的值，还以cmd为键存入结果map中
		for k, vPtr := range strValuePtrsMap {
			retValuesMap[k] = *vPtr
		}
	}
	if len(intValuePtrsMap) > 0 {
		//从【gid存值指针的map】中拿取gid的值，还以gid为键存入结果map中
		for k, vPtr := range intValuePtrsMap {
			retValuesMap[k] = *vPtr
		}
	}
	//返回结果map
	return
}`学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

