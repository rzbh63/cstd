# Lite-版本更新记录

\---------------------------------
2018-12-18， V1.6.0 
\---------------------------------

**功能升级及问题修正：**

1) 控制台上增加浅色主题，优化交互体验。
2) 在浅色主题上升级身份证识别能力，新增用户切换拍摄身份证或手输身份证模式。
3) 修复若干已知问题。

\---------------------------------
2017-06-02， V1.5.3 
\---------------------------------

**功能升级：**

1) 提升了双角度活体的屏幕翻拍防御能力

\---------------------------------
2017-04-07， V1.5.3 
\---------------------------------

**功能升级及问题修正：**

1) 优化语音识别性能 
2) 新增双角度活体验证能力
3) 新增云端活体验证API能力
4) 修复若干已知问题

\---------------------------------
2017-01-13， V1.4.1 
\---------------------------------

**功能升级及问题修正：**

1) 启用视频中的唇语检测判断 
2) 优化视频验证阶段四位随机数的生成逻辑 
3) 修正流程上因超过上传次数被动退出之后Status仍为“PROCESSING”问题，修正为“FAILED” 
4) 修正Lite-GetToken 中 notify_url 没有http头导致回调出错问题

\---------------------------------
2017-01-06， V1.4.0
\---------------------------------

**API和能力升级：**

1) Lite-GetToken时支持配置场景ID
2) 返回值中status字段增加TIMEOUT状态
3) 返回值中status字段增加CANCELLED状态
4) 返回值中增加身份证签发机关字段（idcard_issued_by）
5) 新增身份证人像图和数据源图片比对结果（新增result_idcard_datasource字段）
6) 新增视频中无音频错误类型 （新增NO_AUDIO字段）
7) 取消无源比对的身份证拍摄

**业务流程优化：**

1) UX整体调整更新 
2) 新增身份证签发机关修改控制
3) 流程中用户可以选择取消，同时会通知商户 
4) 流程超时时长限制 
5) 视频上传次数限制 
6) 流程中出错文案的优化

**FaceID 控制台增加移动端身份验证的场景配置：**

1) 身份证识别结果可修改字段的控制
2) 身份证图像上传次数限制修改
3) 视频上传次数限制修改
4) 流程超时时长限制修改
5) 微信跳转页面的标题和底图修改
6) 流程首页的标题和底图修改
7) 流程次页及后续页的标题修改

 