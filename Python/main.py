import string
import time
import win32api
import win32con

print(win32api.__file__)
print(win32con.__file__)
print('')

#win32api.keybd_event(win32con.SHIFT_PRESSED, 0, win32con.KEYEVENTF_EXTENDEDKEY,0)

#win32api.keybd_event(win32con.LEFT_CTRL_PRESSED, 0, win32con.KEYEVENTF_EXTENDEDKEY,0)
#win32api.keybd_event(win32con.LEFT_ALT_PRESSED, 0, win32con.KEYEVENTF_EXTENDEDKEY,0)
#win32api.keybd_event(win32con.VK_DELETE, 0, win32con.KEYEVENTF_EXTENDEDKEY,0)

#win32api.keybd_event(win32con.LEFT_CTRL_PRESSED, 0, 0, 0)
#win32api.keybd_event(win32con.LEFT_ALT_PRESSED, 0, 0, 0)
#win32api.keybd_event(win32con.VK_DELETE, 0, 0, 0)

#win32api.keybd_event(win32con.LEFT_CTRL_PRESSED | win32con.LEFT_ALT_PRESSED | win32con.VK_DELETE, 0, 0, 0)

win32api.keybd_event(win32con.VK_LWIN, 0, 0, 0)
win32api.keybd_event(338, 0, 0, 0)
time.sleep(1)

#win32api.keybd_event(win32con.SHIFT_PRESSED, 0, win32con.KEYEVENTF_KEYUP,0)
#win32api.keybd_event(win32con.LEFT_CTRL_PRESSED, 0, win32con.KEYEVENTF_KEYUP,0)
#win32api.keybd_event(win32con.LEFT_ALT_PRESSED, 0, win32con.KEYEVENTF_KEYUP,0)
#win32api.keybd_event(win32con.VK_DELETE, 0, win32con.KEYEVENTF_KEYUP,0)

win32api.keybd_event(win32con.VK_LWIN, 0, win32con.KEYEVENTF_KEYUP, 0)
win32api.keybd_event(338, 0, win32con.KEYEVENTF_KEYUP, 0)

#print(win32con.__dict__)

for entry in win32con.__dict__:
    #print(entry)
    if entry.startswith('VK_') or entry.startswith('KEYEVENT'):
        print(entry)

print(win32api.VkKeyScan('R'))