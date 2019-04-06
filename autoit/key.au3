
;#include <MsgBoxConstants.au3>
;#include <WindowsConstants.au3>
#include <WinAPI.au3>

Local $hWnd = _WinGetHandleByPnmAndCls("firefox.exe", "MozillaWindowClass")

If Not $hWnd Then
	MsgBox("asdf", "fff", "����û�ҵ�")
	Exit
EndIf

WinActivate($hWnd)
AutoItSetOption("SendKeyDelay", 40)

$file = FileOpen("list.txt", 0)

For $count = 1 To 2
	$line = FileReadLine($file)
Next

For $count = 1 To 1
	; Print the count
	$line = FileReadLine($file)
	;ConsoleWrite($line & Chr(13) )
	If @error = -1 Then ExitLoop
	$strs = StringSplit($line, " ")
	Send("^t")
	ClipPut($strs[1])
	Send("^v")
	Send("{ENTER}")
	Send("{ENTER}")
	Sleep(5000)
	Send($strs[2])
	Send("{ENTER}")
	Send("{ENTER}")
	Sleep(5000)
	MouseClick("left", 810, 261, 1)
	Sleep(1000)
	MouseClick("left", 902, 935, 1)
	MouseClick("left", 902, 835, 1)
	Sleep(3000)
	;Send("^w")
	ConsoleWrite($strs[1] & " " & $strs[2] & Chr(13))
Next
;810 261
FileClose($file)


; ����pname��class��ȡ���ھ�����Ҳ����򷵻�0
Func _WinGetHandleByPnmAndCls($pname, $class)
	; ���ݽ��������ҽ���id
	Local $pid = ProcessExists($pname)
	; ������̴��ڣ�����
	If $pid Then
		Return _WinGetHandleByPidAndCls($pid, $class)
	Else
		Return 0
	EndIf
EndFunc   ;==>_WinGetHandleByPnmAndCls

; ����pid��class��ȡ���ھ�����Ҳ����򷵻�0
Func _WinGetHandleByPidAndCls($pid, $class)
	; ����ʹ��ö�����ж��㴰�ڷ�����WinList�����᷵�ش������ش���
	Local $winArr = _WinAPI_EnumWindowsTop()
	; �������д���,����id��ָ������id�Ƚ�
	For $i = 1 To $winArr[0][0]
		If $pid = WinGetProcess($winArr[$i][0]) And $winArr[$i][1] = $class Then
			; һ�����̻��ж�����ڣ�����Ҫ��class��ɸѡ
			Return $winArr[$i][0]
		EndIf
	Next
	Return 0
EndFunc   ;==>_WinGetHandleByPidAndCls

