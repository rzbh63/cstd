@echo off
if not exist "�½��ı��ĵ�.txt" (
pclip >"�½��ı��ĵ�.txt"
cat "�½��ı��ĵ�.txt"|grep "meta data-vue-met"|sed "s#{\"cid\"#\n{\"cid\"#g"|grep "cid"|grep "vupload"|sed "s/[""]//g">list.txt
mv /y "�½��ı��ĵ�.txt" "�½��ı��ĵ�_htm.txt"
(for /f "tokens=4,8 delims=:," %%a in (list.txt) do (
  echo %%a��%%b
))>�½��ı��ĵ�.txt
)

python "D:\code\git\cstd\py\rename_bilibili.py" "%cd%"
pause