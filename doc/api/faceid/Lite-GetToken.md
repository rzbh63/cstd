# Lite-GetToken

## URL

POST <https://api.megvii.com/faceid/lite/get_token>

**注意：**在生产环境中，请使用**HTTPS**的通信方式。HTTP方式的通信属于不安全链路，存在安全风险，请勿在生产环境中使用。在生产环境中使用HTTP方式的，将无法得到服务可靠性保障

## 说明

此接口提供 FaceID Lite 验证服务，通过此接口客户可以获得一个用于实名验证的 token（token唯一且只能使用一次）。接口同时还能帮助完成人脸比对，并在完成验证后自动将人脸比对结果返回，方便集成开发。

## 参数

| 必选／可选                                 | 参数                     | 类型        | 参数说明                                                     |                                                              |
| :----------------------------------------- | :----------------------- | :---------- | :----------------------------------------------------------- | ------------------------------------------------------------ |
| **必选**                                   | api_key                  | String      | 调用此API的api_key；                                         |                                                              |
| **必选**                                   | api_secret               | String      | 调用此API的api_key的secret；                                 |                                                              |
| **必选**                                   | return_url               | String      | 用户完成或取消验证后网页跳转的目标URL。（**回调方法为Post**） |                                                              |
| **必选**                                   | notify_url               | String      | 用户完成验证、取消验证、或验证超时后，由FaceID服务器请求客户服务器的URL。（推荐为HTTPS页面，如果为HTTP则用户需要通过签名自行校验数据可信性，**回调方法为Post**）**注：**出于安全性考虑，FaceID服务对服务器端回调端口有白名单要求，支持的端口有：443，5000，16003，8883，8028 |                                                              |
| **必选**                                   | biz_no                   | String      | 客户业务流水号，该号必须唯一。并会在notify和return时原封不动的返回给您的服务器，以帮助您确认每一笔业务的归属。此字段不超过128字节。 |                                                              |
| 可选                                       | biz_extra_data           | String      | 在调用notify_url和return_url时会返回的额外数据，用户可以用此接口来传递一些额外信息，以帮助您调试和信息传递。此字段不超过4096字节。 |                                                              |
| 可选                                       | web_title                | String      | 验证网页展示用的标题文字。此字段不超过32字节，但考虑到显示效果，建议控制在10个中文字符以内。此参数默认为空，此时系统将采用默认的标题。请注意，如果您设置了scene_id参数、或者在控制台里设置了默认的场景，它对应的场景配置将覆盖您此处的设定。 |                                                              |
| 可选                                       | scene_id                 | String      | 在控制台配置的对应使用场景的scene_id，用以自定义验证流程中的视觉元素。如果不传此参数，则选择在控制台中设置的默认scene；如果控制台中没有设定任何scene，则采用系统默认方案；如果传入了不存在的scene_id，调用将失败且返回错误码400（BAD_ARGUMENTS）。 |                                                              |
| 可选                                       | procedure_type           | String      | 刷脸活体验证流程的选择。目前仅取以下值："video"：通过自拍有声视频方式进行活体验证"selfie"：通过两张自拍方式进行活体验证本参数默认值为“video”。 |                                                              |
| 可选                                       | liveness_preferences     | String      | 本参数可选，通过一系列配置项来调节活体检测的严格度。目前可用的选项有：“none”：表示没有特别的选项，此为默认值。"selfie_no_metadata_check"：（仅precedure_type == “selfie”时有效）表示不限定照片格式为JPG、也不校验照片的元信息，此设置会降低安全性。"video_strict"：（仅precedure_type == “video”时有效）表示针对上传的视频进行相对严格的活体检测，此设置会提高安全性，但在一定程度上影响通过率。注意：设置其他值，均会返回400（BAD_ARGUMENTS）。 |                                                              |
| **必选**                                   | comparison_type          | String      | 设定本次核身服务的类型，目前支持的比对类型为“有源比对”（取值“1”）或“无源比对”（取值“0”）。传递其他值调用将识别，返回错误码400（BAD_ARGUMENTS）。“1”表示有源比对，表示最终的用户自拍照片将于第三方数据源照片比对。 “0”表示无源比对，FaceID将使用用户自己提供的照片（参数image_ref[x]）作为比对人脸照。**请注意：****本参数影响验证流程中是否存在身份证拍摄环节：如果为“1”，则可选择包含身份证拍摄；如果为“0”，验证流程中将没有身份证拍摄。****本参数取什么值将决定下面“二选一”参数组使用哪一组参数。** |                                                              |
| 二选一：有源比对时（comparison_type ＝ 1） | **必选**                 | idcard_mode | String                                                       | 传递参数“0”，“1”，“2”，“3”或“4”，表示获取用户身份证信息的方法。传递其他值调用将识别，返回错误码400（BAD_ARGUMENTS）。0：不拍摄身份证，而是通过 idcard_name / idcard_number 参数传入；1：仅拍摄身份证人像面，可获取人像面所有信息；2：拍摄身份证人像面和身份证国徽面，可获取身份证所有信息；3：不拍摄身份证，但会要求用户在界面上输入身份证号和姓名；4：拍摄身份证人像面或用户输入身份证号和姓名，用户可在界面上自行选择身份证录入方法。**注意：**该参数只有在控制台中选择使用“浅色主题”时才生效，若未应用浅色主题而传入4，则返回错误码400（BAD_ARGUMENTS）。 |
| 可选                                       | idcard_uneditable_field  | String      | 选择用户不可编辑的字段。当选择拍摄身份证来获取上面的文字信息时，会由于OCR算法的问题导致文字识别不准确（如：生僻字、形近字），通过该参数可以限制用户对识别得到的结果进行编辑。（如：身份证号的识别都是准确的，可以限制用户不能编辑此字段）。目前可以选为不可修改的字段有 姓名（idcard_name），身份证号（idcard_number），身份证有效期（idcard_valid_date），签发机关（idcard_issued_by）。传递参数时用逗号隔开，可以同时设置多个参数。例子：idcard_uneditable_field=idcard_number,idcard_valid_date 表示身份证号和过期时间都不可编辑，如果尝试编辑则会提示需要重新拍摄。传入其他值或格式不对，本次调用将失败，返回错误码400（BAD_ARGUMENTS）。本参数默认值为空，表示所有字段都可以编辑。请注意，如果您设置了scene_id参数、或者在控制台里设置了默认的场景，它对应的场景配置里将覆盖您此处的设定。 |                                                              |
| **条件必选**                               | idcard_name              | String      | idcard_name， 需要核实身份对象的姓名，使用UTF-8编码；idcard_number， 需要核实身份对象的身份证号，也就是一个18位长度的字符串。**idcard_mode = 0 时，这两个参数必须传；在其他情况下可以不传，即使传递了验证流程也不会使用。** |                                                              |
| **条件必选**                               | idcard_number            | String      |                                                              |                                                              |
| 可选                                       | image_ref[x]             | File        | 多张由您自己提供的参照人脸照片。x表示此参数可重复多次，其中**1 <= x <= 2**，即表示可以传最多二张参照人脸照片（参数分别为image_ref1, image_ref2）。如果在image_ref1、image_ref2中的任一张图片里没有找到人脸，将返回错误码400（NO_FACE_FOUND)；如果这些图片中任一张中有多张脸，将返回错误码400（MULTIPLE_FACES）。 |                                                              |
| 二选一：无源比对时（comparison_type ＝ 0） | **必选**                 | uuid        | String                                                       | 如果用户不使用数据源的数据进行比对，则上传此字段，用于标志本次识别对应的用户的唯一ID，要求不长于512字节。建议您对来自同一用户的比对请求使用同样的ID，这非常有利于您反查验证结果以及获得更好的监控报表体验。 |
| **必选**                                   | image_ref[x]             | File        | 多张由您自己提供的参照人脸照片。x表示此参数可重复多次，其中**1 <= x <= 2**，即表示可以传最多二张参照人脸照片（参数分别为image_ref1, image_ref2）。如果在image_ref1、image_ref2中的任一张图片里没有找到人脸，将返回错误码400（NO_FACE_FOUND)；如果这些图片中任一张中有多张脸，将返回错误码400（MULTIPLE_FACES）。 |                                                              |
| 可选                                       | multi_oriented_detection | String      | 决定对于image_ref[x]参数对应的图片，当检测不出人脸时，是否旋转90度、180度、270度后再检测人脸。本参数取值只能是 “1” 或 "0" （**缺省值为“0”**）:“1”：要旋转检测；“0”：不旋转；其他值：返回错误码400（BAD_ARGUMENTS） **请注意：设置此参数为1可能会轻微增加误检人脸的概率，如果您明确您的业务场景里不存在非正向的人脸图片、或概率极低，建议勿设置此参数。** |                                                              |

注：红色部分为新增字段 

## 返回结果

### 返回值说明

| 字段          | 类型   | 说明                                                         |
| :------------ | :----- | :----------------------------------------------------------- |
| request_id    | String | 用于区分每一次请求的唯一的字符串。此字符串可以用于后续数据反查。此字段必定返回。 |
| time_used     | Int    | 整个请求所花费的时间，单位为毫秒。此字段必定返回。           |
| token         | String | 一个字符串，可用于DoVerification接口，调用DoVerification时传入此参数，即可按照上述配置进行活体检测。（注：每个token只能被使用一次） |
| biz_id        | String | 业务流串号，可以用于反查比对结果.                            |
| expired_time  | Int    | 一个时间戳，表示token的有效期。                              |
| error_message | String | 当请求失败时才会返回此字符串，具体返回内容见后续错误信息章节。否则此字段不存在。 |

### 成功

```
{
    "time_used": 5,
    "token": "34fb21937e47ae719f11cbc719615687",
    "biz_id": "1462259748,52b13fb5-8dfb-4537-a62b-a641d5e929f1",
    "request_id": "1462257147,3149525e-2c24-4862-8e9f-92040595f0a4",
    "expired_time": 1462257765
}
```

### 失败

```
{
    "error_message": "BAD_ARGUMENTS: return_url",
    "request_id": "1461740007,71eeb124-08f0-47b3-8fc8-ac048cfa1372",
    "time_used": 4
}
```

### 错误码列表

#### GetToken 特有的 ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                                | 说明                                                         |
| :------------ | :-------------------------------------- | :----------------------------------------------------------- |
| 400           | IMAGE_ERROR_UNSUPPORTED_FORMAT: <param> | 参数<param>对应的图像无法正确解析，有可能不是一个图像文件、或有数据破损。<param>为 image_ref1、 image_ref2中的一个。请注意：<param>只会有一项，即第一个满足条件的名称。 |
| 400           | NO_FACE_FOUND                           | 表示上传的 image_ref[x] 的图像中，没有检测到人脸。           |
| 400           | MULTIPLE_FACES                          | 表示上传的 image_ref[x] 的图像中，检测到了多张人脸。         |
| 400           | INVALID_NAME_FORMAT                     | 有源比对时，idcard_name参数字符数过多（多于32字符）、或者使用错误的编码（姓名要求以UTF-8编码）。 |
| 400           | INVALID_IDCARD_NUMBER                   | 有源比对时，idcard_number参数不是正确的身份证号码格式。身份证号码必定为18位数字，且最后一位可以是X（大小写均可）。 |

#### 通用的ERROR_MESSAGE

| HTTP 状态代码 | 错误信息                      | 说明                                                         |
| :------------ | :---------------------------- | :----------------------------------------------------------- |
| 403           | AUTHENTICATION_ERROR          | api_key 和 api_secret 不匹配。                               |
| 403           | AUTHORIZATION_ERROR: <reason> | api_key被停用、调用次数超限、没有调用此API的权限，或者没有以当前方式调用此API的权限。目前的<reason>有："Denied." :（没有权限调用当前API）"No data source permission." : （表示没有调用数据源的权限，但尝试调取数据源） |
| 403           | CONCURRENCY_LIMIT_EXCEEDED    | 并发数超过限制。                                             |
| 400           | MISSING_ARGUMENTS: <key>      | 缺少某个必选参数。                                           |
| 400           | BAD_ARGUMENTS: <key>          | 某个参数解析出错（比如必须是数字，但是输入的是非数字字符串; 或者长度过长） |
| 404           | API_NOT_FOUND                 | 所调用的API不存在。                                          |
| 500           | INTERNAL_ERROR                | 服务器内部错误，当此类错误发生时请再次请求，如果持续出现此类错误，请及时联系 FaceID 客服或商务。 |

 

 

 