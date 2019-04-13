
# Go语言项目实战：基于开源数据的成语查询 - 尹成的技术博客 - CSDN博客

2018年11月16日 09:37:03[尹成](https://me.csdn.net/yincheng01)阅读数：79


**业务需求**
命令行键入一行诗句启动应用：idiom.exe -cmd start -poem 大王派我来巡山
将诗句中的每个字丢入【模糊查询管道】
另外再建立【精确查询管道】和【结束管道】，分别存储【成语】（大鹏展翅、占山为王、龟派气功…）和【结束指令】（fuckoff）
时钟每秒随机读入一条管道数据：
如果是【模糊查询管道】：起协程进行模糊查询，并汇总数据在内存
如果是【精确查询管道】：起协程进行精确查询，并汇总数据在内存
如果是【结束指令】：停止查询，将内存中的数据持久化为json并退出；
命令行参数获取工具
import (
	"fmt"
	"flag"
)
/*
argInfos	要获取的命令行参数们		例如：[3]interface{}{"cmd","默认命令","要执行的命令"}
retValuesMap	以map的形式返回用户在命令行输入的值 	[cmd:getmoney amount:666 to:张三]
*/
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
}定义成语数据模型
import "fmt"
//成语
type Idiom struct {
	Title      string
	Spell      string
	Content    string
	Sample     string
	Derivation string
}
//打印成语数据
func PrintIdiom(idiom Idiom) {
	if idiom.Title != "" {
		fmt.Printf("Title:%s\n", idiom.Title)
		fmt.Printf("Spell:%s\n", idiom.Spell)
		fmt.Printf("Sample:%s\n", idiom.Sample)
		fmt.Printf("Derivation:%s\n", idiom.Derivation)
		fmt.Printf("Content:%s\n", idiom.Content)
	} else {
		fmt.Println("未找到成语！")
	}
}定义成语数据模型和JSON的互化工具
import (
	"encoding/json"
	"os"
	"fmt"
)
//将模糊查询的json转化为go数据
func ParseJson2Idioms(jsonStr string) (idiomsMap map[string]Idiom) {
	idiomsMap = make(map[string]Idiom)
	//将json转换为go数据
	tempMap := make(map[string]interface{})
	json.Unmarshal([]byte(jsonStr), &tempMap)
	//fmt.Println(tempMap)
	dataSlice := tempMap["showapi_res_body"].(map[string]interface{})["data"].([]interface{})
	//fmt.Printf("type=%T,value=%v",dataSlice,dataSlice)
	for _, v := range dataSlice {
		title := v.(map[string]interface{})["title"].(string)
		idiom := Idiom{Title: title}
		idiomsMap[title] = idiom
	}
	return
}
//将精确查询的json转化为go数据
func ParseJson2Idiom(jsonStr string) Idiom {
	idiom := Idiom{}
	tempMap := make(map[string]interface{})
	json.Unmarshal([]byte(jsonStr), &tempMap)
	dataMap := tempMap["showapi_res_body"].(map[string]interface{})["data"].(map[string]interface{})
	for k, v := range dataMap {
		valueStr := v.(string)
		switch k {
		case "title":
			idiom.Title = valueStr
		case "spell":
			idiom.Spell = valueStr
		case "samples":
			idiom.Sample = valueStr
		case "derivation":
			idiom.Derivation = valueStr
		case "content":
			idiom.Content = valueStr
		}
	}
	return idiom
}
//将go数据写出到json文件
func WriteIdioms2File(idiomsMap map[string]Idiom, path string) {
	fmt.Println("WriteIdioms2File")
	dstFile, _ := os.OpenFile(path, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0666)
	defer dstFile.Close()
	encoder := json.NewEncoder(dstFile)
	err := encoder.Encode(idiomsMap)
	if err != nil {
		fmt.Println("写出json文件失败，err=", err)
		return
	}
	fmt.Println("写出json文件成功！")
}
//读入json文件为go数据
func ReadIdiomsFromFile(dstPath string) (idiomsMap map[string]Idiom, err error) {
	idiomsMap = make(map[string]Idiom)
	//读入json文件数据
	dstFile, _ := os.OpenFile(dstPath, os.O_RDONLY|os.O_CREATE, 0666)
	defer dstFile.Close()
	decoder := json.NewDecoder(dstFile)
	err = decoder.Decode(&idiomsMap)
	if err != nil {
		fmt.Println("加载数据失败！err=", err)
	} else {
		fmt.Println("成功加载数据！")
		fmt.Println("idiomsMap=", idiomsMap)
	}
	return
}定义网络工具
import (
	"net/http"
	"fmt"
	"io/ioutil"
)
//获取模糊查询的url
func GetAmbiguousUrl(keyword string,page string)(url string){
	return "http://route.showapi.com/1196-1?showapi_appid=19988&showapi_sign=968ad4fcc2144e41b5c366838d1b0ec4&keyword="+keyword+"&page="+page+"&rows=20"
}
//获取精确查询的url
func GetAccurateUrl(keyword string)(url string){
	return "http://route.showapi.com/1196-2?showapi_appid=19988&showapi_sign=968ad4fcc2144e41b5c366838d1b0ec4&keyword="+keyword
}
//从url拿到json数据
func GetJson(url string) (jsonStr string, err error) {
	//获得网络数据
	resp, err := http.Get(url)
	if err != nil {
		fmt.Println("http请求失败,err=", err)
		return
	}
	//延时关闭网络IO资源
	defer resp.Body.Close()
	//resp.Body实现了Reader接口，对其进行数据读入
	bytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Println("读取网络数据失败,err=", err)
		return
	}
	//将网络数据转化为字符串输出
	jsonStr = string(bytes)
	//fmt.Println(jsonStr)
	return
}
//模糊查询
func DoAmbiguousQuery(keyword string,page string,chanAccurate chan<- string) {
	//先拿到json
	url := GetAmbiguousUrl(keyword, page)
	jsonStr, _ := GetJson(url)
	//将json转化为成语集合
	idiomsMap := ParseJson2Idioms(jsonStr)
	//将成语集合写入内存数据
	for title,idiom := range idiomsMap{
		dbDataMap[title] = idiom
	}
	//将成语的名字写入精确管道
	for title,_ := range idiomsMap{
		chanAccurate<- title
	}
/*	chanAccurate<- "大鹏展翅"
	chanAccurate<- "隔壁老王"
	chanAccurate<- "龟派气功"
	chanAccurate<- "我很牛逼"
	chanAccurate<- "来咬我呀"
	fmt.Println("DoAmbiguousQuery",keyword,page)
*/
}
//精确查询
func DoAccurateQuery(keyword string) {
	//fmt.Println("DoAccurateQuery",keyword)
	//拿到json
	url := GetAccurateUrl(keyword)
	jsonStr, _ := GetJson(url)
	//将json转化为一个Idiom对象
	idiom := ParseJson2Idiom(jsonStr)
	//将Idiom对象存入总集合，覆盖原来的粗糙对象
	dbDataMap[idiom.Title] = idiom
}主调模块
import (
	"fmt"
	"time"
	"os"
)
const DB_PATH = "d:/temp/idioms-v2.0.json"
var (
	//数据管道
	chanAmbiguous = make(chan string, 20)
	chanAccurate  = make(chan string, 20)
	chanQuit      = make(chan string, 0)
	//全局内存数据
	dbDataMap = make(map[string]Idiom)
)
func main0() {
	//读入命令行参数
	//idiom.exe -cmd start -poem 大王派我来巡山
	cmdInfo := [3]interface{}{"cmd","未知命令","你打算干什么"}
	poemInfo := [3]interface{}{"poem","绞尽果汁想不出","用于启动的一行诗句"}
	retValuesMap := GetCmdlineArgs(cmdInfo,poemInfo)
	cmd := retValuesMap["cmd"].(string)
	poem := retValuesMap["poem"].(string)
	fmt.Println(cmd, poem)
	//将读入的诗句打碎丢入模糊管道
	for _, v := range poem {
		keyword := fmt.Sprintf("%c", v)
		chanAmbiguous <- keyword
	}
	//三选一读入管道数据，周期性执行
	go func() {
		ticker := time.NewTicker(time.Second)
		for {
			<-ticker.C
			select {
			case keyword := <-chanAmbiguous:
				go DoAmbiguousQuery(keyword,"1",chanAccurate)
			case keyword := <-chanAccurate:
				go DoAccurateQuery(keyword)
			case <-chanQuit:
				WriteIdioms2File(dbDataMap, DB_PATH)
				os.Exit(0)
			}
		}
	}()
	//定时20秒结束主程序
	timer := time.NewTimer(20 * time.Second)
	<-timer.C
	chanQuit <- "OVER"
}学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

