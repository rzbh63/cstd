
;#include <MsgBoxConstants.au3>
;#include <WindowsConstants.au3>
#include <WinAPI.au3>

AutoItSetOption("SendKeyDelay", 40)

For $count = 1 To 1000
	Local $hWnd = _WinGetHandleByPnmAndCls("Thunder.exe", "XLUEFrameHostWnd")

	If Not $hWnd Then
		MsgBox("asdf", "fff", "����û�ҵ�")
		Exit
	EndIf

	WinActivate($hWnd)

	MouseClick("left", 2395, 1426, 1)
	Sleep(500)
	Send("{F8}")
	MouseClick("left", 2395, 1426, 1)
	Sleep(500)
	Send("{F8}")
	ConsoleWrite("$count=" & $count & @CRLF)
	Sleep(1000000)
	;MouseClick("left", 2002, 624, 1)
Next


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

